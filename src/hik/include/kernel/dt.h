/*		dt.h

		General Descriptor Table functions

	History:

		Version 0.01	11 Sep 2000	19:50		ObliVion
		Initial version.
*/
#include <kernel/setup.h>

#ifndef _HIK_DT_H_
#define _HIK_DT_H_

#define D_DATA				0x1000  /* Data segment       */
#define D_CODE				0x1800  /* Code segment       */
#define D_TSS				0x0900	/* TSS */
#define D_INT				0x0f00  /* 386 interrupt gate */
#define D_LDT				0x0200	/* Local desriptor tabel */

#define D_EXPAND_DOWN		0x400
#define D_CONFORMING		0x400

#define D_BUSY				0x200

#define D_READ				0x200
#define D_WRITE				0x200

#define D_DPL3				0x6000   /* DPL3 or mask for DPL                    */
#define D_DPL2				0x4000   /* DPL2 or mask for DPL                    */
#define D_DPL1				0x2000   /* DPL1 or mask for DPL                    */
#define D_PRESENT			0x8000   /* Present                                 */

#define D_32BIT				0x400000	  /* Default to 32 bit mode           */
#define D_4K_LIM			0x800000	  /* Limit is in 4K units             */

struct dt_ptr
{
	unsigned short 			limit;
	unsigned long long		*address;
} __attribute__ ((packed));

extern unsigned long long		gdt[GDT_SIZE];

extern void	set_desc(unsigned long long *desc, unsigned long base, unsigned long limit, unsigned long flags);


#endif

