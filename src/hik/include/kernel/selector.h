#ifndef _HIK_SELECTOR_H_
#define _HIK_SELECTOR_H_

#define NULLSEL				0x00		/* Null */
#define KERNEL_CODE_SEL		0x08		/* Kernel code */
#define KERNEL_DATA_SEL		0x10		/* Kernel data */
#define KERNEL_TSS_SEL		0x28		/* Kernel TSS */

#define KERNEL_CODE_INDEX	1		/* Kernel code */
#define KERNEL_DATA_INDEX	2		/* Kernel data */
#define KERNEL_TSS_INDEX	5		/* Kernel TSS */

#endif
