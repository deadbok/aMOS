/*		8259.c

		Routines for the 8259 PIC

	History:

		Version 0.01	18 May 2000	15:13		ObliVion
		Initial version.
*/

#include <i386/8259.h>
#include <i386/ports.h>
#include <kernel/kernel.h>

unsigned int irq_mask = 0xffff;

int init_8259(void)
{
	enter_section();
	print_message("Remapping IRQ's");

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

	outb(M_PIC+1, irq_mask & 0xff);
	outb(S_PIC+1, (irq_mask >> 8) & 0xff);

	return(0);
}

void enable_irq(unsigned short irq_no)
{
	irq_mask &= ~(1 << irq_no);
	if(irq_no >= 8)
		irq_mask &= ~(1 << 2);
	
	outb(M_PIC+1, irq_mask & 0xff);
	outb(S_PIC+1, (irq_mask >> 8) & 0xff);
}

/* disables irq irq_no */
void disable_irq(unsigned short irq_no)
{
	irq_mask |= (1 << irq_no);
	if((irq_mask & 0xff00)==0xff00)
		irq_mask |= (1 << 2);
	
	outb(M_PIC+1, irq_mask & 0xff);
	outb(S_PIC+1, (irq_mask >> 8) & 0xff);
}
