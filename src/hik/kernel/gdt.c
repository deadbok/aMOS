/*		gdt.c

		The GDT stuff

	History:

		Version 0.01	27 June 2000	12:35		ObliVion
		Initial version.
*/
#include <string.h>
#include <kernel/dt.h>
#include <kernel/gdt.h>
#include <kernel/tss.h>
#include <kernel/setup.h>

unsigned long long		gdt[GDT_SIZE];
struct dt_ptr			gdtptr;

void init_gdt(void)
{
	memset(gdt, 0, GDT_SIZE*8);

	set_desc(&gdt[KERNEL_CODE_INDEX], 0x0, 0xfffff, D_32BIT+D_4K_LIM+D_CODE+D_PRESENT+D_READ);
	set_desc(&gdt[KERNEL_DATA_INDEX], 0x0, 0xfffff, D_32BIT+D_4K_LIM+D_DATA+D_PRESENT+D_WRITE);

	set_desc(&gdt[KERNEL_TSS_INDEX], (unsigned long)&kernel_tss, 0x68 /*sizeof(struct tss)*/, D_TSS+D_PRESENT+D_4K_LIM);
		
	gdtptr.limit=(GDT_SIZE*8)-1;
	gdtptr.address=gdt;
}

__inline__ void lgdt(struct dt_ptr *pdt)
{
	__asm__ volatile("lgdt %0" : : "m" (pdt->limit));
}

