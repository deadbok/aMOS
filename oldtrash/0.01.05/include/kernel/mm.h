/*		mm.h

		Header file for the Memory Manager

	History:

		Version 0.01	03 July 2000	22:22		ObliVion
		Initial version.
*/
#ifndef KERNEL_MM_H
#define KERNEL_MM_H


/*
	These values determine the physical location of the current
	Page. Which in turn, helps determine the priority when allocating
	a new page. 1 is best 0xFF is very lousy.
*/
#define NO_MEM		0
#define HIGH_MEM	1
#define LOW_MEM		2
#define DISK_MEM	3
#define UNDEF_MEM	4

/*
	These values determine the ownership of the memory-page
	All values not defined here are for application use
*/
#define SYSTEM_MEM	1
#define KERNEL_MEM	2


#define PAGE_SIZE	4096
#define ALIGN(ADDR, SIZE) ((((ADDR)+(SIZE)-1)/(SIZE))*(SIZE))

struct mem_block_node
{
    struct mem_block_node	*prev, *next;
    unsigned long			start;
    unsigned long			size;
			/* Physical location of the block*/
	unsigned char			loc;
	unsigned long			owner;
} __attribute__ ((packed));

typedef	struct mem_block_node	*mem_block_ptr;


extern int 					init_mm(void);
extern void 				*alloc_block(unsigned long size, unsigned long owner);
extern int 					dealloc_block(void *addr);
extern unsigned long 		get_free_mem(void);

#endif
