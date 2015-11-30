;     bootsector.asm
;
;     Boot sector for ANOS
;
; History:
;
;     Version 0.1     27 Feb 2000  23:13     ObliVion
;     Initial version.


BITS 16

org 7c00h

SECTION .text

entry:					jmp     short start
						nop

;	take a look at "include/boot/boot.h"

; boot_header
;heads					db	2
;sectors					dw	18
;bytes_per_sector		dw	512
;boot_sectors			db	1

; loader_parameters
;size					dd	16
log_level				db	1
;video_mode				dw	3
boot_drive				db	0

start:					cli
	
						mov		ax, cs
						mov		ds, ax
						mov		ss, ax		
						mov		sp, 7ba0h

						sti
						cld
		
						mov		[boot_drive], dl

						mov		si, msg_booting
						call	print
		
 	; reset drive		
 	
						mov		dl, [boot_drive]		
						mov		ax, 00h
						int		13h                    
		
;						mov		dl, [boot_drive]
;						mov		ah, 08h
;						int		13h
;						jc		fail
		
	; INT 13H, AH=08h returns drive parameters.
	;
	; 	Input:
	; 		AH		08h
	;		DL		Drive number.
	;
	; 	Output:
	;		CF		Set on error.
	;		AH		0.
	;		AL		0 on at least some BIOSes.
	;		BL		Drive type (AT/PS2
	;				floppies only).
	;		CH		Low 8 bits of maximum
	;				cylinder number.
	;		CL		Maximum sector number in
	;				bits 0-5, and high 2 bits
	;				of maximum cylinder number
	;				in bits 6-7.
	;		DH		Maximum head number.
	;		DL		The number of drives.
	;		ES:DI	Drive parameter table
	;				(floppies only).		
		
;						mov		[heads], dh
;						xor		ax, ax
;						and		cl, 3fh
;						mov		[sectors], cl
;						
						mov     si, msg_progress
						call    print								
			
	; first load the three other sectors
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
    	
    					mov		ax,	0203h				
						mov 	cx,	2
						mov		ax, 9000h		
						mov		es, ax
						xor 	bx,	bx
						int		13h
						
						mov     si, msg_progress
						call    print								
						
						jnc 	near .loaded
        
	; there was an error, try again:
						mov     si, msg_error
						call    print
						
						xor		ax, ax
						int 	13h

						mov		ax, 0203h
						int		13h

						jc 		near read_fail
			
.loaded:				mov     si, msg_OK
						call    print

           				push	ds
						pop		es
;						mov		di, log_level
;						mov		cx, [size]
						jmp		9000h:0h
						
read_fail:				mov		si, msg_read_fail
						call	print

fail:					mov		si, msg_boot_fail
						call	print

hang:					jmp		short hang


;Messages printed during boot
msg_booting				db	"Loading", 0
msg_progress			db	".", 0
msg_error				db	"Error", 0
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

	; check if should print at all
	
						mov 	al, 0
						cmp		al, [log_level]
						je		.exit
		           
	; print string as TTY		
		
						mov     ah, 0eh
						xor		bx, bx

.putch:					lodsb
						cmp     al, 0
						je      .exit
						int     10h
						jmp     short .putch

.exit:					pop		bx
						pop		ax
						ret

;-----------------------------------------------------------------------------
;       readSectors:    Reads a bunch of sectors into the buffer that
;			ES:BX points to
;
;       Call with:      DX:AX = address of the start sector
;                       DI    = number of sectors to read
;                       ES:BX = destination buffer
;
;-----------------------------------------------------------------------------

;read_sectors:			push	dx
;						push	ax
;						
;						mov     si, msg_progress
;						call    print				
;
;						div		word [sectors]
;						mov		cx, dx                  	;cx = sectors
;						inc		cx
;
;						xor		dx, dx				
;						div		word [heads]
;						mov		dh, dl                 		;ax = track & dh = head
;
;						ror		ah, 1
;						ror		ah, 1
;						xchg	al, ah                 		; al = [7-6] high cylinder & [5-0] sector
;															; ah = cylinder
;						or		cx, ax
;
;						mov		dl, byte [boot_drive]
;						mov		ax, 0201h
;						int		13h
;						jnc		.ok
;
;						xor		ax, ax						; error: reset disk and re-read
;						int		13h							; may lead to a continuos loop
;						pop		ax
;						pop		dx
;						
;						mov     si, msg_error
;						call    print				
;
;						jmp		short read_sectors
;
;.ok:					add     bx, word [bytes_per_sector]
;						jnc     .same_segment
;
;						mov     ax, es
;						add     ah, 10h
;						mov     es, ax
;		
;.same_segment:			mov     si, msg_progress
;						call    print				
;
;						pop     ax
;						pop     dx
;						add     ax, 1
;						adc     dx, byte 0
;						dec     di
;						jnz     read_sectors
;		
;.exit:         			ret

times 510-$+entry 		db 	' '
			  			db	55h
			  			db	0AAh
				
