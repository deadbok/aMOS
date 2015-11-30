/*        memcpy.c

          Memcpy function for libc

     History:

          Version 0.01   15 July 2000   19:01          ObliVion
          Initial version.

     Comments:
		This could easily be implemented in assembler assuring
		fast code but this is portable!!
		The code that I've seen gcc produces with -O3 is not
		that impressive so maybe after all we should do this
		in asm. Or maybe is should take a couple of more
		glances at the code when I understand asm better.


*/
#include <string.h>

void *memcpy(void *dest, void *src, size_t count)
{
	size_t		left = count;
	char		*d = dest;
	char		*s = src;	
     
	for (; left>0; left--)
		*d++=*s++;

	return(dest);
}
