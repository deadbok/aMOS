/*		memcmp.c

   			Memcmp function for libc

        History:

           Version 0.01.01   25 Nov 2001		03:01		ObliVion
           Initial version.

        Comments:
           This could easily be implemented in assembler assuring
           fast code but this is portable!!
           The code that I've seen gcc produces with -O3 is not
           that impressive so maybe after all we should do this
           in asm. Or maybe is should take a couple of more
           glances at the code when I understand asm better.
*/
/*! \file memcmp.c
    \brief Standard libc memcmp function
*/

#include <string.h>

/*!	\brief Compares n bytes of the memory areas p1 & p2.
	\param p1 A pointer to the first memory area to compare. 
	\param p2 A pointer to the second memory area to compare.
	\param n Number of bytes to compare.
	\return The same sign as the difference between the first
	differing pair of bytes (zero if equal).
*/
int memcmp(const void* p1, const void* p2, size_t n)
{
	const unsigned char	*ch1 = p1;
	const unsigned char	*ch2 = p2;
	
	while (n--)
	{  
		/* If the bytes are not equal*/
		if (*ch1++ != *ch2++)
				return(*--ch1 - *--ch2);		/* Return difference*/
		
	}

	return(0);		/* Return no difference*/
}

