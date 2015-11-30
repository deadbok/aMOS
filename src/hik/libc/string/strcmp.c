/*		strcmp.c

   			Strcmp function for libc

        History:

           Version 0.02.00	10 Aug 2001		22:51		ObliVion
           Returns the prober sign.

           Version 0.01.00   15 Jul 2000		19:01		ObliVion
           Initial version.
*/
/*! \file strcmp.c
    \brief Standard libc strcmp function
*/

#include <string.h>

/*!	\brief Compare the two strings.
	\param first_string The first of the strings to compare.
	\param second_strin The second of the strings to caompare.
	\return <ul><li>negative if the second string is the largest.
				<li>zero if the strings are equal.
				<li>positive if the first string is the largest.
			</ul>
*/

int strcmp( char* first_string, char* second_string)
{
	char	*s1 = first_string;
	char	*s2 = second_string;

	/* An infinite loop :) */
	while (1)
	{
		/* If this is the end of the first string */
		if (!*(s1))
		{
			if (!*(s2))
            	return(0);		/* Then if this is the end the second string, they are equal (0)*/
			else
				return(-1);		/* Else the second string is longer than the first and hence "larger" (-1)*/
		}

		/* If this is the end of the second string and NOT the first
		   The second string is shortest and therefore "smallest" (1) */
		if (!*s2)
			return(1);

		/* If the current character of the first string has the lesser ASCII number */
		if (*s1<*s2)
          return(-1); /* Then the first string is the "smallest" (-1)*/
		else
			if (*s1>*s2)
  		        return(1); /* If the current character of the second string has the lesser
							  ASCII number of the two then the first string is the "largest" (1)*/

		/*The characters must be equal. Proceed to the next ones */
		s1++;
		s2++;
	}

	return(5);
}
