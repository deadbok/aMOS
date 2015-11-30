/*		cpu.S

		Routines to detect and init CPU's compatbile with the I386

	History:

		Version 0.01	20 May 2000	00:42		ObliVion
		Initial version.
*/

#define ASM_FILE
#include <i386/cpu.h>

/*
 *
 *
 */

.global has_cpuid
.align 4

has_cpuid:
				pushl	%ebp
				movl	%esp, %ebp

				pushfd