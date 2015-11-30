/*		mailbox.c

		The mailbox code for communicating with (and within) the micro-kernel

	History:
		Version 0.01.02	16 Jul 2000	00:59		ObliVion
		Fixed a bug in the send function. Now local copies of message and data are used

		Version 0.01.01	10 Sept 2000	13:15		ObliVion
		Initial version.
*/
/*! \file mailbox.c
	\brief This is the code for the message system of the kernel.

	This is the heart of the IPC provided by the kernel. It is
	implemented using mailboxes.
*/

#include <stdio.h>
#include <string.h>
#include <kernel/list.h>
#include <kernel/mailbox.h>

/*!	It is OK to enter the dispatcher when this is one */
int dispatch_ok = 1;

struct mailbox *get_mb(mailbox_id_t name);

/*! The list of mailboxes */
struct list_root	*mailboxes		= NULL;
/*! The next mailbox to go through the dispatcher */
struct list_node	*dispatch_mailbox = NULL;
/*!	The last mailbox returned by \ref get_mb, used for optimization */
struct mailbox		*last_mailbox = NULL;


int mailbox_compare(void *first, void *second);
/*!	\brief The compare function for the mailbox list.

	This function compare the names of to mailboxes. It is used for sorting and searching
	and replaces the standard \ref list_compare_default, for the mailbox list.
	\param first A pointer to a \ref mailbox struct.
	\param second A pointer to a \ref mailbox struct.
 	\return <ul><li>negative if the first string is the largest.
				<li>zero if the strings are equal.
				<li>positive if the second string is the largest.
			</ul>
	\internal
*/
int mailbox_compare(void *first, void *second)
{
	return(strcmp(((struct mailbox *)first)->name, ((struct mailbox *)second)->name));
}

/*!	\brief Creates a new mailbox.

	This function creates a new mailbox and inserts it in the message system.
	\param name The name of the mailbox.
	\param receiver A function pointer to the receiver function of the mailbox
 	\return MAILBOX_OK if everthing is OK
*/
int create_mb(mailbox_id_t name, rcv_func_t receiver)
{
	struct list_node		*mb_node	= NULL;
	struct mailbox			*mb			= NULL;

	dispatch_ok = 0;

	/* Initialize the list of mailboxes if it has not been done allready */
	if (!mailboxes)
		mailboxes = create_list(mailbox_compare);

	/* Allocate memory for the mailbox */
	mb = (struct mailbox *)malloc(sizeof(struct mailbox));

	/* Save the name of the mailbox */
	mb->name = (char *)malloc(strlen(name));
	strcpy(mb->name, name);

	/* Create a list for the messages */
	mb->messages = create_list(NULL);
	/* Set the receiver function */
	mb->receiver= receiver;

	/* Create a node for the mailbox and associate the mailbox with it */
	mb_node = new_node();
	(struct mailbox *)mb_node->data = mb;

	/* Add the mailbox to the list */
/*	add_node_sorted(mailboxes, mb_node);*/
	add_node(mailboxes, mb_node);
	if (!mailboxes->head)
	{
		dispatch_ok = 1;
		return(MAILBOX_CANT_ADD);
	}

	dispatch_ok = 1;

	return(MAILBOX_OK);
}

/*!	\brief Find a mailbox by its name

	\param name The name of the mailbox to be found.
 	\return NULL of a pointer to the mailbox with the given name
	\internal
*/
struct mailbox *get_mb(mailbox_id_t name)
{
	struct list_node		*mb_node;
	struct mailbox			*mb;

	/* Allocate memory for the mailbox */
	mb=(struct mailbox *)malloc(sizeof(struct mailbox));

	/* Save the name of the mailbox */
	mb->name = (char *)malloc(strlen(name));
	strcpy(mb->name, name);

	if (mailbox_compare(mb, last_mailbox)!=0) /* Check if the mailbox is the same as the last for faster access */
	{
		mb_node = find_node(mailboxes, mb);

		if (mb_node)
			last_mailbox = mb_node->data;
		else
		{
			return(NULL);
		}
	}

	return(last_mailbox);
}

/*!	\brief Sends a message to a mailbox

	\param name The name of the receiver mailbox
	\param message The message to send
 	\return MAILBOX_OK if everything went well
*/
int send(mailbox_id_t name, struct msg *message)
{
	struct mailbox		*mb = NULL ;
	struct list_node	*msg_node = NULL;
	struct msg			*temp_msg;

	dispatch_ok = 0;

	if (!message)
	{
		dispatch_ok = 1;

		return(MAILBOX_EMPTY_MESSAGE);
	}

	mb=get_mb(name);

	if (!mb)
	{
		dispatch_ok = 1;

		return(MAILBOX_DONT_EXIST);
	}

	msg_node = new_node();

	if (!msg_node)
	{
		dispatch_ok = 1;

		return(MAILBOX_DONT_EXIST);
	}

	/*shouldn't need to do this*/
	msg_node->next=NULL;

/*	This is against the description of the Message System
	but it is used until a better solution is possible*/
	temp_msg = (struct msg *)malloc(sizeof(struct msg));
	memcpy(temp_msg, message, sizeof(struct msg));

	temp_msg->data = (void *)malloc(message->data_size);
	memcpy(temp_msg->data, message->data, message->data_size);

	temp_msg->sender = (char *)malloc(strlen(message->sender));
	strcpy(temp_msg->sender, message->sender);

	msg_node->data = temp_msg;

	add_node(mb->messages, msg_node);

	if (!mb->messages)
	{
            dispatch_ok = 1;
			return(MAILBOX_DONT_EXIST);
	}

	dispatch_ok = 1;
	return(MAILBOX_OK);
}

/*!	\brief Function called regularly from the scheduler to dispatch messages
 	\return 0
	\internal
*/
int message_dispatch(void)
{
	struct mailbox		*mb;
	struct list_node	*msg_node;
	struct msg			*message;

	if ((!dispatch_ok) || (!mailboxes->head))
		return(0);

	if (!dispatch_mailbox)
		dispatch_mailbox = mailboxes->head;

	mb = (struct mailbox *)dispatch_mailbox->data;

	if (mb->messages->head)
	{
		msg_node = (struct list_node *)mb->messages->head;

		message = (struct msg *)msg_node->data;

		mb->receiver(message);

		delete_node(mb->messages, msg_node);
	}

	while (!((struct mailbox *)dispatch_mailbox->data)->messages->head)
	{
		if (dispatch_mailbox->next)
			dispatch_mailbox = dispatch_mailbox->next;
		else
		{
			dispatch_mailbox = NULL;
			return(0);
		}
	}

	return(0);
}

/*!	\brief Function to dispatch all waiting messages at once
 	\return 0
	\internal
*/
int message_dispatch_all(void)
{
	struct list_node	*mb_node;
	struct mailbox		*mb;
	struct list_node	*msg_node;
	struct msg			*message;

	mb_node = mailboxes->head;

	while(mb_node)
	{
		mb = (struct mailbox *)mb_node->data;

		while (mb->messages->head)
		{
			msg_node = (struct list_node *)mb->messages->head;

			message = (struct msg *)msg_node->data;

			mb->receiver(message);

			delete_node(mb->messages, msg_node);
		}
		mb_node = mb_node->next;
	}

	return(0);
}
