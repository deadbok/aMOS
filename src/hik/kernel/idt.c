/*		idt.c

		The IDT stuff

	History:

		Version 0.01	02 July 2000	14:15		ObliVion
		Initial version.
*/
/*! \file idt.c
	\brief Function forInterrupt Descriptor Tables on the x86

	This sets up the ID's for all interrupts, which is needed
	when running in 32-bit protected mode.
*/
#include <string.h>
#include <kernel/dt.h>
#include <kernel/idt.h>
#include <kernel/selector.h>
#include <kernel/setup.h>

/*!	\brief An array of Interrupt Descriptors */
unsigned long long		idt[MAX_INTS] __attribute__((__section__(".data.idt")));
/*!	\brief A pointer to the array, in the format needed by \ref lidt */
struct dt_ptr			idtptr;

void set_int(unsigned short index, unsigned long offset, unsigned short selector, unsigned long flags);

/*!	\brief Define that makes adding an entry to the ID array even easier */
#define SET_DEFAULT_INT(INT, SELECTOR, FLAGS) 	set_int(INT, (unsigned long)_intr##INT, SELECTOR, FLAGS);

/*!	\brief Intitialization of the IDT

 	This function initalizes the Interrupt Descriptor Tables
	for the x86. This enables the full protected mode interrupt
	capabilities of the x86 processors*/
void init_idt(void)
{
	memset(idt, 0, MAX_INTS*8);

	SET_DEFAULT_INT(0, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(1, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(2, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(3, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(4, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(5, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(6, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(7, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(8, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(9, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(10, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(11, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(12, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(13, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(14, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(15, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(16, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(17, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(18, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(19, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(20, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(21, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(22, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(23, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(24, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(25, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(26, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(27, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(28, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(29, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(30, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(31, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(32, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(33, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(34, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(35, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(36, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(37, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(38, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(39, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(40, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(41, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(42, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(43, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(44, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(45, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(46, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(47, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(48, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(49, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(50, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(51, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(52, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(53, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(54, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(55, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(56, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(57, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(58, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(59, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(60, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(61, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(62, KERNEL_CODE_SEL, D_PRESENT+D_INT);
	SET_DEFAULT_INT(63, KERNEL_CODE_SEL, D_PRESENT+D_INT);

	idtptr.limit=(MAX_INTS*8)-1;
	idtptr.address=(unsigned long long *)idt;
}

/*!	\brief Loads the Interrupts Desciptor Table
	\param pdt A Descriptor Table pointer*/
__inline__ void lidt(struct dt_ptr *pdt)
{
	__asm volatile("lidt %0" : : "m" (pdt->limit));
}

/*!	\brief Function to add an entry to the ID array */
void set_int(unsigned short index, unsigned long offset, unsigned short selector, unsigned long flags)
{
/*	idt[2*index]=(offset & 0x0000ffff) | ((selector & 0xffff) << 16);
	idt[2*index+1]|=(offset & 0xffff0000) | (flags & 0x0000ffff);*/
	idt[index]=offset & 0x0000ffff;
	idt[index]|=(unsigned long long)selector << 16;
	idt[index]|=(unsigned long long)flags << 32;
	idt[index]|=(unsigned long long)(offset & 0xffff0000) << 32;
}

