/*		mm.c

		Memory Managment

	History:

		Version 0.01	03 July 2000	22:20		ObliVion
		Initial version.

		"Apartment of Computer Science"
*/

/*
	All these expressions, all these people, all their feelings
	All my hopes, all my dreams, all my hurts, all this light
	All energy being it god or evil, all this music, all this MUSIC
	I better make some music
*/
/*
	Aah now the moon is up
*/
/*
	"And if he doesn't kiss you, but it feels like you've been kissed"
*/
/*
	My name is Bondage, James Bondage.
*/
#include <string.h>
#include <kernel/mm.h>
#include <kernel/kernel.h>

#define TEST_PATTERN    "_iseeyou"

/* These symbols define the start and end of the kernel */
extern long main;
extern long end;

unsigned long			physical_mem_size;
mem_block_ptr			free_list = NULL;
mem_block_ptr			alloc_list = NULL;
/*new_mem_block should allways be updated so we know where to put the next block*/
mem_block_ptr			new_block_ptr = (mem_block_ptr) 0x00200000;
/*This holds a list of all allocated memory descripters that are currently unused*/
mem_block_ptr			free_descriptor_list = NULL;

unsigned long   		kernel_start, kernel_pages;

/* Forward decl */
unsigned long 			probe_phys_mem(void);
void 					add_unused_descriptor(mem_block_ptr unused_descripter);
mem_block_ptr			get_unused_descriptor(void);
mem_block_ptr 			alloc_new_descriptor(void);
mem_block_ptr 			add_mem(mem_block_ptr last_block, unsigned long start, unsigned long size, unsigned char loc, unsigned long owner);
int 					init_mem_list(void);
unsigned long 			calc_mem(mem_block_ptr mem_block);
void					dump_mem_list(void);

/*
 *		int init_mm(void)
 *
 *		Initializes the Memory Manager
 *  
 */

int init_mm(void)
{
	print_message("Memory Manager...\n");
	enter_section();

	kernel_start=(unsigned long)&main;
	kernel_pages=ALIGN((((unsigned long)&end)-kernel_start), PAGE_SIZE)/PAGE_SIZE;

	print_message("Physical memory: ");
	physical_mem_size=probe_phys_mem();
	kprintf("%d Mb\n", physical_mem_size/1048576);

	print_message("Creating memory map...\n");
	init_mem_list();

	print_message("Free memory    : %d Mb\n", get_free_mem()/1048576);

	leave_section();
	print_message("");

	return(0);
}

/*
 *		unsigned long probe_phys_mem(void)
 *
 *		Determines and returns the amount of physical memory
 *		This assumes that the kernel size is less than 2MB
 *  
 *  	I: roadrunner
 *  
 */

unsigned long probe_phys_mem(void)
{
    unsigned long		*ptr = (unsigned long *) 0x000ffffc;

/*	ptr+=0x00100000;
	for (*ptr=(unsigned long)TEST_PATTERN; (*ptr==(unsigned long)TEST_PATTERN);	ptr+=0x00100000)
	{
	}*/
	do
	{
		ptr+=0x00100000;
		*ptr=(unsigned long)TEST_PATTERN;
	} while (*ptr==(unsigned long)TEST_PATTERN);

    return((unsigned long) (ptr + 1) - 0x00100000);
}

/*
 *		void add_unused_descriptor(mem_block_ptr unused_descripter)
 *
 *		Adds an unused desriptor to the list of unused desriptors
 *  
 */
	
void add_unused_descriptor(mem_block_ptr unused_descriptor)
{
	mem_block_ptr		mem_block;

	if (free_descriptor_list)
	{
		mem_block=free_descriptor_list;

		while (mem_block->next)
			mem_block=mem_block->next;

		unused_descriptor->prev=mem_block;
		mem_block->next=unused_descriptor;
		unused_descriptor->next=NULL;
	}
	else
	{
		unused_descriptor->prev=NULL;
		unused_descriptor->next=NULL;
		free_descriptor_list=unused_descriptor;
	}
}


/*
 *		mem_block_ptr get_unused_descriptor(void)
 *
 *		Returns a pointer to an unused descriptor, if any
 *  
 */
	
mem_block_ptr get_unused_descriptor(void)
{
	mem_block_ptr		mem_block;

	if (free_descriptor_list)
	{
		mem_block=free_descriptor_list;
		free_descriptor_list=mem_block->next;

		return(mem_block);
	}
	
	return(NULL);
}

/*
 *		mem_block_ptr alloc_new_descriptor(void)
 *
 *		Places a new descriptor at first empty address
 *  
 */
	
mem_block_ptr alloc_new_descriptor(void)
{
	mem_block_ptr		mem_block;

	mem_block=get_unused_descriptor();

	if (!mem_block)
	{
		mem_block=new_block_ptr;
		new_block_ptr+=sizeof(*mem_block);
	}

	return(mem_block);
}

/*
 *		mem_block_ptr add_mem(mem_block_ptr last_block, unsigned long start, unsigned long size, unsigned char loc, unsigned long owner)
 *
 *		Add a memory descriptor after "last_block"
 *  
 */

mem_block_ptr add_mem(mem_block_ptr last_block, unsigned long start, unsigned long size, unsigned char loc, unsigned long owner)
{
	mem_block_ptr		mem_block;

	if (last_block)
	{
		mem_block=alloc_new_descriptor();
		last_block->next=mem_block;

		mem_block->next=NULL;
		mem_block->prev=last_block;

		mem_block->start=start;
		mem_block->size=size;
		mem_block->loc=loc;
		mem_block->owner=owner;
	}
	else
	{
		mem_block=alloc_new_descriptor();
		
		mem_block->next=NULL;
		mem_block->prev=NULL;

		mem_block->start=start;
		mem_block->size=size;
		mem_block->loc=loc;
		mem_block->owner=owner;
	}

	return(mem_block);
}

/*
 *		int init_mem_list(void)
 *
 *		Initializes the list of descriptors containing information about
 *		each block of memory
 *		I will need to re-implement this for portability
 *  
 */

int init_mem_list(void)
{
	mem_block_ptr		mem_block;

	alloc_list=add_mem(alloc_list, 0x00, 0x0500, LOW_MEM, SYSTEM_MEM);

	mem_block=add_mem(alloc_list, 0x0a0000, 0x0c0000-0x0a0000, LOW_MEM, SYSTEM_MEM);

	mem_block=add_mem(mem_block, 0x0c0000, 0x00100000-0x0c0000, LOW_MEM, SYSTEM_MEM);

	mem_block=add_mem(mem_block, 0x00100000, 0x00200000-0x00100000, HIGH_MEM, KERNEL_MEM);

	mem_block=add_mem(mem_block, 0x00200000, 0x00300000-0x00200000, HIGH_MEM, KERNEL_MEM);

	free_list=add_mem(free_list, 0x0500, 0x0a0000-0x0500, LOW_MEM, 0);

	mem_block=add_mem(free_list, 0x00300000, physical_mem_size-0x00300000, HIGH_MEM, 0);

	return(0);
}

/*
 *		void *alloc_block(unsigned long size, unsigned long owner)
 *
 *		allocate "size" bytes of memory at "first fit"
 *  
 */

void *alloc_block(unsigned long size, unsigned long owner)
{
	mem_block_ptr		alloced_block;
	mem_block_ptr		free_block;
	mem_block_ptr		temp_block;

	free_block=free_list;
	
	while ((free_block->size<size) && (free_block->next!=NULL))
		free_block=free_block->next;

	if (free_block->size==size)
	{
		/* unhook node */
		(mem_block_ptr)(free_block->prev->next)=free_block->next;
		(mem_block_ptr)(free_block->next->prev)=free_block->prev;

		alloced_block=free_block;

		/*insert in alloced list*/
		temp_block=alloc_list;
		while ((temp_block->start<alloced_block->start) && (temp_block->next!=NULL))
			temp_block=temp_block->next;

		alloced_block->next=temp_block->next;
		(mem_block_ptr)(alloced_block->next->prev)=alloced_block;
		alloced_block->prev=temp_block;
		temp_block->next=alloced_block;

		alloced_block->owner=owner;

		return((void *)alloced_block->start);
	}

	if (free_block->size>size)
	{
		/*split block*/

		/*create new block for the allocated mem*/
		alloced_block=alloc_new_descriptor();

		/*save the original block so we have it's data handy*/
		memcpy(alloced_block, free_block, sizeof(*free_block));

		/*adjust the empty one*/
		free_block->start=alloced_block->start+size;
		free_block->size=alloced_block->size-size;
		free_block->loc=alloced_block->loc;
		free_block->owner=alloced_block->owner;

		/*insert new block into alloced list*/
		temp_block=alloc_list;

		while ((temp_block->start<alloced_block->start) && (temp_block->next!=NULL))
			temp_block=temp_block->next;

		alloced_block->prev=temp_block->prev;
		(mem_block_ptr)(temp_block->prev->next)=alloced_block;

		alloced_block->next=temp_block;
		(mem_block_ptr)(temp_block->prev)=alloced_block;

		/*...and adjust values*/
		alloced_block->size=size;
		alloced_block->owner=owner;

		return((void *)alloced_block->start);
	}
	
	return(NULL);
}

/*
 *		int dealloc_block(void *addr)
 *
 *		Deallocate block at "addr"
 *  
 */

int dealloc_block(void *addr)
{
	mem_block_ptr		temp_block;
	mem_block_ptr		free_block;

	temp_block=alloc_list;
	
	while ((temp_block->start<(unsigned long)addr) && (temp_block->next!=NULL))
		temp_block=temp_block->next;

	if (temp_block->start!=(unsigned long)addr)
		return(1);

	if (temp_block->start==(unsigned long)addr)
	{
		/* unhook node */
		(mem_block_ptr)(temp_block->prev->next)=temp_block->next;
		(mem_block_ptr)(temp_block->next->prev)=temp_block->prev;

		/* find a free block and add the mem to it (if possible)*/
		free_block=free_list;
		
		while ((free_block->start<(unsigned long)addr) && (free_block->next!=NULL))
			free_block=free_block->next;

		if ((temp_block->start+temp_block->size)==(unsigned long)free_block->start)
		{
			free_block->start=temp_block->start;
			free_block->size+=temp_block->size;

			add_unused_descriptor(temp_block);
		}
		
		return(0);
	}

	
	return(1);
}

unsigned long get_free_mem(void)
{
	return(calc_mem(free_list));
}

unsigned long calc_mem(mem_block_ptr mem_block)
{
	if (mem_block->next)
		return(mem_block->size+calc_mem(mem_block->next));
	else
		return(mem_block->size);
}

void dump_mem_list(void)
{
	mem_block_ptr		mem_block;
	unsigned long		i = 0;
	int					temp_int;

	print_message("Dumping allocated memory regions:\n");

	mem_block=alloc_list;

	do
	{
		print_message(0, "Block: %d",i);

		temp_int=(int)mem_block->start;
		kprintf(" Start: %x",temp_int);

		temp_int=(int)mem_block->size;
		kprintf(" Size: %d",temp_int);

		temp_int=(int)mem_block->owner;
		kprintf(" Owner: %d",temp_int);

		temp_int=(int)mem_block->next;
		kprintf(" Next: %x\n",temp_int);

		i++;
		if (mem_block->next)
			mem_block=mem_block->next;
		else
			i=0;
	} while(i);

	print_message("Dumping free memory regions:\n");

	mem_block=free_list;

	do
	{
		print_message(0, "Block: %d",i);

		temp_int=(int)mem_block->start;
		kprintf(" Start: %x",temp_int);

		temp_int=(int)mem_block->size;
		kprintf(" Size: %d",temp_int);

		temp_int=(int)mem_block->owner;
		kprintf(" Owner: %d",temp_int);

		temp_int=(int)mem_block->next;
		kprintf(" Next: %x\n",temp_int);

		i++;
		if (mem_block->next)
			mem_block=mem_block->next;
		else
			i=0;
	} while(i);

	print_message("That's all folks\n");
}