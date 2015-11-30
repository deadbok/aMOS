/*       aMOS isr stub
 *
 * History:
 *
 *     Version 0.01     01 Jul 2000  16:12     ObliVion
 *     Initial version.
 */
	.text
	.version	"00.01"
	.code32

#define ASM_FILE

/*
 *      k32_startup:          This is where execution continues after the loader
 *
 */


#define ISR(INT, NAME)	\
	.globl _intr##INT;	\
_intr##INT##:;	\
	cli;	\
	pushl		%ebp;	\
	movl		%esp, %ebp;	\
	leave;	\
	sti;	\
	iret;	\

   .globl   default_isr
default_isr:
	cld
	pushal
	pushw %es
	pushw %ds

	movl	$0x10, %eax
	movw	%ax, %ds
	movw	%ax, %es

   movl  $0x07530753, %eax
   movl  %eax, (0xb8000)

	popw	%ds
	popw	%es
	popal

   iret

