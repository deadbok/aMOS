/*		int.c

		Micro kernel side interrupt system

	History:
		Version 0.01.02	02 Dec 2001	23:12		ObliVion
		The interrupt system can now communicate through mailboxes

		Version 0.01.01	22 Jul 2001	00:34		ObliVion
		Initial version.

*/
/*! \file int.c
	\brief Micro kernel side interrupt system

	This architecture independent part of the interrupt system
	implemented using mailboxes.

	\note The interrupt system is allowed to send messages directly to
	the receiver skipping the regular message queue. This is due to the
	importance of this kind of message, and the fact that the IPC system
	dispatches message using the timer interrupt, and therefore it is
	not possible to send interrupt messages as regular messages, as they
	would never get to the receiver.
*/

/*	12 Sept 2001. Yesterday America was attacked, and the World Trade Center
	crushed to the ground. Thousands of people have died. There is apparently
	no end to the level of stupidity that this world can('t) manage. Please give
	a thought for the sacrificed, they did not deserve this death. Personally
	it makes me wonder if we will survive this century. If we are wise enough
	to walk the surface of this beautiful earth. We should realize that we're
	all to blame. Take a look at the show that has been put on.
*/

#include <string.h>
#include <stdio.h>
#include <kernel/setup.h>
#include <kernel/ports.h>
#include <kernel/pic.h>
#include <kernel/mm.h>
#include <kernel/mailbox.h>
#include <kernel/list.h>
#include <kernel/int.h>

/*!	\brief The name of the interrupt system mailbox */
mailbox_id_t		intr_mailbox = "/dev/intr";
/*!	\brief A list of all interrupt handlers */
struct list_root	*intr_list[MAX_INTS];
/*!	\brief The message telling that an interrupt was fired. */
struct msg			msg_int_fired;

void intr_install_msg_handler(unsigned long intr_nr, rcv_func_t receiver, msg_type_t msg_type);

void intr_rcv_func(struct msg *message);

/*!	\brief This function initialize the interrupt system

	This function initialize the interrupt system and
	create a mailbox for communicating with the interrupt system
 	\return <ul><li>0 if initialization went OK
				<li>>0 if initialization went wrong
			</ul>
*/
int init_intr(void)
{
	unsigned long	i;

	printf("Initializing interrupt system...\n");

	create_mb(intr_mailbox, intr_rcv_func);

	for (i=0; i<MAX_INTS; i++)
		intr_list[i] = create_list(NULL);

	msg_int_fired.type =  0;
	msg_int_fired.data_size = sizeof(unsigned long);
	msg_int_fired.data = (unsigned long *)malloc(sizeof(unsigned long));
	msg_int_fired.sender = intr_mailbox;

	init_pic();
	int_enable();

	return(0);
}

/*!	\brief This is the receiver function of the interrupt system

	This function processes incoming messages
	\param message The message that has been received
	\internal
*/
void intr_rcv_func(struct msg *message)
{
	switch (message->type)
	{
		case IMSG_HOOK:		intr_install_msg_handler(((struct IMSG_HOOK_DATA *)message->data)->int_nr, ((struct IMSG_HOOK_DATA *)message->data)->receiver, ((struct IMSG_HOOK_DATA *)message->data)->msg_type);
							break;
	}
}

/*!	\brief This function installs an internal interrupt handler

	This function is used to install interrupt handlers from within the HIK
	\param intr_nr The number of the interrupt that you want to install
	\param handler A function pointer to a the handler function
*/
void intr_install_hik_handler(unsigned long intr_nr, isr_func_t handler)
{
	/*!	\todo check if the interrupt is out of range */

	struct intr_hik_handler	*new_handler;
	struct list_node		*handler_node;

	handler_node = new_node();
	new_handler = (struct intr_hik_handler *)malloc(sizeof(struct intr_hik_handler));

	new_handler->type = HIK_INT;
	new_handler->handler = handler;

	handler_node->data = new_handler;

	add_node(intr_list[intr_nr], handler_node);

	unmask_irq(intr_nr);

	/*! \todo Only unmask hardware IRQ's */
}

/*!	\brief This function installs a IPC driven interrupt handler

	This function is used to install interrupt handlers from outside the HIK through IPC
	\param intr_nr The number of the interrupt that you want to install
	\param receiver The function to receive a message when the interrupt is fired
	\param msg_type The message type that will be sent to the receiver
	\internal
*/
void intr_install_msg_handler(unsigned long intr_nr, rcv_func_t receiver, msg_type_t msg_type)
{
	/*!	\todo check if the interrupt is out of range */

	struct intr_msg_handler	*new_handler;
	struct list_node		*handler_node;

	handler_node = new_node();
	new_handler = (struct intr_msg_handler *)malloc(sizeof(struct intr_msg_handler));

	new_handler->type = MSG_INT;
	new_handler->receiver = receiver;
	new_handler->msg_type = msg_type;

	handler_node->data = new_handler;

	add_node(intr_list[intr_nr], handler_node);

	unmask_irq(intr_nr);

	/*! \todo Only unmask hardware IRQ's */
}

/*!	\brief This function handles all interrupts

	This function routes the interrupts to the right handlers
	\param intr The number of the interrupt that was fires
	\internal
*/
void handle_intr(unsigned long intr)
{
	struct list_node		*handlers = NULL;

	handlers = intr_list[intr]->head;

	while (handlers)
	{
		/* This includes ugly typecasts of the data in the list
		to struct intr_hik_handler which is not always true, but all
		types of data possible at this pointer should have the type
		field as the first */

		switch(((struct intr_hik_handler *)handlers->data)->type)
		{

			case	HIK_INT:	((struct intr_hik_handler *)(handlers->data))->handler(intr);
								break;

			case	MSG_INT:	msg_int_fired.type = ((struct intr_msg_handler *)handlers->data)->msg_type;
								*((unsigned long *)msg_int_fired.data) = intr;

								((struct intr_msg_handler *)(handlers->data))->receiver(&msg_int_fired);
								break;
		}
		handlers = handlers->next;
	}

	if ((intr>=0x20) && (intr<0x28))
		outb(M_PIC, (intr-0x20)+EOI);

	if ((intr>=0x28) && (intr<0x30))
	{
		outb(S_PIC, (intr-0x20)+EOI);
		outb(M_PIC, 0x02+EOI);
	}
	
}

