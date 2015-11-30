/*		main.c

		The main C code for the aMOS kernel

	History:

		Version 0.01	17 May 2000	17:45		ObliVion
		Initial version.
*/

/*
	They keep showing this comercial on the telly
	"this dream feels like ours"
	It's one of those comercials that depends on "harmony"
	The mood is wonderfully expressed
	But the point is...
	There is this girl
	And there is this 1 second shot of her face
	As she wakes up her boyfriend
	She has the most beautiful expression in her face
	I've never seen anything quite as beautiful
	In a comercial
	And all it does my dear sunflower
	Is push the knife in a little bit more
	'Cos you were that beautiful most of the time
	That's one of the reasons I still love you
	When you've seen what you think is perfection
	You don't just let it slip away
*/

#include <kernel/kernel.h>
#include <kernel/mm.h>
#include <i386/8259.h>
#include <i386/cpu.h>
#include <i386/setup.h>
#include <i386/intsys.h>

int main(void) 
{
	unsigned long		i;

	if (preinit_console())
		return(0);

	print_startup_header();
	print_message("Init...\n");

	print_status(1, init_8259());

	print_status(1, init_intr());

	print_status(1, init_mm());

	print_status(1, init_cpu());

	for(;;)
	{
		i++;
		i--;
	}

	return(0);
}
