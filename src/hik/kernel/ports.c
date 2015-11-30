/*		port.c

		Routines for port I/O

	History:

		Version 0.01	18 May 2000	15:13		ObliVion
		Initial version.
*/
#include <kernel/ports.h>

__inline__ unsigned char inb(unsigned int port)
{
   unsigned char ret;

   __asm__ volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
   return ret;
}

__inline__ void outb(unsigned int port, unsigned char value)
{
   __asm__ volatile ("outb %%al,%%dx"::"d" (port), "a" (value));
}

__inline__ unsigned char inw(unsigned int port)
{
   unsigned char ret;

   __asm__ volatile ("inw %%dx,%%ax":"=a" (ret):"d" (port));
   return ret;
}

__inline__ void outw(unsigned int port, unsigned int value)
{
   __asm__ volatile ("outw %%ax,%%dx"::"d" (port), "a" (value));
}
