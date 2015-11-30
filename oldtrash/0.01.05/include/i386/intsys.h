/*		int.h

		The interrupt sub-system of the kernel (nothing cool yet)

	History:

		Version 0.01.00	01 july 2000	21:18		ObliVion
		Initial version.
*/
#ifndef _KERNEL_INTSYS_H_
#define _KERNEL_INTSYS_H_

#define	MAX_INTS			256

typedef void				(*int_handler_ptr)(void);

extern void handle_int(int intr);
extern void install_isr(int intr, int_handler_ptr isr);
extern void do_opcode_except(int intr, unsigned long ret);
extern void default_isr(void);

#define int_enable()		__asm__ __volatile__ ("sti" ::: "memory")
#define int_disable()		__asm__ __volatile__ ("cli" ::: "memory")
#define int10()		__asm__ __volatile__ ("int $0x10" ::: "memory")
#define int6()		__asm__ __volatile__ ("int $0x6" ::: "memory")
#define iret()		__asm__ __volatile__ ("iret" ::: "memory")


#endif