/*		gdt.h

		The GDT stuff

	History:

		Version 0.01	27 june 2000	12:35		ObliVion
		Initial version.
*/
#ifndef _KERNEL_GDT_H_
#define _KERNEL_GDT_H_

#include <i386/selector.h>

#define D_DATA				0x1000  /* Data segment       */
#define D_CODE				0x1800  /* Code segment       */
#define D_INT				0x0e00  /* 386 interrupt gate */

#define D_EXPAND_DOWN		0x400
#define D_CONFORMING		0x400

#define D_READ				0x200
#define D_WRITE				0x200

#define D_DPL3				0x6000   /* DPL3 or mask for DPL                    */
#define D_DPL2				0x4000   /* DPL2 or mask for DPL                    */
#define D_DPL1				0x2000   /* DPL1 or mask for DPL                    */
#define D_PRESENT			0x8000   /* Present                                 */

#define D_32BIT				0x400000	  /* Default to 32 bit mode           */
#define D_4K_LIM			0x800000	  /* Limit is in 4K units             */

#define GDT_SIZE			8192
#define NON_TSS_ENTRIES		2048
#define TSS_ENTRIES			6144

#define K_TSS				0x4000

struct x86_dt_ptr
{
	unsigned short 			limit;
	unsigned long long		*address;
} __attribute__ ((packed));

struct x86_tss
{
    unsigned long			back;
    unsigned long			esp0;
    unsigned long			ss0;
    unsigned long			esp1;
    unsigned long			ss1;
    unsigned long			esp2;
    unsigned long			ss2;
    unsigned long			cr3;
    unsigned long			eip;
    unsigned long			eflags;
    unsigned long			eax;
    unsigned long			ecx;
    unsigned long			edx;
    unsigned long			ebx;
    unsigned long			esp;
    unsigned long			ebp;
    unsigned long			esi;
    unsigned long			edi;
    unsigned long			es;
    unsigned long			cs;
    unsigned long			ss;
    unsigned long			ds;
    unsigned long			fs;
    unsigned long			gs;
    unsigned long			ldt;
    unsigned long			ioperm;
} __attribute__((packed));

extern	void init_gdt(void);

#endif /* _KERNEL_GDT_H_ */