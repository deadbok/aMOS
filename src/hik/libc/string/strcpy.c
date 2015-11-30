/*		strcpy.c

		strcpy function for libc

	History:

		Version 0.01.02		05 Sep 2001		23:31		ObliVion
		Fixed an ugly bug. I was checking if the source pointer
		was '\0' instead the character in the string.

		Version 0.01.01		24 Apr 2001		01:48		ObliVion
		Initial version.

*/
#include <string.h>

char *strcpy(char *dest, const char *src)
{
	char	*d = dest;
	const char	*s = src;

	while(*s!='\0')
		*d++=*s++;

	*d='\0';

	return(dest);
}
