/*		kernel.h

		Defines and ... header for the aMOS kernel

	History:

		Version 0.01	02 July 2000	20:04		ObliVion
		Initial version.
*/
#ifndef KERNEL_H
#define KERNEL_H

#include <stdarg.h>
#include <types.h>

#define TRUE			1
#define FALSE			0

/*kprintf.c*/
extern int kprintf(const char *format,...);
extern int print(const char *format,va_list va);

/*coollook.c*/
extern void print_startup_header(void);
extern void enter_section(void);
extern void leave_section(void);
extern void print_message(char *format, ...);
extern void print_status(int critical, int error);

/*kmalloc.c*/
extern void *kmalloc(size_t size);
extern void *kfree(void *ptr);

#endif