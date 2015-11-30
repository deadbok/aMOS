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

int strcmp(const char* s1, const char* s2)
{
	while(*s1==*s2)
	{
		if(!*s1++)
			return(0);
		s2++;
   }
   return(1);
}
