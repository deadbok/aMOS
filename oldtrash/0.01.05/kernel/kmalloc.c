/*		kmalloc.c

		Malloc & free function used in kernel space

	History:

		Version 0.01	04 June 2000	16:06		ObliVion
		Initial version.
*/

#include <types.h>
#include <kernel/mm.h>
#include <kernel/kernel.h>

void *kmalloc(size_t size)
{	
	return(alloc_block(size, KERNEL_MEM));
}

void *kfree(void *ptr)
{
	if (!dealloc_block(ptr))
	{
		return(NULL);
	}
	return(ptr);
}
