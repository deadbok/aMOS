;     asm.asm
;
;       aMOS Kernel loader stub
;
; History:
;
;     Version 0.01     27 Feb 2000  23:13     ObliVion
;     Initial version.

BITS 16

SECTION .text

%define ABS(x) (x-_start+LOADER_OFF)

;-----------------------------------------------------------------------------
;      _start:          This is where execution continues after the bootsector
;
;-----------------------------------------------------------------------------

extern	cmain

global			_start
_start:  		jmp		short stub
				nop

global			loader_setup
loader_setup:
global			_boot_drive
	_boot_drive			db  	0

global			_bytes_per_sector
	_bytes_per_sector	dw		0

global			_boot_sectors
	_boot_sectors		db		0

global			_loader_start
	_loader_start		dd		0

global			_kernel_LBA
	_kernel_LBA			dd		0

global			_kernel_sectors
	_kernel_sectors		dd		0

%include "../include/boot/defs.inc"

global stub
stub:			cld
			
				xor		ax, ax
				mov     es, ax

				mov		di, ABS(_boot_drive)
				rep		movsb

				cli

				xor		ax, ax
				mov     ds, ax
				mov     es, ax
				mov		ss, ax
				mov     bp, RM_STACK
				mov     sp, bp

				sti

				mov		si, ABS(msg_init)
				call	print

				mov		si, ABS(msg_386_check)
				call	print

				call	check_386
				test	ax, ax
				jnz		short no386
				mov		si, ABS(msg_present)
				call	print

				call    dword [cs:0x96e4]


;				mov		si, ABS(msg_PM_switch)
;				call	print

;				call	pm_switch

;BITS 32
;				
;				call	_putch
;
;.pm_hang			jmp		short .pm_hang

;BITS 16


no386:			mov		si, ABS(msg_not_present)
				call	print

.error:			mov		si, ABS(msg_error)
				call 	print

.hang:			jmp 	short .hang
				
msg_init					db		"Loader init...", 10, 13, 0
msg_386_check				db		"    Checking for 386+ processor...", 0
msg_PM_switch				db		"    Switching to Protected Mode...", 0
msg_not_present				db		"NOT present", 10, 13, 0
msg_present					db		"present", 10, 13, 0
msg_error					db		10, 13, "An error occured!" , 10, 13 
							db		"Please reboot the system with another OS!", 0
	
align 4, db 0
gdt_ptr:
		gdt_limit			dw		gdt_length
		gdt_addr			dd		ABS(gdt)

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
%include "disk.inc"
%include "video.inc"

pm_stack:					dd		PM_INIT_STACK		