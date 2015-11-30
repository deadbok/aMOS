/*		task.h

		Task manegment code

	History:

		Version 0.01.01	10 Sep 2000	17:03		ObliVion
		Initial version.
*/
#ifndef _TASK_H_
#define _TASK_H_

#include <hik/mailbox.h>

struct tcb
{
	struct tcb		*next;
	void			(*entry)();
	char			priority;
	struct msg		message;
};

#endif


