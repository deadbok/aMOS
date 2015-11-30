/*		halt.h

		Header for halt functions

	History:

		Version 0.01	17 July 2000	15:05		ObliVion
		Initial version.
*/
#ifndef _HIK_HALT_H_
#define _HIK_HALT_H_

#define	halt()		__asm__ __volatile__("hlt": : :"memory")
#define	safe_halt()	__asm__ __volatile__("sti; hlt": : :"memory")

extern void loop_halt(void);

#endif

