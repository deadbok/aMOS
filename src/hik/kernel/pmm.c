/*		memory.c

		Physical Memory Management

	History:
		Version 0.01.01	30 Dec 2001	04:00		ObliVion
		Initial version.

*/

#include <string.h>
#include <kernel/pmm.h>
#include <kernel/pic.h>

unsigned long			mem_size_mb = 0;

void init_pmm(void)
{
	register unsigned long	*mem;
	unsigned long			mem_count = 0;
	unsigned long			tmp;
	unsigned short			irq_mask;
	unsigned long			cr0;

	irq_mask = get_irq_mask();

	set_irq_mask(0);

	// store a copy of CR0
	__asm__ __volatile("movl %%cr0, %0" : "=r" (cr0));

	// invalidate the cache
	// write-back and invalidate the cache
	__asm__ __volatile__ ("wbinvd");

	// plug cr0 with just PE/CD/NW
	// cache disable(486+), no-writeback(486+), 32bit mode(386+)
	__asm__ __volatile__("movl %0, %%cr0" : : "r" (cr0 | 0x00000001 | 0x40000000 | 0x20000000));

	do
	{
		mem_size_mb++;
		mem_count += 1024 * 1024;
		mem = (unsigned long *)mem_count;

		tmp = *mem;

		*mem = 0x55AA55AA;

		// the empty asm calls tell gcc not to rely on whats in its registers
		// as saved variables (this gets us around GCC optimisations)
		__asm__ __volatile__("":::"memory");
		if(*mem != 0x55AA55AA)
			mem_count = 0;
		else
		{
			*mem = 0xAA55AA55;
			asm("":::"memory");
			if(*mem != 0xAA55AA55)
				mem_count = 0;
		}

		asm("":::"memory");
		*mem = tmp;
	}	while(mem_size_mb < 4096 && mem_count != 0);

	__asm__ __volatile__("movl %0, %%cr0" :: "r" (cr0) : "eax");

	set_irq_mask(irq_mask);
}

unsigned long	pmm_get_size_mb(void)
{
	return(mem_size_mb);
}

