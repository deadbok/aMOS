/*       aMOS HIK isr stub
 *
 * History:
 *
 *     Version 0.01.01     01 Jul 2000  00:36     ObliVion
 *     Cleanup.
 *
 *     Version 0.01.00     01 Jul 2000  16:12     ObliVion
 *     Initial version.
 */
	.text
	.version	"00.01"
	.code32

#define ASM_FILE

	.MACRO isr INTR
	.GLOBAL _intr\INTR
	_intr\INTR:
	cli
	cld

	pushal
	pushw		%ds
	pushw		%es

	movl		$0x10, %eax
	movw		%ax, %ds
	movw		%ax, %es			
		
	xorl		%eax, %eax
	movl		$\INTR, %eax
	pushl		%eax
	call		handle_intr
	addl		$4, %esp

1:	popw		%es
	popw		%ds
	popal

	sti
	iret
	
	.ENDM

	isr 0
	isr 1
	isr 2
	isr 3
	isr 4
	isr 5
	isr 6
	isr 7
	isr 8
	isr 9
	isr 10
	isr 11
	isr 12
	isr 13
	isr 14
	isr 15
	isr 16
	isr 17
	isr 18
	isr 19
	isr 20
	isr 21
	isr 22
	isr 23
	isr 24
	isr 25
	isr 26
	isr 27
	isr 28
	isr 29
	isr 30
	isr 31
	isr 32
	isr 33
	isr 34
	isr 35
	isr 36
	isr 37
	isr 38
	isr 39		
	isr 40
	isr 41
	isr 42
	isr 43
	isr 44
	isr 45
	isr 46
	isr 47
	isr 48
	isr 49		
	isr 50
	isr 51
	isr 52
	isr 53
	isr 54
	isr 55
	isr 56
	isr 57
	isr 58
	isr 59
	isr 60
	isr 61
	isr 62
	isr 63
	isr 64
	isr 65
	isr 66
	isr 67
	isr 68
	isr 69
	isr 70
	isr 71
	isr 72
	isr 73
	isr 74
	isr 75
	isr 76
	isr 77
	isr 78
	isr 79
	isr 80
	isr 81
	isr 82
	isr 83
	isr 84
	isr 85
	isr 86
	isr 87
	isr 88
	isr 89
	isr 90
	isr 91
	isr 92
	isr 93
	isr 94
	isr 95
	isr 96
	isr 97
	isr 98
	isr 99		
.END
