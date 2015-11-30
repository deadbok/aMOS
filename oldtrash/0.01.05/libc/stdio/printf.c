/*		printf.c

		Implementation of the printf function

	History:

		Version 0.01	02 July 2000	16:53		ObliVion
		Initial version.
*/

#include <stdio.h>
#include <string.h>
#include <console/console.h>

int printf(const char *format,...)
{
    va_list         va;
	unsigned long	i = 0;

    va_start(va, format);

	while(*format)
	{
		if ((*format)=='%')
		{
			i++;

			switch(*format)
			{
				default: break;
			}		
		}
		else
			console_putc((*format));
		
		format++;
	}

    return 0;
}
