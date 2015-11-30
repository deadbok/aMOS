/*       aMOS Kernel entry point
 *
 * History:
 *		Version 0.01.10	10 Sep 2000	14:07	ObliVion
 *		Made it possible for the kernel to return to
 *		the loader, when execution is given up.
 *		(If the kernel restores the the startup 
 *		enviroment like this code)
 *
 *		Version 0.01.00	27 Feb 2000	23:13	ObliVion
 *		Initial version.
 */
	.text
	.version	"00.01"
	.code32

#define ASM_FILE

#include <boot/boot.h>
#include <kernel/selector.h>
#include <kernel/setup.h>

/*
 *      k32_startup:          This is where execution continues after the loader
 *
 */

	.globl		k32_startup
k32_startup:	pushl	%ebp
				movl	%esp, %ebp

				movl	%esp, old_sp

				cli

				/* Clear BSS */
				xorl	%eax, %eax
				movl	$_edata, %edi
				movl	$_end, %ecx
				subl	%edi, %ecx
				cld
				rep
				stosb

				call	init_gdt
				lgdt	gdtptr

				ljmp	$KERNEL_CODE_SEL, $prefetch_clear

prefetch_clear:
				movl	$KERNEL_DATA_SEL, %eax
				movl	%eax, %ds
				movl	%eax, %es
				movl	%eax, %fs
				movl	%eax, %gs

				lss		kstack_frame, %esp 
				call	init_idt

				lidt	idtptr
				
	   			pushfl                       /* Clear the NT flag           */
				andl $0xffffbfff,(%esp)  
				popfl

				call	main

				movl	old_sp, %esp

				movl	%esp, %ebp
				popl	%ebp			
				ret

old_sp:
	.long	0

	.align 4										
	.space KSTACK_SIZE
	.globl kstack
kstack:

.align 4
kstack_frame:	
		.long	kstack
		.word	KERNEL_DATA_SEL
