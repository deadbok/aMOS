/*		list.c

		Source file for the aMOS mikro-kernel linked list
		implementation

	History:
		Version 0.01.01	07 Apr 2001	19:55		ObliVion
		Initial version.

	-	In every data object there should be pointers to function which
		new and delete the data. These functions are then supplied bu the
		code usihng the linked list. Here we just call them blindly hoping
		that they will do the right thing.
*/
/*! \file list.c
	\brief Implementation of a singly list.

	This code is a general implementation of a linked list. By isolating
	this we make sure that we have only one source of error when it comes
	to linked lists
*/

#include <string.h>
#include <kernel/list.h>

void 				list_swap_nodes(struct list_node *first, struct list_node *second);
int 				list_compare_default(void *first, void *second);
void 				list_sort(struct list_root *list, struct list_node *head, struct list_node *tail);

/*!	\brief Call this function to create a list.

	Allocate memory for the first node, fill in intitial information in the \ref list_root struct.
	\parm compare_func A pointer to a function that compare the data the list holds.
	\return A pointer to the \ref list_root struct of the node.
*/
struct list_root *create_list(list_compare_t compare_func)
{
	struct list_root		*list;

	list = (struct list_root *)malloc(sizeof(struct list_root));
	
	list->head = NULL;
	list->tail = NULL;

	list->n_nodes = 0;

	if (!compare_func)
		list->compare = list_compare_default;
	else
		list->compare = compare_func;

	list->ordered = 1;

	return(list);
}

/*!	\brief Allocate and initialize a new node.

	This function allocate space for and initialize a new node
	\return The address of the new node. 
*/
struct list_node *new_node(void)
{
	struct list_node	*temp_node;

#ifdef DEBUG
	debug_printf("DEBUG: list.c:new_node\n");
#endif

	temp_node = (struct list_node *)malloc(sizeof(struct list_node));

	temp_node->data = NULL;

    temp_node->next = NULL;
	temp_node->prev = NULL;

#ifdef DEBUG
	debug_printf("DEBUG: list.c:new_node returns 0x%x\n", (unsigned int)temp_node);
#endif

	return(temp_node);
}

/*!	\brief Swap two nodes in the list.

	This function swap the two nodes given.
	\param first The first of the two nodes to be swapped.
	\param second The second of the two nodes to be swapped.
*/
void list_swap_nodes(struct list_node *first, struct list_node *second)
{
	struct list_node 	*first_prev = first->next; /*	Save the pointer to the node before the first */
	struct list_node 	*first_next = first->next; /*	Save the pointer to the node after the first */

	first->prev->next = second;
	first->next->prev = second; /*	Make the nodes sorrounding the first node point to the second node */

	first->prev = second->prev;
	first->next = second->next; /*	Make the first node point to the nodes sorrounding the second */

	second->prev->next = first;
	second->next->prev = first;	/*	Make the nodes sorrounding the second node point to the first.
									The first node is now in place */
	second->prev = first_prev;
	second->next = first_next; /*	Make the second node point to the nodes sorrounding the first.
									The second node is now in place */
}

/*!	\brief The default compare function for the list.

	This function compare the data of two nodes. It is used for sorting and searching.
	This is a stub function, it always returns 0 (e.g the nodes are equal). This function
	must be supplied by the user to enable sort and search facilities.
	\param first A pointer to the first data used for comparison.
	\param second A pointer to the second data used for comparison.
 	\return <ul><li>negative if the first string is the largest.
				<li>zero if the strings are equal.
				<li>positive if the second string is the largest.
			</ul>
	\internal
*/
int list_compare_default(void *first, void *second)
{
	return(0);
}

/*!	\brief Sort the list.

	This function takes care of sorting the list using the compare function
	of the given list. The algorithm used for sorting is the standard quick-
	sort.
	\param list The \ref list_root of the list
	\param head The first node of the list
	\param tail The lst node of the list
	\todo Replace this slooow bubble-sort with something better.
	\internal
*/
void list_sort(struct list_root *list, struct list_node *head, struct list_node *tail)
{
	struct list_node	*temp_node = head;
	int					sorted =  1;

	if (list->ordered)
		return;

	do
	{
		sorted = 1;
		
		while (temp_node!=tail)
		{
			if (list->compare(temp_node->data, temp_node->next->data)!=0)
			{
				list_swap_nodes(temp_node, temp_node->next);
				sorted = 0;
			}
			temp_node = temp_node->next;
		}
		
		temp_node = head;		
		
	} while (!sorted);
	
	list->ordered = 1;
}

/*!	\brief Add a node to the requested list

	This function add a node to the tail of the list, thereby causing
	an unsorted list. If a function is called, that require a sorted list,
	the \ref list_sort function is executed.
	\param list The \ref list_root struct created by \ref create_list
    \param new_node A node allocated by \ref new_node
    \return A pointer to the node added
*/
struct list_node *add_node(struct list_root *list, struct list_node *new_node)
{
	if (!new_node)
	{
#ifdef DEBUG		
	debug_printf("DEBUG: list.c:add_node NULL node no action taken\n");
#endif
						
		return(NULL);
	}

	list->ordered = 0;
	list->n_nodes += 1;

	new_node->next = NULL;

	if (list->tail) /*If list exist */
	{
		new_node->prev = list->tail;
		list->tail->next = new_node;
		list->tail = new_node;
    }
    else /* If first node */
	{
		new_node->prev = NULL;

		list->head = new_node;
		list->tail = new_node;
	}

#ifdef DEBUG		
	debug_printf("DEBUG: list.c:add_node added 0x%x new root 0x%x\n", (unsigned int)new_node, (unsigned int)list->head);
#endif

	return(new_node);
}

/*!	\brief Add a node to the list while keeping it ordered.
	
	This function add the node to the list and determine the position
	by using the user supplied compare function in \ref list_root. If the
	list is scrambled when this function is called, the list will be sorted
	before insertion.
	\param list The \ref list_root struct returned by \ref create_list.
	\param new_node The node to add to the list.
	\return The address of the node added.
	\note The reason the list is sorted before insertion is that we can use a
	binary search, to find the place of the new node. This has yet to be coded.
	\note There is no protection against inserting the same node twice.
*/
struct list_node *add_node_sorted(struct list_root *list, struct list_node *new_node)
{
	struct list_node	*temp_node		= list->head;

#ifdef DEBUG
	debug_printf("DEBUG: list.c:add_node_sorted\n");
#endif
	if (list->head) /*	If the list is not empty */
	{
		/*	If the list is scrambled sort it */
/*		if (!list->ordered)
			list_sort(list, list->head, list->tail);*/

		while ((list->compare(temp_node->data, new_node->data) < 0) && (temp_node))
		{
			temp_node = temp_node->next;
		}

		temp_node->prev->next = new_node;
		new_node->prev = temp_node->prev;
		temp_node->prev = new_node;

		list->n_nodes += 1;

		return(new_node);
	}
	else	/* If this is the first node */
	{
		temp_node = add_node(list, new_node);
		list->ordered = 1;

		return(temp_node);
	}

}

/*!	\brief Delete a node from the requested list

	This function deletes a node from the list in \ref list_root. It unlinks the
	node from the list deallocate the space used by \e both data and the node itself.
	\param list The \ref list_root struct created by \ref create_list
    \param node A node allocated by \ref new_node
    \return A pointer to the node added
*/
struct list_node *delete_node(struct list_root *list, struct list_node *node)
{

#ifdef DEBUG
	debug_printf("DEBUG: list.c:delete_node\n");
#endif

	if (list->n_nodes > 0)
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;

		if (node == list->head)
			list->head = node->next;

		if (node == list->tail)
			list->tail = node->prev;

		free(node->data);
		free(node);

		list->n_nodes -= 1;
	}
	
	return(node);
}

/*!	\brief Return the first node of the list
	
	\param list The \ref list_root struct returned by \ref create_list.
	\return The first node in the list.
*/
struct list_node	*get_first_node(struct list_root *list)
{
	return(list->head);
}

/*!	\brief Return the last node of the list
	
	\param list The \ref list_root struct returned by \ref create_list.
	\return The last node in the list.
*/
struct list_node	*get_last_node(struct list_root *list)
{
	return(list->tail);
}

/*!	\brief Look up the node containing the given data
	
	This function uses the compare function in \ref list_root to find
	a node which contain the data.  If the list is scrambled when this
	function is called, the list will be sorted	before insertion.
	\param list The \ref list_root struct returned by \ref create_list.
	\param data A pointer to the data to be found in the list.
	\return The node which contain the data or NULL if unsuccesful.
*/
struct list_node *find_node(struct list_root *list, void *data)
{
	struct list_node	*temp_node		= list->head;

#ifdef DEBUG
	debug_printf("DEBUG: list.c:find_node\n");
#endif

	/*	If the list is scrambled sort it */
/*	if (!list->ordered)
		list_sort(list, list->head, list->tail);*/

	while (temp_node)
	{
		if (list->compare(temp_node->data, data) == 0)
			return(temp_node);

		temp_node = temp_node->next;
	}

	return(NULL);
}

/*!	\brief Destroy a linked list
	
	This function deletes the list, and deallocate memory used by both node and \e data.
	\param list The \ref list_root struct returned by \ref create_list.
	\return NULL if succesful.
*/
struct list_root *destroy_list(struct list_root *list)
{
	struct list_node	*temp_node;

	temp_node = list->tail;

	while (temp_node)
	{
		if (temp_node->data)
			free(temp_node->data);

		/* Free the node that comes after this one if any */
		if (temp_node->next)
			free(temp_node->next);

		temp_node = temp_node->prev;
	}

   free(list);

   return(list);
}

