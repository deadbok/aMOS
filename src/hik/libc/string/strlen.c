/*		strlen.c

		Standard libc function to determine the length of a zero terminated string

	History:

		Version 0.01	02 July 2000	19:32		ObliVion
		Initial version.
*/
#include <string.h>

size_t strlen(char *p)
{
	char	*tmp_ptr = p;
	int		x = 0;
                     
	if (!tmp_ptr)
	{
		return(0);
	}            
                     
	while (*tmp_ptr!='\0')
	{
		tmp_ptr++;
		x++;
	}
	
	return(x);   
}
