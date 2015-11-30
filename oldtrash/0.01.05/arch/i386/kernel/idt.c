/*		idt.c

		The IDT stuff

	History:

		Version 0.01	02 July 2000	14:15		ObliVion
		Initial version.
*/
#include <string.h>
#include <i386/gdt.h>
#include <i386/idt.h>
#include <i386/intsys.h>
#include <i386/setup.h>

unsigned long long		idt[MAX_INTS] __attribute__((__section__(".data.idt")));
struct x86_dt_ptr		idtptr;

void set_int(unsigned short index, unsigned long offset, unsigned short selector, unsigned long flags);

#define SET_DEFAULT_INT(INT, SELECTOR, FLAGS) 	set_int(INT, (unsigned long)default_isr, SELECTOR, FLAGS);

void init_idt(void)
{
	unsigned long		i;	


	memset(idt, 0, MAX_INTS*8);

	for (i=0; i<MAX_INTS; i++)
		SET_DEFAULT_INT(i, KERNEL_CODE_SEL, D_PRESENT+D_INT+D_DPL3);

	idtptr.limit=(MAX_INTS*8)-1;
	idtptr.address=idt;
}

void set_int(unsigned short index, unsigned long offset, unsigned short selector, unsigned long flags)
{
	if (index<NON_TSS_ENTRIES)
	{
		idt[index]=(offset & 0xffff);
		idt[index]|=((selector & 0xffff) << 16);
		idt[index]|=((unsigned long long)(offset >> 16) & 0xffff) << 48;
		idt[index]|=((unsigned long long)flags << 32);
	}
}

