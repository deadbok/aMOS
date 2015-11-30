/*		gdt.c

		The GDT stuff

	History:

		Version 0.01	27 June 2000	12:35		ObliVion
		Initial version.
*/
#include <string.h>
#include <i386/gdt.h>
#include <i386/setup.h>

unsigned long long		gdt[GDT_SIZE];
struct x86_dt_ptr		gdtptr;

void set_desc(unsigned short index, unsigned long base, unsigned long limit, unsigned long flags);
void set_tss_desc(unsigned short selector, unsigned long base, unsigned long limit, unsigned char type);


void init_gdt(void)
{
	memset(gdt, 0, GDT_SIZE*8);

	set_desc(KERNEL_CODE_INDEX, 0x0, 0xfffff, D_32BIT+D_4K_LIM+D_CODE+D_PRESENT+D_READ);
	set_desc(KERNEL_DATA_INDEX, 0x0, 0xfffff, D_32BIT+D_4K_LIM+D_DATA+D_PRESENT+D_WRITE);

	gdtptr.limit=(GDT_SIZE*8)-1;
	gdtptr.address=gdt;
}

void set_desc(unsigned short index, unsigned long base, unsigned long limit, unsigned long flags)
{
	if (index<NON_TSS_ENTRIES)
	{
		gdt[index]=(limit & 0xffff);
		gdt[index]|=((base & 0xffff) << 16);
		gdt[index]|=((unsigned long long)(base >> 16) & 0xff) << 32;
		gdt[index]|=((unsigned long long)(limit>>16) & 0xf) << 48;
		gdt[index]|=((unsigned long long)(base >> 24) & 0xff) << 56;
		gdt[index]|=((unsigned long long)flags << 32);
	}
}

void set_tss_desc(unsigned short index, unsigned long base, unsigned long limit, unsigned char type)
{
	if ((index>NON_TSS_ENTRIES) && (index<GDT_SIZE))
	{
/*		gdt[index].tss_desc.limit0=limit & 0xffff;;
		gdt[index].tss_desc.base0=base & 0xffff;
		gdt[index].tss_desc.base16=(base >> 16) & 0xff;
		gdt[index].tss_desc.type=type;
		gdt[index].tss_desc.limit16=(limit >> 16) & 0xff;
		gdt[index].tss_desc.base24=(base >> 24) & 0xff;*/
	}
}

