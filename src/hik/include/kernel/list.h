/*		list.h

		Include file for the aMOS mikro-kernel linked list
		implementation

	History:

		Version 0.01.01	07 Apr 2001	19:55		ObliVion
		Initial version.

*/

/*! \file hik/include/kernel/list.h
    \brief Header for list.c
*/

#ifndef _HIK_LIST_H_
#define _HIK_LIST_H_

#include <types.h>
#include <hik/list.h>

/*#define	DEBUG*/
#undef	DEBUG

extern	struct list_root	*create_list(list_compare_t compare_func);
extern	struct list_node	*new_node(void);
extern	struct list_node	*add_node(struct list_root *list, struct list_node *new_node);
extern	struct list_node	*add_node_sorted(struct list_root *list, struct list_node *new_node);
extern	struct list_node	*delete_node(struct list_root *list, struct list_node *new_node);
extern	struct list_node	*get_first_node(struct list_root *list);
extern	struct list_node	*get_last_node(struct list_root *list);
extern	struct list_node	*find_node(struct list_root *list, void *data);
extern	struct list_root	*destroy_list(struct list_root *list);

#endif

