/*        malloc.c

          Malloc function for libc

     History:

          Version 0.01   02 Apr 2001   23:05          ObliVion
          Initial version.

*/

#include <string.h>
#include <stddef.h>
#include <kernel/mm.h>

void *malloc(size_t size)
{
	return(alloc_block(size));
}

