/*		kprintf.c

		Implementation of a minimal printf function in kernel space

	Features
		These arguments are implented:
			%s %d %x
		Every unknown argument is printed

	History:

		Version 0.01	02 July 2000	16:53		ObliVion
		Initial version.
*/

#include <kernel/kernel.h>
#include <i386/setup.h>
#include <console/console.h>
#include <stdarg.h>

/*#define isdigit(C) (((C>='0') && (C<=9)) ? 1 : 0)*/

void int_to_str(int i, char *str, int base);

int kprintf(const char *format,...)
{
    va_list         va;

    va_start(va, format);
	return(print(format, va));
}

int print(const char *format, va_list va)
{

	unsigned long	i = 0;
	int				arg_processed = 0;
	char			*str_ptr;
	char			fixed_str_ptr[200];
	int				int_parm;

	while(*format)
	{
		if ((*format)=='%')
		{
			format++;
			while(!arg_processed)
			{
				switch(*format)
				{
					case 'd':	int_parm=va_arg(va, int);
								
								int_to_str(int_parm, fixed_str_ptr, 10);

								while (fixed_str_ptr[i]!='\0')
							    	console_putc(fixed_str_ptr[i++]);

								arg_processed=1;
								format++;
								i=0;
								break;

					case 's':	str_ptr=va_arg(va, char *);
								while (str_ptr[i]!='\0')
							    	console_putc(str_ptr[i++]);
								arg_processed=1;
								format++;
								i=0;
								break;

					case 'x':	int_parm=va_arg(va, int);
								
								fixed_str_ptr[0]='0';
								fixed_str_ptr[1]='x';

								int_to_str(int_parm, &fixed_str_ptr[2], 0x10);

								while (fixed_str_ptr[i]!='\0')
							    	console_putc(fixed_str_ptr[i++]);

								arg_processed=1;
								format++;
								i=0;
								break;

					default: 	arg_processed=1;
								format++;
								break;
				}
			}
			arg_processed=0;
		}
		else
		{
			console_putc((*format));
			format++;
		}
	}

    return 0;
}

/*
 *		void int_to_str(int i, char *str, int base)
 *		Converts an int to a string by of "base"
 *  
 *  
 *  	i:	roadrunner
 */

void int_to_str(int i, char *str, int base)
{
	int				temp_int;
	char			temp_str[200];
	unsigned long	j=0, k=0;
	unsigned long	end=0;

	if (i==0)
	{
		str[0]='0';
		str[1]='\0';
	}
	else
	{
		while (i>0)
		{
			temp_int=i%base;

			if (temp_int<10)
				temp_str[end++]=temp_int+'0';
			else
				temp_str[end++]=temp_int-10+'a';
		
			i/=base;
		}
	
		/*	The string resulting from the previous processing is 
			reversed so we fix that... */
		j=end;
		while (j>0)
		    str[k++]=temp_str[--j];

		str[k]='\0';
	}
}

