/*		mailbox.h

		The mailbox code for communication with (and within) the micro-kernel

	History:

		Version 0.01	17 May 2000	17:45		ObliVion
		Initial version.
*/
/*! \file hik/include/kernel/mailbox.h
	\brief Header for the mailbox/IP communication system
*/
#include <stddef.h>
#include <kernel/list.h>
#include <hik/mailbox.h>
#include <hik/task.h>

#ifndef _HIK_MAILBOX_H_
#define _HIK_MAILBOX_H_

/*! \brief return value if addition of another mailbox fails */
#define	MAILBOX_CANT_ADD		0
/*! \brief return value if everything went OK */
#define	MAILBOX_OK				1
/*! \brief return value if an empty message was given as a parameter */
#define MAILBOX_EMPTY_MESSAGE	2
/*! \brief return value if a mailbox that do not exist was addressed */
#define MAILBOX_DONT_EXIST		3

extern int create_mb(mailbox_id_t name, rcv_func_t receiver);
extern int send(mailbox_id_t name, struct msg *message);
extern int message_dispatch(void);
extern int message_dispatch_all(void);

#endif

