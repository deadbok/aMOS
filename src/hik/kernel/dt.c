/*		dt.c

		Misc Descriptor Table functions

	History:

		Version 0.01.01	06 Oct 2000 04:15		ObliVion
		set_desc() has moved here.

		Version 0.01.00	11 Sep 2000	19:50		ObliVion
		Initial version.
*/
#include <kernel/dt.h>

void set_desc(unsigned long long *desc, unsigned long base, unsigned long limit, unsigned long flags)
{
	*desc|=(limit & 0xffff);
	*desc|=((base & 0xffff) << 16);
	*desc|=((unsigned long long)(base >> 16) & 0xff) << 32;
	*desc|=((unsigned long long)(limit>>16) & 0xf) << 48;
	*desc|=((unsigned long long)(base >> 24) & 0xff) << 56;
	*desc|=((unsigned long long)flags << 32);
}
