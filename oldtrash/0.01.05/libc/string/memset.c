/*		memset.c

		Memset function for libc

	History:

		Version 0.01	09 July 2000	22:13		ObliVion
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

void *memset(void *dest, int c, size_t count)
{
	char 		*d = dest;
	
/*	for (*d++=c; count>0; count--)*/

	for (; count>0; count--)
		*d++=c;

	return(dest);
}
