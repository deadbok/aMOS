;     loader.asm
;
;     Kernel loader for aMOS
;
; History:
;
;     Version 0.01     27 Feb 2000  23:13     ObliVion
;     Initial version.

BITS 16

SECTION .text

extern	_cmain
global			_main
_main:  		jmp		short start
				nop

boot_drive				db  	0
kernel_LBA				dd		0
kernel_sectors			dd		0

%include "defs.inc"

;-----------------------------------------------------------------------------
;       start:          Main loader program
;
;       Call with:      ES:DI:	Address of the Boot Parameters
;						CX:	 	Size (in bytes) of the Boot Parameter structure
;-----------------------------------------------------------------------------


start:			cli
				
				mov     ax, cs
				mov     ds, ax
				mov     ax, 7fffh
				mov     ss, ax

				sti

				mov		[boot_drive], dl				
				
;				mov		si, ParamSize
;				rep		movsb

				mov     si, msg_loader_init
				call    print

				call	no_cursor

				mov     si, msg_386_check
				call    print
		
				call	check_386
				cmp		ax,	1
				jne		fail

				mov     si, msg_present
				call    print
				
				mov		[boot_drive], dl				
				call	_cmain
				
				mov		si, msg_init_OK
				call	print
				jmp		short hang

				mov     si, msg_PM_switch
				call    print

				cli

				xor  	eax, eax
 				mov		ax, ds
  				shl  	eax, 4  	         ;eax = linear address of data segment
  				add  	[gdt_ptr+2], eax     ;patch psuedo-descriptor
   				
				lgdt	[gdt_ptr]

				mov		eax, cr0
				or		eax, 01h
				mov		cr0, eax

;It's seems that it would be quite cool to INCBIN "main.bin" the C part if this code
;after it's been converted to a pure binary image by OBJCOPY


				mov		ax, 10h
				mov		ds, ax
				mov		es, ax
				mov		fs, ax
				mov		gs, ax
				mov		ss, ax

				jmp		dword (gdt_code-gdt):PM_cseg

BITS 16
		
fail:			add		al, "0"
				mov		[msg_error], al

				add		ah, "0"
				mov		[msg_error+1], ah

				mov		si, msg_error
				call	print

				mov     si, msg_reboot_error
				call    print
				
hang:			jmp		short hang
		
;-----------------------------------------------------------------------------
;
;	Text Messages
;
;-----------------------------------------------------------------------------

		msg_loader_init    	db      "aMOS Loader init...", 10, 0
		msg_386_check		db      "    Checking for 386+ processor...", 0
		msg_PM_switch		db		"    Switching to Protected Mode...", 0
		msg_reboot_error	db		7, 10, "Please reboot with another OS", 10, 0
		msg_present			db		"Present", 10, 0
		msg_not_present		db		"NOT Present", 10, 0
		msg_error			db		"   error", 10, 0
		msg_init_OK			db		"...OK", 10, 0

align 4, db 0
gdt_ptr:
		gdt_limit			dw		gdt_length
		gdt_addr			dd		gdt

gdt:
gdt_null:	; null segment thingy
							dw		0, 0
							db		0, 0, 0, 0
gdt_code:	; code segment
							dw		0ffffh, 0
							db		0, D_PRESENT | D_CODE, D_32BIT | D_4KGRAN | 0fh, 0
gdt_data:	; data segment
							dw		0ffffh, 0
							db		0, D_PRESENT | D_DATA, D_32BIT | D_4KGRAN | 0fh, 0
gdt_code16:	; 16 bit real mode CS 
							dw		0ffffh, 0
							db		0, 9eh, 0, 0
gdt_data16:	; 16 bit real mode DS
							dw		0ffffh, 0
							db		0, 92h, 0, 0

gdt_length equ $ - gdt - 1

%include "cpu.inc"
%include "video.inc"


PM_cseg:		

BITS 32

PM_hang:		jmp		short PM_hang

;times 4094-$+_main 		db 	' '
