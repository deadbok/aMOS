/*       aMOS Kernel entry point
 *
 * History:
 *
 *     Version 0.01     27 Feb 2000  23:13     ObliVion
 *     Initial version.
 */
	.text
	.version	"00.01"
	.code32

#define ASM_FILE

#include <boot/boot.h>
#include <i386/selector.h>
#include <i386/setup.h>

/*
 *      k32_startup:          This is where execution continues after the loader
 *
 */

	.globl		k32_startup
k32_startup:	/* Clear BSS */
				xorl	%eax, %eax
				movl	$_edata, %edi
				movl	$_end, %ecx
				subl	%edi, %ecx
				cld
				rep
				stosb

				pushl	$0
				popfl

				call	init_gdt

				lgdt	gdtptr

				ljmp	$KERNEL_CODE_SEL, $prefetch_clear

prefetch_clear:
				movl	$KERNEL_DATA_SEL, %ax
				movl	%ax, %ds
				movl	%ax, %ss
				movl	%ax, %es
				movl	%ax, %fs
				movl	%ax, %gs

				movl	$KERNEL_STACK, %esp

				call	main
				ret





