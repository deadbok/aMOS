/*		int.h

		Micro kernel side interrupt system

	History:
		Version 0.01.01	22 Jul 2001	00:34		ObliVion
		Initial version.

*/
/*! \file kernel/int.h
	\brief Header for the kernel implementation of the interrupt system
	
	This is architecture independent part of the interrupt system
*/
#ifndef _HIK_INT_PUBLIC_H_
#define _HIK_INT_PUBLIC_H_

#include <kernel/mailbox.h>

typedef	unsigned char	isr_t;

#define HIK_INT		0
#define	MSG_INT		1

/*!	\brief Message telling that somebody want to hook an interrupt.*/
#define	IMSG_HOOK	1

/*!	\brief This is the data of the message to hook an interrupt.*/
struct IMSG_HOOK_DATA
{
	/*! The function that is invoked when the interrupt is fired.*/
	rcv_func_t		receiver;
	/*! The interrupt number that is to be hooked.*/
	int				int_nr;
	/*! The message type id to send to the receiver when an interrupt occurs.*/
	msg_type_t		msg_type;
};

/*!	\brief Contain the data for a receiver of an interrupt through a mailbox.*/
struct intr_msg_handler
{
	/*! Define wether this is an internal HIK interrupt that works through direct handler call or
	a interrupt that is to be send using a mailbox.*/
	isr_t	type;
	/*! The function that is invoked when the interrupt is fired.*/
	rcv_func_t		receiver;
	/*! The message type id to send to the receiver when an interrupt occurs.*/	
	msg_type_t		msg_type;
};

extern mailbox_id_t		intr_mailbox;

#endif


