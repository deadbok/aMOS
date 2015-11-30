/*		mailbox.h

		The mailbox code for communication with (and within) the micro-kernel

	History:

		Version 0.01.01		17 May 2000	17:45		ObliVion
		Initial version.
*/
/*! \file hik/mailbox.h
	\brief Header for the mailbox/IP communication system
*/
#include <stddef.h>
#include <hik/list.h>
#include <hik/task.h>

#ifndef _MAILBOX_H_
#define _MAILBOX_H_

struct msg;
struct mailbox;

/*! The type of the ID's of all mailboxes*/
typedef char			*mailbox_id_t;
/*! The type of the type identifier of a message*/
typedef unsigned long	msg_type_t;
/*! Definition of the receive function needed to handle messages */
typedef	void			(*rcv_func_t)(struct msg *message);

/*! \brief Structure of a message.

	This structure is what is exchanged by the sender and receiver, when
	communicating.
*/
struct msg
{
	/*! The type of the data in the message */
	msg_type_t			type;
	/*! The size of the data in the message */
	size_t				data_size;
	/*! A pointer to the data */
	void				*data;
	/*! A the mailbox of the sender */
	mailbox_id_t		sender;
};

/*! \brief Structure of a mailbox.

	This structure hold all data needed to manage a the mailbox.
*/
struct mailbox
{
	/*! The name and path of the mailbox */
	mailbox_id_t		name;
	/*! The messages waiting to arrive */
	struct list_root	*messages;
	/*! Pointer to the function used to receive the messages */
	rcv_func_t			receiver;
	/*! The priority of the mailbox (not implemented)*/
	unsigned long		priority;
};

#endif

