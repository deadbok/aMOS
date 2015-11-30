/*       aMOS Kernel loader stub
 *
 * History:
 *
 *     Version 0.01.00     27 Feb 2000  23:13     ObliVion
 *     Initial version.
 */
	.text
	.version	"00.01.00"
	.code16

#define ASM_FILE
#define ABS(x) (x-_start+LOADER_OFF)
#define GDT_INDEX(x) (x-gdt)

#define DATA32		.byte 0x66;
#define ADDR32		.byte 0x67;

#include <boot/boot.h>
#include "asm.h"

/*
 *      _start:          This is where execution continues after the bootsector
 *
 */

	.globl		_start
_start:  		jmp		stub
				nop

	.globl		loader_setup
loader_setup:
boot_drive:		.byte	  	0
bytes_per_sector:	.word		0
boot_sectors:		.byte		0
loader_start:		.long		0
loader_sectors:		.long		0
kernel_start:		.long		0
kernel_sectors:		.long		0

	.globl		stub
stub:			cld

				xorw	%ax, %ax
				movw	%ax, %es
				/*call	halt*/
	
				movw	$ABS(loader_setup), %di
				rep
				movsb

				cli

				xorw	%ax, %ax
				movw	%ax, %ds
				movw    %ax, %es
				movw	%ax, %ss
				movw    RM_STACK, %bp
				movw    %bp, %sp

/*				sti*/

				movw	$ABS(msg_init), %si
				call	print

				call	no_cursor

				movw	$ABS(msg_386_check), %si
				call	print

				call	check_386
				testw	%ax, %ax
				jnz		1f

				movw	$ABS(msg_present), %si
				call	print

				movw	$ABS(msg_pm_switch), %si
				call	print

				call	A20_enable

				call	pm_switch
.code32

				call	main

/*		If everything works out we should never return from main.
 *		So if we do switch to Real Mode and print fatal error and die
 */

				call	rm_switch

.code16

				jmp		2f

1:				movw	$ABS(msg_not_present), %si
				call	print

2:				movw	$ABS(msg_error), %si
				call 	print

3:				jmp		3b

/*
 *		Messages for the asm stub
 *  
 */

				
msg_init:			.asciz		"Loader init...\n\r"
msg_386_check:		.asciz		"    Checking for 386+ processor..."
msg_pm_switch:		.asciz		"    First Protected Mode switch..."
msg_not_present:	.asciz		"NOT present\n\r"
msg_present:		.asciz		"present\n\r"
msg_error:			.asciz		"An error occurred!\n\n\rPlease reboot the system with another OS!"

/*
 *		Turns off cursor
 *  	(Twice to make sure)
 */

no_cursor:
				movw	$0x2000, %cx
				movb	$0x01, %ah
				int		$0x10
				movw	$0x2000, %cx
				movb	$0x01, %ah
				int		$0x10
				ret

/*
 *		Prints a zstring using int 10h AH=0x0e
 *
 */
/*	INT 10 -	VIDEO - TELETYPE OUTPUT
 *				AH = 0Eh
 *				AL = character to write
 *				BH = page number
 *				BL = foreground color (graphics modes only)
 *		Return:	nothing
 *		Desc:	display a character on the screen, advancing the cursor and scrolling
 *				the screen as necessary
 *		Notes:	characters 07h (BEL), 08h (BS), 0Ah (LF), and 0Dh (CR) are interpreted
 *				and do the expected things
 *				IBM PC ROMs dated 4/24/81 and 10/19/81 require that BH be the same as
 *				the current active page
 *		BUG:	if the write causes the screen to scroll, BP is destroyed by BIOSes
 *				for which AH=06h destroys BP
 */

print:
				movb	$0x0e, %ah
				xorw	%bx, %bx

1:				lodsb
				cmpb	$0, %al
				je      2f
				int     $0x10
				jmp     1b

2:				ret

/*
 *		Checks if the system have at leats a 386 processor
 *
 */

check_386:
				pushfw				/* 8088/86*/
				popw 	%ax
				movw 	%ax, %cx
				andw 	$0x0fff, %ax
				pushw 	%ax
				popfw
				pushfw
				popw	%ax
				andw	$0xf000, %ax
				cmpw	$0xf000, %ax
				je		1f

				orw 	$0xf000, %cx		/* 80286*/
				pushw 	%cx
				popfw
				pushfw
				popw 	%ax
				andw 	$0xf000, %ax
				jnz		2f

1:				movw	$1,	%ax
				jmp		3f

2:				movw	$0, %ax			/* 80386 ;=) COOL;*/
3:				ret

/*
 *		Wait till the keyboard buffer is empty
 *
 * +:	This will hang the machine until the buffer is cleared
 */
	.code16
kb_wait:
		/* Read from port 0x64 and check bit 1
		 * If 1 then the input buffer is full so wait until cleared
		 *
		 */
				inb		$0x64, %al
				testb	$0x02, %al
				jnz		kb_wait
				ret



/*
 *		Enables the A20 line
 *
 *
 *  	i:	os-faq, grub
 *		+:	Maybe I should add some code that clears the output buffer
 *			before I begin sending commands (Linux boot does this)
 */
	.code16
A20_enable:
				cli

				call 	kb_wait
				movb	$0xad, %al	/*0xad = disable keyboard*/
				outb	%al, $0x64

				call 	kb_wait
				movb	$0xd0, %al	/*0xd0 = read*/
				outb	%al, $0x64

				call 	kb_wait
				inb		$0x60		/*read the data*/
				movl	%eax, %ebx

				call 	kb_wait
				movb	$0xd1, %al	/*0xd1 = write*/
				outb	%al, $0x64

				call 	kb_wait
				movl	%ebx, %eax
				orb	$0x02, %al
				outb	%al, $0x60

				call 	kb_wait
				movb	$0xae, %al	/*0xae = enable keyboard*/
				outb	%al, $0x64

				call 	kb_wait
				ret

/*
 *		PM switch
 *
 */
	.code16
pm_switch:
				cli

				lgdt	ABS(gdt_desc)

				movl	%cr0, %eax
				orl		$0x01, %eax
				movl	%eax, %cr0

				DATA32
	/*ljmp*/	.byte 0xea
				.long pm_code_seg
				.word CODE_32
	.code32

pm_code_seg:
				movw	$DATA_32, %ax
				movw	%ax, %ds
				movw	%ax, %es
				movw	%ax, %fs
				movw	%ax, %gs
				movw	%ax, %ss

				movl	(%esp), %eax
				movl	 %eax, RM_STACK

				movl	pm_stack, %eax
				movl	%eax, %esp
				movl	%eax, %ebp

				/* get return address*/
				movl	RM_STACK, %eax
				movl	%eax, (%esp)

				ret

rm_switch:
	.code32
				lgdt	gdt_desc

				movl	%esp, %eax
				movl	%eax, pm_stack

				/* save return address*/
				movl	(%esp), %eax
				movl	%eax, RM_STACK

				/* set rm stack*/
				movl	$RM_STACK, %eax
				movl	%eax, %esp
				movl	%eax, %ebp

				movw	$DATA_16, %ax
				movw	%ax, %ds
				movw	%ax, %es
				movw	%ax, %fs
				movw	%ax, %gs
				movw	%ax, %ss

				ljmp	$CODE_16, $not_quite_rm

not_quite_rm:
	.code16

				movl	%cr0, %eax
				andl	$0xfffffffe, %eax
				movl	%eax, %cr0

				ljmp	$0x00,$rm_code_seg


	.code16
rm_code_seg:

				xorl	%eax, %eax
				movw	%ax, %ds
				movw	%ax, %es
				movw	%ax, %fs
				movw	%ax, %gs
				movw	%ax, %ss

				sti

				ret

/*
 *		putch(char ch)
 *		print character
 */
	.code32
	.globl BIOS_putch
BIOS_putch:
				push	%ebp
				push	%eax
				push	%ebx

				movb	0x10(%esp), %bl

				call	rm_switch

	.code16
				movl	%ebx, %eax
				movb	$0x0e, %ah
				xorl	%ebx, %ebx
				int		$0x10

				call	pm_switch
	.code32

				pop		%ebx
				pop		%eax
				pop		%ebp
				ret

/*
 *		int disk_reset(char drive)
 *		Resets the disk controller
 */
	.code32
	.globl disk_reset
disk_reset:
				pushl	%ebp
				movl	%esp, %ebp

				pushl	%ebx
				pushl	%edx

				movl	0x08(%ebp), %edx

				call	rm_switch

	.code16

				xorl	%eax, %eax
				int		$0x13
				movl	%eax, %ebx

				call	pm_switch
	.code32
				movl	%ebx, %eax
				xorb	%al, %al
				xchg	%al, %ah

				popl	%edx
				popl	%ebx

				movl	%esp, %ebp
				popl	%ebp
				ret

/*
 *		extern int  disk_info_standard(int drive,
 *						   unsigned long *cylinders,
 *							   unsigned long *heads,
 *							   unsigned long *sectors);
 *
 *		Uses standard int 13 AH=08h to get disk info
 */
	.code32
	.globl disk_info_standard
disk_info_standard:
				pushl	%ebp
				movl	%esp, %ebp

				pushl	%ebx
				pushl	%ecx
				pushl	%edx
				pushl	%esi

				movl	0x08(%ebp), %edx

				call	rm_switch

	.code16

				movl	$0x0800, %eax
				int		$0x13

				movl	%eax, %ebx

				call	pm_switch

	.code32

				test	%ebx, %ebx
				jz		1f
				movl	%ebx, %eax
				jmp		2f

1:				leal	0x10(%esp), %ebp		/*restore ebp*/

				xorl	%eax, %eax 	/*heads*/
				movb	%dh, %al
				incl	%eax
				movl	0x10(%ebp), %esi
				movl	%eax, (%esi)

				xorl	%eax, %eax 	/*sector*/
				movb	%cl, %al
				andb	$0x3f, %al
				movl	0x14(%ebp), %esi
				movl	%eax, (%esi)

				xorl	%eax, %eax 	/*cylinder*/
				movb	%cl, %al
/*				andb	$0xc0, %al*/
				shll	$2, %eax
				movb	%ch, %al
				incl	%eax
				movl	0x0c(%ebp), %esi
				movl	%eax, (%esi)

				xorl	%eax, %eax 	/*everything should be clear now (no error)!!!*/

2:
				xorb	%al, %al
				xchgb	%al, %ah

				popl	%esi
				popl	%edx
				popl	%ecx
				popl	%ebx

				movl	%esp, %ebp
				popl	%ebp
				ret


/*
 *		int disk_read_standard(int disk,
 *							   unsigned long cylinder,
 *							   unsigned long head,
 *							   unsigned long sector,
 *							   unsigned long sectors,
 *							   unsigned long segment);
 *
 *		Uses standard int 13 AH=02h to read sectors from disk
 */
	.code32
	.globl disk_read_standard
disk_read_standard:
				pushl	%ebp
				movl	%esp, %ebp

				movl	0x0c(%ebp), %eax
				movb	%al, %ch

				movl	0x14(%ebp), %ebx
				movb	%bl, %al	/* al=sector. rest of eax cylinder*/
				shlb	$2, %al
				shrw	$2, %ax
				movb	%al, %cl

				movl	0x10(%ebp), %eax    /*head*/
				movb	%al, %dh

				movl	0x1c(%ebp), %ebx    /*segment*/

				movl	0x08(%ebp), %eax
				movb	%al, %dl

				movl	0x18(%ebp), %eax
				movb	$0x02, %ah
				movw	%ax, %di

				movw	$0x13, %si /*Number of retries*/

				call	rm_switch

	/* INT 13H, AH=02h Read sector(s) in to memory
	 *
	 * 	Input:
	 *		AH		02h
	 *		AL		number of sectors to read (must be nonzero)
	 *		CH		low eight bits of cylinder number
	 *		CL		sector number 1-63 (bits 0-5)
	 *				high two bits of cylinder (bits 6-7, hard disk only)
	 *		DH		head number
	 *		DL		Drive number.
	 *		ES:BX	data buffer
	 *
	 * 	Output:
	 *		CF		Set on error.
	 *				if AH = 11h (corrected ECC error), AL = burst length
	 *		AH		status
	 *		AL		number of sectors transferred (only valid if CF set for some
	 *				BIOSes)
	 *	Copied from Ralf Brown's Interrupt List
	 */


	.code16
				movw	%bx, %es

1:				movw	%di, %ax
				xorl	%ebx, %ebx

				int		$0x13
				jnc		2f

				xorw	%ax, %ax
				int		$0x13

				decw	%si
				cmpw	$0, %si
				jne		1b

2:				movl	%eax, %ebx

				call	pm_switch

	.code32

				movl	%ebx, %eax
				xorb	%al, %al
				xchg	%al, %ah

				movl	%esp, %ebp
				popl	%ebp
				ret

/*
 * stop_floppy()
 *
 */
	.code32
	.globl stop_floppy
stop_floppy:
				movw	$0x3f2, %dx
				xorb	%al, %al
				outb	%al, %dx
				ret
/*
 * int basic_detect(void);
 *
 */
	.code32
	.globl basic_detect
basic_detect:
	/* Int 10h function 12h with subfunction bl=10h
	 * will return with bl=10h, if video card isn't at least an EGA
	 * since the call is'nt supporisn't
     */
				pushl	%ebp
				movl	%esp, %ebp

				call	rm_switch

	.code16
				movb	$0x12, %ah
				movb	$0x10, %bl
				int		$0x10

				call	pm_switch

	.code32
				cmpb	$0x10, %bl
				jne		1f
				movl	$VIDEO_TYPE_CGA, %eax
				jmp		3f

1:				call	rm_switch

	.code16
				movw	$0x1a00, %ax
				int		$0x10

				movw	%ax, %bx
				call	pm_switch

	.code32

				cmpb	$0x1a, %bl
				jne		2f
				movl	$VIDEO_TYPE_VGA, %eax
				jmp		3f

2:				movl	$VIDEO_TYPE_EGA, %eax

3:				movl	%esp, %ebp
				popl	%ebp
				ret

/*
 * int get_state(void);
 *
 * Returns bl=Current display mode in bit 0..6, bh=Characters per column, rest=current page
 * Only works on EGA/VGA
 */
	.code32
	.globl get_state
get_state:
				pushl	%ebp
				movl	%esp, %ebp

				call	rm_switch

	.code16
				movb	$0x0f, %ah
				int		$0x10

				xorl	%edx, %edx
				movw	%bx, %dx
				shll	$8, %edx
				movw	%ax, %dx
				movl	%edx, %ebx

				call	pm_switch

	.code32
				movl	%ebx, %eax
				movl	%esp, %ebp
				popl	%ebp
				ret

/*
 * int engage(void);
 *
 * Set up the registers and invoke the kernel
 */
	.code32
	.globl engage
engage:			call	*kernel_addr
				ret
	.code32
	.globl halt				
halt:			hlt


/*
 *		GDT stuff
 *
 */

	.p2align	2	/* force 4-byte alignment */

gdt_desc:
gdt_limit:			.word		0x27
gdt_addr:			.long		gdt

gdt:
gdt_null:	/* null segment thingy */
					.word		0, 0
					.byte		0, 0, 0, 0
gdt_code:	/* code segment */
					.word		0xffff, 0
					.byte		0, D_PRESENT | D_CODE, D_32BIT | D_4KGRAN | 0x0f, 0
gdt_data:	/* data segment */
					.word		0xffff, 0
					.byte		0, D_PRESENT | D_DATA, D_32BIT | D_4KGRAN | 0x0f, 0
gdt_code16:	/* 16 bit CS */
					.word		0xffff, 0
					.byte		0, 0x9e, 0, 0
gdt_data16:	/* 16 bit DS */
					.word		0xffff, 0
					.byte		0, 0x92, 0, 0

/*gdt_length equ $ - gdt - 1*/

pm_stack:
					.long		PM_INIT_STACK



