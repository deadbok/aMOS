/*		console.c

		The main console driver interface

	History:

		Version 0.01	31 May 2000	14:16		ObliVion
		Initial version.
*/

#include <types.h>
#include <kernel/kernel.h>
#include <console/console.h>
#include <console/vga_cons.h>

unsigned char			nr_consoles = 0;
/*struct list_container	consoles;*/
struct console_dev		*consoles = NULL;
struct console_dev		*current_console = NULL;

void console_init(void)
{
	if (current_console)
		current_console->init();
} 

int add_console(struct console_dev	*console)
{
	struct console_dev		*temp_console;
		
	if (consoles)
	{
		temp_console=consoles;
				
		while (temp_console->next)
			temp_console=temp_console->next;
						
		console->next=NULL;
		temp_console->next=console;
				
		nr_consoles++;
	}
	else
	{
		console->next=NULL;
		consoles=console;
				
		nr_consoles++;
	}
		
	return(nr_consoles);
}

void console_putc(char ch)
{
	if (current_console)
		current_console->putc(ch);
}

void console_clear(void)
{
	if (current_console)
		current_console->clear();
}

void console_set_attr(int attr, int value)
{
	if (current_console)
		current_console->set_attr(attr, value);
}

int		console_xres(void)
{
	if (current_console)
		return(current_console->get_xres());

	return(-1);
}

int		console_yres(void)
{
	if (current_console)
		return(current_console->get_yres());

	return(-1);
}

void console_close(void)
{
	if (current_console)
		current_console->close();
}

int		console_switch(int cons_dev)
{
	unsigned long			i = cons_dev;
	struct console_dev		*temp_console;
		
	if (consoles)
	{
		temp_console=consoles;
		
		while (i>0)
		{
			if (temp_console->next)
			{
				temp_console=temp_console->next;
				i--;
			}
			else
				return(1);
		}
				
		current_console=temp_console;
		return(0);
	}
	return(1);
}
