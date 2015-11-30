/*		setup.h

		Architecture dependant setup functions structures...

	History:

		Version 0.01	23 July 2000	10:07		ObliVion
		Initial version.
*/
#ifndef _HIK_SETUP_H_
#define _HIK_SETUP_H_

#ifndef ASM_FILE

#include <boot/boot.h>
#include <limits.h>

/*extern int	arch_setup(void);
extern int	install_sched_func(void *func);
extern void	init_gdt(void);
extern void debug_init(void);
extern void	dump_mem(unsigned char *ptr, unsigned long bytes);*/

#endif /* ASM_FILE */

#define KERNEL_STACK		0x1fffff
#define KSTACK_SIZE			8192
#define GDT_SIZE			8192
#define MAX_INTS			256
#define NON_TSS_ENTRIES		2048
#define TSS_ENTRIES			6144
#define KERNEL_MEM_POOL		256
#define MEM_LIST_ENTRIES	65535
#define TIMER_INT			0x20

#endif
