/*		list.h

		Header for a linked-list implementation

	History:

		Version 0.01.00	05 July 2000	13:26		ObliVion
		Initial version.
*/
#ifndef	_LIST_H_
#define _LIST_H_

#ifdef _KERNEL_

enum list_state
{
	LIST_FREE = 0,
	LIST_INUSE,
	LIST_BAD
};

struct list_node
{
	struct list_node	*prev, *next;
	void				*data;
};

struct list_container
{
	struct list_node	*head, *tail;
	unsigned long		nodes;
	enum list_state		state;
};

extern int				list_create(struct list_container list, struct list_node *head);
extern int 				list_add(struct list_container list, struct list_node *pos, struct list_node *node);
extern int				list_add_head(struct list_container list, struct list_node *node);
extern int				list_add_tail(struct list_container list, struct list_node *node);
extern int				list_remove(struct list_container list, struct list_node *node);
extern int				list_remove_head(struct list_container list);
extern int 				list_remove_tail(struct list_container list);
extern struct list_node	*get_head(struct list_container list);
extern struct list_node	*get_tail(struct list_container list);
extern struct list_node	*get_index(struct list_container list, unsigned long i);

#endif	/* _KERNEL_ */
#endif	*/ _LIST_H_ */