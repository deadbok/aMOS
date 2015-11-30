/*		pic.c

		Routines for the 8259 PIC

	History:

		Version 0.01.00	18 May 2000	15:13		ObliVion
		Initial version.
*/
#include <stdio.h>
#include <kernel/ports.h>
#include <kernel/pic.h>

#ifdef DEBUG
	#include <arch/debug.h>
#endif

int init_pic(void)
{
	printf("Remapping IRQ's...\n");

							/* See 8259.h for a description of the	*/
							/* bits that we fiddle with in the ICW	*/

	outb(M_PIC, ICW1);		/* Send initialization command to PIC 1	*/
	outb(S_PIC, ICW1);		/* Send initialization command to PIC 2	*/

	outb(M_PIC+1, M_VEC);    /* Base interrupt vector        */
	outb(S_PIC+1, S_VEC);

	outb(M_PIC+1, M_ICW3);		/* Tell PIC that IRQ2 is cascaded */
	outb(S_PIC+1, S_ICW3);		

	outb(M_PIC+1, ICW4);     /* Finish 8259 initialization   */
	outb(S_PIC+1, ICW4);

	outb(M_PIC+1, 0xfb);	/* Disable all but cascade */
	outb(S_PIC+1, 0xff);
		
	return(0);
}

void unmask_irq(unsigned short irq_no)
{
	unsigned char		mask;

	if (irq_no < S_VEC)
	{
		mask=inb(M_PIC+1);
		mask&= ~(1 << (irq_no - M_VEC));
		outb(M_PIC+1, mask);
	}
	else
	{
		mask=inb(S_PIC+1);
		mask&= ~(1 << (irq_no - S_VEC));
		outb(S_PIC+1, mask);
	}
}

void mask_irq(unsigned short irq_no)
{
	unsigned char		mask;

	if (irq_no < S_VEC)
	{
		mask=inb(M_PIC+1);
		mask&= (1 << (irq_no - M_VEC));
		outb(M_PIC+1, mask);
	}
	else
	{
		mask=inb(S_PIC+1);
		mask&= (1 << (irq_no - S_VEC));
		outb(S_PIC+1, mask);
	}
}

unsigned short get_irq_mask(void)
{
	unsigned short	mask;

	mask = inb(M_PIC+1);
	mask += inb(S_PIC+1) << 8;

	return(mask);
}

void set_irq_mask(unsigned short mask)
{
	outb(M_PIC+1, (unsigned char)(mask & 0x00ff));
	outb(S_PIC+1, (unsigned char)((mask & 0xff00) >> 8));
}
