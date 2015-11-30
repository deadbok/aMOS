;	video.inc
;
;	Video routines for the aMOS loader
;
;	History:
;
;		Version 0.1		17 Apr 2000  19:24		ObliVion
;		Initial version.

%ifndef		LOADER_VIDEO_INC
%define		LOADER_VIDEO_INC

;-----------------------------------------------------------------------------
;		_no_cursor():		Turns of the cursor
;
;-----------------------------------------------------------------------------

proc no_cursor
						mov		ecx, 2000h
						mov		ah, 01h
						int		10h 
endproc

;-----------------------------------------------------------------------------
;       _putch(char ch):         	print character
;
;-----------------------------------------------------------------------------

proc putch
						push	eax
						push	ebx

						mov		al, '0'
						mov     ah, 0eh
						xor		ebx, ebx
						int		10h

						pop		ebx
						pop		eax

endproc

;-----------------------------------------------------------------------------
;       print:         	print a zstring
;
;-----------------------------------------------------------------------------

print:
						mov     ah, 0eh
						xor		bx, bx

.putch:					lodsb
						cmp		al, 0
						je      .exit
						int     10h
						jmp     short .putch

.exit:					ret



%endif
