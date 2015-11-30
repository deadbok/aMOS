/*		intsys.c

		The interrupt sub-system of the kernel (nothing cool yet)

	History:

		Version 0.01.00	01 july 2000	21:18		ObliVion
		Initial version.
*/
#include <string.h>
#include <kernel/kernel.h>
#include <i386/setup.h>
#include <i386/intsys.h>
#include <i386/halt.h>

int_handler_ptr		intlist[MAX_INTS];

int init_intr(void)
{
	print_message("Interrupt system...\n");
	enter_section();

	memset(intlist, 0, MAX_INTS);

	leave_section();
	print_message("");

	return(0);

}

void handle_int(int intr)
{
	if (intlist[intr])
		(*intlist[intr])();
	else
		kprintf("\nUnhandled Interrupt %x\n", intr);
	iret();
}

void do_opcode_except(int intr, unsigned long ret)
{
	kprintf("Invalid Opcode Execption (Int 6)\n");
	kprintf("%x\n", ret);
	loop_halt();
}

void install_isr(int intr, int_handler_ptr isr)
{
	if (intr<MAX_INTS)
	{
		intlist[intr]=isr;
	}
}