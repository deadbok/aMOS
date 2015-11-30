/*		pit.c

		Routines for the 8253/54 PIT

	History:

		Version 0.01	18 May 2000	15:13		ObliVion
		Initial version.
*/
#include <stdio.h>
#include <kernel/pit.h>
#include <kernel/ports.h>

int init_pit(void)
{
	printf("Starting timer...\n");

	outb(PIT_MODE, CNT_LMSB+CNT_MODE_3);
	
	outb(PIT_CNT_0, ((unsigned long)1193182/50) & 0xff);
	outb(PIT_CNT_0, ((unsigned long)1193182/50) >> 8);

	return(0);
}
