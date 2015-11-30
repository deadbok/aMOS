;	defs.inc
;
;	Common defines and structs for the aMOS loader
;
;	History:
;
;		Version 0.1		24 Apr 2000  19:49		ObliVion
;		Initial version.

%ifndef		LOADER_DEFS_INC
%define		LOADER_DEFS_INC

%define BOOT_SEG		07c0h
%define RM_STACK		7ff0h
%define PM_INIT_STACK	67ff0h

%define LOADER_OFF		8000h
%define LOADER_SEC		10h

D_DATA				equ		  00010010b   	;segment is data, read/write 
D_CODE    			equ		  00011010b		;segment is code, read/exec 
D_PRESENT			equ		  10000000b		;Present

D_4KGRAN			equ		  10000000b		;4K Granularity
D_32BIT				equ	  	  01000000b		;Default to 32 bit mode (USE32)

; Copied from c32.mac in the NASM 0.98 distrubtion
; NASM macro set to make interfacing to 32-bit programs easier -*- nasm -*-

%imacro proc 1			; begin a procedure definition
%push proc
          global %1
%1:       push ebp
          mov ebp,esp
%assign %$arg 8
%define %$procname %1
%endmacro

%imacro arg 0-1 4		; used with the argument name as a label
	  equ %$arg
%assign %$arg %1+%$arg
%endmacro

%imacro endproc 0
%ifnctx proc
%error Mismatched `endproc'/`proc'
%else
	  leave
	  ret
__end_%$procname:		; useful for calculating function size
%pop
%endif
%endmacro

%endif
