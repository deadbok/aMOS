/*		8259.h

		Routines for port I/O

	History:

		Version 0.01	18 May 2000	15:13		ObliVion
		Initial version.
*/

#ifndef _HIK_PORTS_H
#define _HIK_PORTS_H

__inline__ unsigned char inb(unsigned int port);
__inline__ void outb(unsigned int port, unsigned char value);
__inline__ unsigned char inw(unsigned int port);
__inline__ void outw(unsigned int port, unsigned int value);

#endif
