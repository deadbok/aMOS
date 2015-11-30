/*		pic.h

		Routines for the 8259 PIC

	History:

		Version 0.01	18 May 2000	15:13		ObliVion
		Initial version.
*/

#ifndef _HIK_PIC_H
#define _HIK_PIC_H

/*#define	DEBUG*/
#undef	DEBUG

#define M_PIC		0x20     /* I/O for master PIC              */
#define M_IMR		0x21     /* I/O for master IMR              */
#define S_PIC		0xA0     /* I/O for slave PIC               */
#define S_IMR		0xA1     /* I/O for slace IMR               */

#define EOI			0x20     /* EOI command                     */
#define SPEC_EOI	0x60     /* EOI command                     */

#define ICW1		0x11	/* 4: 1=This is an ICW1				*/
							/* 3: 0=Edge triggered Interrupts	*/
							/* 2: 0=Call Address interval of 8	*/
							/* 1: 0=The PIC is cascaded			*/
							/* 0: 1=Need ICW4					*/
#define M_ICW3		0x04	/* IRQ2 connected to slave			*/
#define S_ICW3		0x02	/* IRQ2 comes from master			*/
#define ICW4		0x01	/* 8086/8080 mode                   */

#define M_VEC		0x20	/* Vector for master               */
#define S_VEC		0x28	/* Vector for slave                */

#define OCW3_IRR	0x0A	/* Read IRR                        */
#define OCW3_ISR	0x0B	/* Read ISR                        */

extern int				init_pic(void);
extern void				unmask_irq(unsigned short irq_no);
extern void				mask_irq(unsigned short irq_no);
extern unsigned short 	get_irq_mask(void);
extern void 			set_irq_mask(unsigned short mask);


#endif
