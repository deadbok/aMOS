/*		int.h

		The architecture dependant interrupt sub-system of the kernel (nothing cool yet)

	History:

		Version 0.01.00	01 July 2000	21:18		ObliVion
		Initial version.
*/
#ifndef _HIK_INT_H_
#define _HIK_INT_H_

#include <hik/int.h>

/*	\brief a typedef of an interrupt handler */
typedef void (*isr_func_t)(unsigned long intr);

/*!	\brief Contain the data for a receiver of an interrupt through direct function call.*/
struct intr_hik_handler
{
	/*! Define wether this is an internal HIK interrupt that works through direct handler call or
	a interrupt that is to be send using a mailbox.*/
	isr_t			type;
	/*! The function that is invoked when the interrupt is fired.*/
	isr_func_t		handler;
};

/*!	\brief Enable interrupts on the x86 platform */
#define int_enable()		__asm__ __volatile__ ("sti" ::: "memory")

/*!	\brief Disable interrupts on the x86 platform */
#define int_disable()		__asm__ __volatile__ ("cli" ::: "memory")

extern int	init_intr(void);
extern void handle_intr(unsigned long intr);
extern void intr_install_hik_handler(unsigned long int_nr, isr_func_t handler);

#endif
