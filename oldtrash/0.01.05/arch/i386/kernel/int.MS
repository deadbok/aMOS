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

	.globl		_isr
_isr:
	pushl		%ebp
	movl		%esp, %ebp

	pushl		%edi
	pushl		%esi
	pushl		%edx
	pushl		%ecx
	pushl		%ebx
	pushl		%eax

	movl		8(%ebp), %eax
	pushl		%eax
	sti

	call 		handle_int
	cli
	addl		$4, %esp

	popl		%eax
	popl		%ebx
	popl		%ecx
	popl		%edx
	popl		%esi
	popl		%edi

	leave
	ret


.MACRO ISR INT
	cli
	pushl		%ebp
	movl		%esp, %ebp
	pusha
	pushl		$\INT
	call		_isr
	addl		$4, %esp
	sti
	leave
	iret
.ENDM

ISR0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6


	.globl		int_enable
int_enable:
			sti
			ret

	.globl		int_disable
int_disable:
			cli
			ret

.END