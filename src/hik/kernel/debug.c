/*		printf.c

		Implementation of the printf function

	History:

		Version 0.01	02 July 2000	16:53		ObliVion
		Initial version.
*/

#include <stdarg.h>
#include <string.h>
#include <kernel/debug.h>
#include <kernel/video.h>
#include <kernel/halt.h>

void	debug_int_to_str(int i, char *str, int base);
int debug_print(const char *format, va_list va);


void init_debug(void)
{
	vid_init();
}

int debug_printf(const char *format,...)
{
    va_list         va;
	int				ret;
#ifdef DEBUG_WAIT	
	unsigned long	i;
#endif	

    va_start(va, format);
	ret = debug_print(format, va);

#ifdef DEBUG_WAIT
	for (i = 0; i < DEBUG_WAIT_LOOPS; i++)
	{
	}
#endif

	return(ret);
}

int debug_print(const char *format, va_list va)
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
								
								debug_int_to_str(int_parm, fixed_str_ptr, 10);

								for (i=0; fixed_str_ptr[i]>0; i++)
									vid_putch(fixed_str_ptr[i]);

								arg_processed=1;
								format++;
								i=0;
								break;

					case 's':	str_ptr=va_arg(va, char *);
								while (str_ptr[i]!='\0')
							    	vid_putch(str_ptr[i++]);
								arg_processed=1;
								format++;
								i=0;
								break;

					case 'x':	int_parm=va_arg(va, int);
								
								debug_int_to_str(int_parm, &fixed_str_ptr[0], 0x10);

								for (i=0; fixed_str_ptr[i]>0; i++)
									vid_putch(fixed_str_ptr[i]);

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
			vid_putch((*format));				
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

void debug_int_to_str(int i, char *str, int base)
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

void debug_halt(void)
{
    debug_printf("DEBUG: System halted on debug request\n");
    loop_halt();
}

