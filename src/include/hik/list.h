/*		list.h

		Include file for the aMOS mikro-kernel linked list
		implementation

	History:

		Version 0.01.01	07 Apr 2001	19:55		ObliVion
		Initial version.

*/

/*! \file hik/list.h
    \brief Header for list.c
*/

#ifndef _LIST_H_
#define _LIST_H_

#include <types.h>

/* \brief Type definition for the user supplied compare function of the list */
typedef int (*list_compare_t)(void *first, void *second);

/*!	\brief This structure contain all information about the list.
*/
struct list_root
{
	/*! This is the head of the associated list */
	struct list_node		*head;
	/*! This is the tail of the associated list */
	struct list_node		*tail;
	/*! Tell how many nodes the list spans */
	size_t					n_nodes;
	/*! If this flag is 0 the list is unsorted */
	char					ordered;
	/*! Pointer to the function used to compare nodes */
	list_compare_t			compare;
};

/*!	\brief This is the structure of every node in any list */
struct list_node
{
	/*! This is a pointer to the actual data stored in the node */
	void				*data;
	/*! This point to the previous node in the list */
	struct list_node	*prev;
	/*! This point to the next node in the list */
	struct list_node	*next;
};

#endif

