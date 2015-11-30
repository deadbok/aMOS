;     boot.asm
;
;     Boot sector for ANOS
;
; History:
;
;     Version 0.15     5 May 2000  23:14     ObliVion
;     Boot sector now accepts parameters and passes them on to the loader.
;	  Everything else is still lousy
;
;     Version 0.1     27 Feb 2000  23:13     ObliVion
;     Initial version.


BITS 16

org 0h

%define ABS(x) (x-entry+0x7c00)

SECTION .text

entry:					jmp     short start
						nop
boot_setup:
	boot_drive			db  	0
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

						sti
						cld

						mov		[ABS(boot_drive)], dl

						mov		si, ABS(msg_booting)
						call	print

 	; reset drive

						mov		dl, [ABS(boot_drive)]
						xor		ax, ax
						int		13h

	; INT 13H, AH=02h Read sector(s) in to memory
	;
	; 	Input:
	; 		AH		02h
	;		AL		number of sectors to read (must be nonzero)
	;		CH		low eight bits of cylinder number
	;		CL		sector number 1-63 (bits 0-5)
	;				high two bits of cylinder (bits 6-7, hard disk only)
	;		DH		head number
	;		DL		Drive number.
	;		ES:BX	data buffer
	;
	; 	Output:
	;		CF		Set on error.
	;				if AH = 11h (corrected ECC error), AL = burst length
	;		AH		status
	;		AL		number of sectors transferred (only valid if CF set for some
	;				BIOSes)
	;	Copied from Ralf Brown's Interrupt List

						mov		ah, 02h
						mov		al, LOADER_SEC
						mov 	cx,	2
						xor		dh, dh
						mov		dl, [ABS(boot_drive)]
						xor 	bx,	bx
						mov		es, bx
						mov		bx, LOADER_OFF
						int		13h

						jnc 	near .loaded

	; there was an error, try again:
						mov     si, ABS(msg_error)
						call    print

						xor		ax, ax
						int 	13h

						mov		ah, 02h
						mov		al, LOADER_SEC
						int		13h

						jc 		near read_fail

.loaded:				mov     si, ABS(msg_OK)
						call    print

						mov		dx, 03f2h		; Stop floppy motor
						xor		al, al
						out		dx, al


						mov		dl, [ABS(boot_drive)]
						xor		cx, cx
						mov		cl, [ABS(setup_size)]
						mov		si, ABS(boot_setup)


						xor		ax, ax
						push	ax
						mov		ax, LOADER_OFF
						push	ax
						retf

read_fail:				mov		si, ABS(msg_read_fail)
						call	print

fail:					mov		si, ABS(msg_boot_fail)
						call	print

hang:					jmp		short hang


;Messages printed during boot
msg_booting				db	"Loading...", 0
msg_error				db	"Error", 10, 13, 0
msg_OK					db	"OK", 10, 13, 0
msg_read_fail			db	"Error reading disk", 10, 13, 0
msg_boot_fail			db	"Please reboot!!!", 7, 10, 13, 0

;-----------------------------------------------------------------------------
;       print:          print zstring as TTY
;
;       Call with:      SI = address of the first char
;-----------------------------------------------------------------------------

print:					push	ax
						push	bx

	; print string as TTY

						mov     ah, 0eh
						xor		bx, bx

.putch:					lodsb
						cmp		al, 0
						je      .exit
						int     10h
						jmp     short .putch

.exit:					pop		bx
						pop		ax
						ret

times 510-$+entry 		db 	' '
			  			db	55h
			  			db	0AAh
