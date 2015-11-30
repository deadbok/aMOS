/*		gdt.h

		The GDT stuff

	History:

		Version 0.01	27 june 2000	12:35		ObliVion
		Initial version.
*/
#ifndef _HIK_GDT_H_
#define _HIK_GDT_H_

#include <kernel/selector.h>

#define K_TSS				0x4000

__inline__ void lgdt(struct dt_ptr *pdt);

extern	void init_gdt(void);

#endif /* _KERNEL_GDT_H_ */

