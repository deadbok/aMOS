/*		debug.h

		Debugging functions for the kernel

	History:
		Version 0.01.02	29 April 2001	01:07		ObliVion
		Initial version.
*/

#ifndef _HIK_DEBUG_H_
#define _HIK_DEBUG_H_

#include <stdarg.h>

/*#define DEBUG_WAIT*/
#define DEBUG_WAIT_LOOPS	50000

extern int debug_printf(const char *format,...);
extern void debug_halt(void);
extern void init_debug(void);

#define	DEBUG_ENTRY(FUNCTION)	debug_printf("Entering %s\n", FUNCTION)
#define DEBUG_EXIT(FUNCTION)	debug_printf("leaving %s\n", FUNCTION)

#endif

