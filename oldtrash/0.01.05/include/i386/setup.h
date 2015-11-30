/*		setup.h

		Architecture dependant setup functions structures...

	History:

		Version 0.01	23 July 2000	10:07		ObliVion
		Initial version.
*/
#ifndef _KERNEL_SETUP_H_
#define _KERNEL_SETUP_H_

#ifndef ASM_FILE

extern int	preinit_console(void);
extern void	init_gdt(void);
extern int	init_intr(void);

#endif /* ASM_FILE */

#define KERNEL_STACK		0x1fffff

#endif
