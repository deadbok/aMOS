/*		8259.h

		Routines for the 8259 PIC

	History:

		Version 0.01	18 May 2000	15:13		ObliVion
		Initial version.
*/
/*
GazOS Operating System
Copyright (C) 1999  Gareth Owen <gaz@athene.co.uk>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef KERNEL_8259_H
#define KERNEL_8259_H

#define M_PIC  0x20     /* I/O for master PIC              */
#define M_IMR  0x21     /* I/O for master IMR              */
#define S_PIC  0xA0     /* I/O for slave PIC               */
#define S_IMR  0xA1     /* I/O for slace IMR               */

#define EOI    0x20     /* EOI command                     */

#define ICW1   0x11     /* 4: 1=This is an ICW1				*/
						/* 3: 0=Edge triggered Interrupts	*/
						/* 2: 0=Call Address interval of 8	*/
						/* 1: 0=The PIC is cascaded			*/
						/* 0: 1=Need ICW4					*/
#define M_ICW3   0x04	/* IRQ2 connected to slave			*/
#define S_ICW3   0x02	/* IRQ2 comes from master			*/
#define ICW4   0x01     /* 8086/8080 mode                   */

#define M_VEC  0x20     /* Vector for master               */
#define S_VEC  0x28     /* Vector for slave                */

#define OCW3_IRR  0x0A  /* Read IRR                        */
#define OCW3_ISR  0x0B  /* Read ISR                        */

int init_8259(void);
void enable_irq(unsigned short irq_no);
void disable_irq(unsigned short irq_no);


#endif
