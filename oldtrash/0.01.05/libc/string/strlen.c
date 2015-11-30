/*		strlen.c

		Standard libc function to determine the length of a zero terminated string

	History:

		Version 0.01	02 July 2000	19:32		ObliVion
		Initial version.
*/
#include <string.h>

int strlen(const char *p)
{  
	int		x = 0;
                     
	if (!p)
	{
		return(0);
	}            
                     
	while (*p++) 
		x++;
	
	return(x);   
}                    
