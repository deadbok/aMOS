/*		printf.c

		Implementation of the printf function

	History:
		Version 0.01.03	05 Dec	2001	00:59		ObliVion
		Moved this file in to the HIK and removed IPC in favor of
		direct calls to the video driver
	
		Version 0.01.02	16 Jul	2000	00:59		ObliVion
		printf now uses IPC to send the characters to the video driver

		Version 0.01.01	02 Jul	2000	16:53		ObliVion
		Initial version.
*/

#include <stdio.h>
#include <string.h>
#include <kernel/video.h>
#include <kernel/mailbox.h>
#include <kernel/mm.h>

struct msg	str_msg = {VMSG_STRING, 0, NULL, "printf"};
struct msg	ch_msg = {VMSG_CHAR, 1, NULL, "printf"};

void	int_to_str(int i, char *str, int base);
int		print(const char *format, va_list va);

int printf(const char *format,...)
{
	va_list         va;

	va_start(va, format);
	return(print(format, va));
}

int print(const char *format, va_list va)
{
	int				arg_processed = 0;
	char			*str_ptr = NULL;
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

								vid_putstr(fixed_str_ptr);
																				
								arg_processed=1;
								format++;
								break;

					case 's':	str_ptr=va_arg(va, char *);
										
								vid_putstr(str_ptr);
								
								arg_processed=1;
								format++;
								break;

					case 'x':	int_parm=va_arg(va, int);
								
								int_to_str(int_parm, fixed_str_ptr, 0x10);

								vid_putstr(fixed_str_ptr);

								arg_processed=1;
								format++;
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
			vid_putch(*format);

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

