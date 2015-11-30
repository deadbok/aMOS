/*		schedule.h

		The scheduler

	History:

		Version 0.01.01	6 Okt 2000	00:35		ObliVion
		Initial version.
*/
#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

#include <hik/mailbox.h>

/*#define	DEBUG*/
#undef	DEBUG

/*!	\brief Message telling the scheduler to invoke the schedule() function.*/
#define SMSG_GO		1

extern mailbox_id_t		sched_mailbox;

#endif
