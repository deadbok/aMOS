;	cpu.inc
;
;	CPU related routines for the aMOS loader
;
;	History:
;
;		Version 0.1		17 Apr 2000  19:24		ObliVion
;		Initial version.

%ifndef		LOADER_CPU_INC
%define		LOADER_CPU_INC

;-----------------------------------------------------------------------------
;       check_386:			Check if the CPU is 386 or higher
;
;       Return:				1	the CPU is not 386 or higher
;							0	the CPU is 386 or higher
;-----------------------------------------------------------------------------
BITS 16

check_386
				pushf				; 8088/86
				pop 	ax
				mov 	cx, ax
				and 	ax, 0fffh
				push 	ax
				popf
				pushf
				pop		ax
				and		ax, 0f000h
				cmp		ax, 0f000h
				je		.no386

.no86:			or 		cx, 0f000h		; 80286
				push 	cx
				popf 	
				pushf
				pop 	ax
				and 	ax, 0f000h
				jnz		short .386

.no386:			mov		ax,	1
				jmp		short .exit

.386:			mov		ax, 0			; 80386 ;=) COOL;
.exit:			ret

%endif
