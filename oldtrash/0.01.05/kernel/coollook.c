/*		coollook.c

		Functions giving the kernel startup a(n) (un)cool look

	History:

		Version 0.01	03 July 2000	17:09		ObliVion
		Initial version.
*/
#include <string.h>
#include <kernel/kernel.h>
#include <kernel/version.h>
#include <i386/setup.h>
#include <console/console.h>
#include <i386/halt.h>

/*
   Most of these functions could be optimized by memcpy 
   but I have'nt made that function yet
   Same goes for memset and malloc...
   Damn I really need malloc
   Not having malloc cuases a lot of fixed size strings and
   other nasty things...
*/

static int	indent_level = 0;

void print_startup_header(void)
{
	console_set_attr(BG_COLOR, BLACK);
	console_set_attr(FG_COLOR, RED);

	kprintf("aMOS V.%s %s\n", STRING_VERSION, BUILD_ARCH);

	console_set_attr(FG_COLOR, BLUE);

	kprintf("Build %s %s by %s on %s\n\n", COMPILE_DATE, COMPILE_TIME, COMPILE_BY, COMPILE_HOST);

	console_set_attr(FG_COLOR, WHITE);
}

void enter_section(void)
{
	indent_level+=4;
}

void leave_section(void)
{
	indent_level-=4;
}


void print_message(char *format, ...)
{
	va_list		va;
	char		indent_str[152];

	console_set_attr(BG_COLOR, BLACK);
	console_set_attr(FG_COLOR, WHITE);

	if(indent_level)
	{
		memset(indent_str, (int)' ', indent_level);

		indent_str[indent_level]='\0';
		kprintf("%s", indent_str);
	}

    va_start(va, format);
	print(format, va);
}

void print_status(int critical, int error)
{
	if (!error)
	{
		kprintf("...");
		console_set_attr(BG_COLOR, BLACK);
		console_set_attr(FG_COLOR, GREEN);

		kprintf("OK\n");
	}
	else
	{
		kprintf("...");
		console_set_attr(BG_COLOR, BLACK);
		console_set_attr(FG_COLOR, RED);

		kprintf("ERROR!\n");

		if (critical)
		{
			kprintf("System halted!\n");
			loop_halt();
		}
	}
	console_set_attr(BG_COLOR, BLACK);
	console_set_attr(FG_COLOR, WHITE);
}



	
