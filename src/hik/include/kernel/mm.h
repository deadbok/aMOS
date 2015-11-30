/*		mm.h

		Header file for the Memory Manager

	History:
		Version 0.02.01	08 Oct 2000		19:09		ObliVion
		Simplified the whole damn thing, as this only used by the kernel.

		Version 0.01.01	03 July 2000	22:22		ObliVion
		Initial version.
*/
#ifndef _HIK_MM_H
#define _HIK_MM_H

/*#define	DEBUG*/
#undef DEBUG

#define MM_USED		0
#define MM_FREE		1

#define MEM_BLOCK_SIZE  64

struct mem_info
{
	unsigned long			free_blocks;
	unsigned long			used_blocks;
	unsigned long			free_bytes;
	unsigned long			used_bytes;
	unsigned long			free_node_bytes;
	unsigned long			used_node_bytes;
};

struct mem_block_node
{
	struct mem_block_node	*next;
	struct mem_block_node	*prev;
	void					*mem;
	unsigned long			size;
	unsigned long			end_block;
	unsigned char			status;
} __attribute__ ((packed));

extern int init_mm(void);
extern void *alloc_block(unsigned long size);
extern int dealloc_block(void *addr);
extern unsigned long get_block_size(void *mem);
extern unsigned long get_free_mem(void);
extern unsigned long get_used_mem(void);


#endif

