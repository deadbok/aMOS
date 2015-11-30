/*        free.c

          Free function for libc

     History:

          Version 0.01   02 Apr 2001   23:11          ObliVion
          Initial version.

*/

#include <stddef.h>
#include <string.h>
#include <kernel/mm.h>

void free(void *addr)
{
	dealloc_block(addr);
}

