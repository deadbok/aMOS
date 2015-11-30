/*		main.c

		The main C code for the aMOS kernel

	History:
		Version 0.01.10	10 Sep 2000	14:07		ObliVion
		Initial version.

		Version 0.01.01	17 May 2000	17:45		ObliVion
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
/*!	\mainpage The aMOS source
	\author ObliVion

	\section intro Introduction
	This is the source level documentation for the aMOS Operating System.

    \section debug About debugging the code
    There is only very limited debugging support in the kernel at the moment.
    The only thing that is possible is to print information to the screen
    skipping the IPC system. That is, no matter what, if the code is executing
    the message will be printed on screen.

    \section IPC About interprocess communication
    Interprocess communication is a conerstone in aMOS. Modules outside the
	kernel have no other way of executing kernel-code (or system calls).
	Every entity in the system should export a mailbox through wich all
	outside communication is carried out.
*/

#include <stdio.h>
#include <stddef.h>
#include <kernel/mm.h>
#include <kernel/setup.h>
#include <kernel/int.h>
#include <kernel/video.h>
#include <kernel/mailbox.h>
#include <kernel/schedule.h>
#include <kernel/video.h>
#include <kernel/pit.h>
#include <kernel/tss.h>
#include <hik/version.h>
#include <kernel/ports.h>
#include <kernel/fd.h>
#include <kernel/pmm.h>
#include <kernel/cpu.h>

int main(void)
{
	init_pmm();

	if (init_mm())
		return(1);

	vid_init();

	vid_set_attr(FG_COLOR, RED);

	printf("\naMOS BOSS V.%s %s\n", STRING_VERSION, BUILD_ARCH);
	printf("Build %s %s by %s on %s\n\n", COMPILE_DATE, COMPILE_TIME, COMPILE_BY, COMPILE_HOST);

	vid_set_attr(FG_COLOR, WHITE);

	init_pit();

	init_tss();

	if (init_intr())
		return(1);

	init_cpu();

	if (init_sched())
		return(1);

	init_fd();

	printf("System memory: %d Mb\n\n", (int)pmm_get_size_mb());
	printf("Free kernel mem: %d bytes\nUsed kernel mem: %d bytes\n", (int)get_free_mem(), (int)get_used_mem());

	for (;;) {}
		
	return(0);
}

