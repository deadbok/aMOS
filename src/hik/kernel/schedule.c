/*		schedule.c

		The scheduler

	History:

		Version 0.01.00	6 Okt 2000	00:35		ObliVion
		Initial version.
*/
/*! \file schedule.c
	\brief This is the microkernel scheduler

	This is the microkernel scheduler
*/
#include <stdio.h>
#include <string.h>
#include <kernel/int.h>
#include <kernel/setup.h>
#include <kernel/schedule.h>

/*!	\brief The name of the scheduler mailbox */
mailbox_id_t			sched_mailbox = "/dev/sched";
/*!	\brief 1 when in the scheduler routine */
int 					in_scheduler = 0;

void sched_rcv_func(struct msg *message);
void schedule(unsigned long unused);

/*!	\brief Initialize the scheduler */
int init_sched(void)
{
	printf("Starting scheduler...\n");

	create_mb(sched_mailbox, sched_rcv_func);

	intr_install_hik_handler(TIMER_INT, schedule);

	message_dispatch_all();
	return(0);
}

/*!	\brief This is the receiver function of the scheduler

	This function processes incoming messages
	\param message The message that has been received
*/
void sched_rcv_func(struct msg *message)
{
	switch (message->type)
	{
		case SMSG_GO:		schedule(0);
							break;			
	}
}

/*!	\brief The scheduler routine */
void schedule(unsigned long unused)
{
	if (in_scheduler)
		return;

	in_scheduler = 1;

	if (message_dispatch())
		printf("Error dispatching messages!\n");
				
	in_scheduler = 0;
}
