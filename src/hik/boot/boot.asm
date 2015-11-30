;		boot.asm
;
;		Boot sector for aMOS
;
;	History:
;		Version 0.02.00  16 Sep 2000  18:22     ObliVion
;		The bootsector now uses loader_start & loader_sectors
;		to load the aMOS loader.
;
;		Version 0.01.06  14 Sep 2000  03:34     ObliVion
;		Just a few minor rearangements before the big blow (0.02.00)
;		Although it is not the optimal solution I have decided that
;		I do not believe in pure 8086 compatible code
;		So don't use this bootsector on computers from the stoned age
;
;		Version 0.01.05  5  May 2000  23:14     ObliVion
;		Boot sector now accepts parameters and passes them on to the loader.
;		Everything else is still lousy
;
;		Version 0.01.00  27 Feb 2000  23:13     ObliVion
;		Initial version.


BITS 16

org 0h

%define ABS(x) (x-entry+0x7c00)

SECTION .text

entry:				cld
					jmp			short start

boot_setup:
	boot_drive			db		0
	bytes_per_sector	dw		0
	boot_sectors		db		0
	loader_start		dd		0
	loader_sectors		dd		0
	kernel_start		dd		0
	kernel_sectors		dd		0
	setup_size			db		20

%include "../../../include/boot/defs.inc"

start:					cli

						xor		ax, ax
						mov		ds, ax
						mov		ss, ax
						mov		sp, RM_STACK

						mov		[ABS(boot_drive)], dl

						mov		si, ABS(msg_booting)
						call	print

 	; reset drive

						mov		dl, [ABS(boot_drive)]
						xor		ax, ax
						int		13h

	; INT 13 - DISK - GET DRIVE PARAMETERS (PC,XT286,CONV,PS,ESDI,SCSI)
	;
	;	Input:
	; 		AH		08h
	;		DL		drive (bit 7 set for hard disk)
	;
	;	Output:
	;		CF 		set on error
	;		AH		status
	;
	;		CF		clear if successful
	;		AH		00h
	;		AL		00h on at least some BIOSes
	;		BL		drive type (AT/PS2 floppies only)
	;		CH		low eight bits of maximum cylinder number
	;		CL		maximum sector number (bits 5-0)
	;				high two bits of maximum cylinder number (bits 7-6)
	;		DH		maximum head number
	;		DL		number of drives
	;		ES:DI	drive parameter table (floppies only)
	;
	;	Copied from Ralf Brown's Interrupt List
		
						mov		ax, 08h
						int		13h
						
						inc		dh						
						mov		[ABS(heads)+1], dh
					
						mov		al, cl
						and		al, 3fh
						mov		[ABS(sectors)+1], al

						xor		ax, ax
						mov		al, cl
	;					andb	$0xc0, %al
						shl		ax, 2
						mov		al, cl
						inc		ax
						mov		[ABS(cylinders)], ax

						mov		cx, [ABS(loader_sectors)]	; Number of sectors to load

						xor		dx, dx
						mov		ax, [ABS(loader_start)]		; Start sector

						mov		es, dx
						mov		bx, LOADER_OFF

.load:					call	readsector
						add     bx, word [ABS(bytes_per_sector)]

						add     ax, 1
						adc     dx, byte 0

						mov		si, ABS(msg_read)
						call	print

;						xor		dx, dx
;						mov		es, dx

						dec     cx												
						jnz     .load
						

.loaded:				mov     si, ABS(msg_OK)
						call    print

						mov		dx, 03f2h		; Stop floppy motor
						xor		al, al
						out		dx, al

						mov		dl, [ABS(boot_drive)]
						xor		cx, cx
						mov		cl, [ABS(setup_size)]
						mov		si, ABS(boot_setup)

						jmp		0:LOADER_OFF

read_fail:				mov		si, ABS(msg_read_fail)
						call	print

fail:					mov		si, ABS(msg_boot_fail)
						call	print

hang:					jmp		short hang


;Messages printed during boot
msg_booting				db	"Loading", 0
msg_error				db	"Error", 10, 13, 0
msg_read				db	".", 0
msg_OK					db	"OK", 10, 13, 0
msg_read_fail			db	"Error reading disk", 10, 13, 0
msg_boot_fail			db	"Please reboot!", 10, 13, 0

; Disk geometry data
heads					dw	0
cylinders				dw	0
sectors					dw	0

;-----------------------------------------------------------------------------
;       print:          print zstring as TTY
;
;       Call with:      SI = address of the first char
;-----------------------------------------------------------------------------

print:					pusha

	; print string as TTY

						mov     ah, 0eh
						xor		bx, bx

.putch:					lodsb
						cmp		al, 0
						je      .exit
						int     10h
						jmp     short .putch

.exit:					popa
						ret
																		
;-----------------------------------------------------------------------------
;		readsector:		Read one sector to ES:BX
;
;		Call with:		DX:AX = address of the start sector
;						ES:BX = destination buffer
;
;-----------------------------------------------------------------------------

readsector:				pusha
						push	dx				
						push	ax
														; chs_geometry->sector=sector%(disk_geometry.sector)+1;
														; chs_geometry->head=sector/disk_geometry.sector;
														; chs_geometry->cylinder=chs_geometry->head/disk_geometry.head;
														; chs_geometry->head%=disk_geometry.head;


						div		word [ABS(sectors)]		; sector=sector mod sectors
						mov		cx, dx					; cx = sector
						inc		cx					 	; ax = temp
		
						xor		dx, dx
						div		word [ABS(heads)]		; cylinder= head div heads
														; ax = cylinder
														; head = temp mod heads
														; dx = head
						mov     dh, dl             		;ax = cylinder & dl = head

						ror     ah, 2
						xchg    al, ah             		; al = [7-6] high cylinder & [5-0] sector
														; ah = cylinder
						or      cx, ax

						mov		dl, [ABS(boot_drive)]

						mov     ax, 0201h
						int   	13h

	; INT 13 - DISK - READ SECTOR(S) INTO MEMORY
	;
	;	Input:
	; 		AH		02h
	;		AL		number of sectors to read (must be nonzero)
	;		CH		low eight bits of cylinder number
	;		CL		sector number 1-63 (bits 0-5)
	;				high two bits of cylinder (bits 6-7, hard disk only)
	;		DH		head number
	;		DL		drive number (bit 7 set for hard disk)
	;		ES:BX	data buffer
	;	Output:
	; 		CF 		set on error
	; 	  			if AH = 11h (corrected ECC error), AL = burst length
	;
	;		CF		clear if successful
	;		AH		status
	;		AL		number of sectors transferred (only valid if CF set for some
	;				BIOSes)								
	;
	;	Copied from Ralf Brown's Interrupt List

					jnc     short .ok

					mov		si, ABS(msg_error)
					call	print

					xor     ax, ax			; error: reset disk and re-read
					int     13h				; may lead to a continuos loop
					pop     ax
					pop     dx
					jmp     short readsector

.ok:				pop     ax
					pop     dx
					popa

.exit				ret


times 510-$+entry 		db 	' '
			  			db	55h
			  			db	0AAh

;						mov		dh, [ABS(boot_drive)]	; dh = drive
;						xchg	dh, dl					; dl = drive, dh = head
;
;						mov		cx, di
;						mov		ch, ah
;						shl		cx, 6					; ah = high two bits of cylinder
;						mov		cl, bl					
;						xchg	cl, ch					; ch = low eight bits of cylinder number
														; cl = sector number 1-63 (bits 0-5)
														;		high two bits of cylinder
