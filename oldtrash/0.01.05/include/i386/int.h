/*		int.h

		The interrupt sub-system of the kernel (nothing cool yet)

	History:

		Version 0.01.00	01 july 2000	21:18		ObliVion
		Initial version.
*/
#ifndef _KERNEL_INT_H_
#define _KERNEL_INT_H_

#define	MAX_INTS			256

typedef void				(*int_handler_ptr)(void);

#endif