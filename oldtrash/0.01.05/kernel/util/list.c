/*		list.c

		A linked-list implementation

	History:

		Version 0.01.00	05 July 2000	13:26		ObliVion
		Initial version.
*/
#include <util/list.h>
#include <types.h>

struct list_node *list_back(struct list_node *node, unsigned long i);
struct list_node *list_forward(struct list_node *node, unsigned long i);

int list_create(struct list_container list, struct list_node *head)
{
	if (list.state!=LIST_INUSE)
	{
		list.head=head;
		list.tail=head;
	
		list.state=LIST_INUSE;
		return(0);
	}
	return(1);
}

int list_add(struct list_container list, struct list_node *pos, struct list_node *node)
{
	if (pos)
	{
		node->next=pos->next;

		pos->next=node;
		node->prev=pos;

		list.nodes++;

		if (pos==list.head)
			list.head=node;

		if (pos==list.tail)
			list.tail=node;

		return(0);
	}
	return(1);
}

int list_add_head(struct list_container list, struct list_node *node)
{
	return(list_add(list, list.head, node));
}

int list_add_tail(struct list_container list, struct list_node *node)
{
	return(list_add(list, list.tail, node));
}

int list_remove(struct list_container list, struct list_node *node)
{
	if (node)
	{
		if (node->prev)
		{
			node->prev->next=node->next;
			if (node->next)
				node->next->prev=node->prev;
			else
				return(1);
		}
		else
			if (node->next)
				node->next->prev=node->prev;
			else
				return(1);

		list.nodes--;
		node->prev=NULL;
		node->next=NULL;

		return(0);
	}
	return(1);
}

int list_remove_head(struct list_container list)
{
	return(list_remove(list, list.head));
}

int list_remove_tail(struct list_container list)
{
	return(list_remove(list, list.tail));
}

struct list_node *get_head(struct list_container list)
{
	return(list.head);
}


struct list_node *get_tail(struct list_container list)
{
	return(list.tail);
}

struct list_node *get_index(struct list_container list, unsigned long i)
{
	unsigned long	mid = list.nodes>>1;

	return(list_forward(list.head, i));
}

struct list_node *list_back(struct list_node *node, unsigned long i)
{
	if (i>0)
		if (node->prev)
			return(list_back(node->prev, i--));
		else
			return(NULL);
	else
		return(node);
}

struct list_node *list_forward(struct list_node *node, unsigned long i)
{
	if (i>0)
		if (node->next)
			return(list_forward(node->next, i--));
		else
			return(NULL);
	else
		return(node);
}
