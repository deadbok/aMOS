/*		halt.c

		Functions to stop the system on error

	History:

		Version 0.01	17 July 2000	15:05		ObliVion
		Initial version.
*/
#include <kernel/halt.h>

void loop_halt(void)
{
	char	i;

	for(;;)
	{
		i++;
		i--;
	}
}

