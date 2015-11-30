/*		mm.c

		Memory Managment

	History:
		Version 0.02.01	08 Oct 2000		19:09		ObliVion
		Simplified the whole damn thing, as this only used by the kernel.

		Version 0.01.01	03 July 2000	22:20		ObliVion
		Initial version.

		"Apartment of Computer Science"
*/

/*
	All these expressions, all these people, all their feelings
	All my hopes, all my dreams, all my hurts, all this light
	All energy being it good or evil, all this music, all this MUSIC
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
#include <kernel/setup.h>
#include <kernel/mm.h>

/* These symbols define the start and end of the kernel */
extern long k32_startup;
extern long end;

struct mem_info			kernel_mem_info;
#define			N_MEM_BLOCKS	((KERNEL_MEM_POOL *1024)/MEM_BLOCK_SIZE)

struct mem_block_node	mem_blocks[N_MEM_BLOCKS];

/* Forward decl */
struct mem_block_node	*mm_create_node(struct mem_block_node *prev_node, unsigned long size);
void 					mm_add_node(struct mem_block_node *node, unsigned char status);
void 					mm_release_node(struct mem_block_node *node);
unsigned long 			mm_find_node_by_mem(void *mem);
void 					calc_mem(void);

    unsigned long           di = 0;

/*
 *		struct mem_block_info	*find_node_by_mem(void *mem, struct *mem_block_node node)
 *
 *		Find a node by the memory it owns
 *  
 */
unsigned long mm_find_node_by_mem(void *mem)
{
	unsigned long	i = 0;
	
	while((i<N_MEM_BLOCKS) && (mem_blocks[i].mem<mem))
		i++;

	return(i);
}

/*
 *		int init_mm(void)
 *
 *		Initializes the Memory Manager
 *
 */
int init_mm(void)
{
	unsigned long	i;

	/*Initialize the array of blocks with non-allocated blocks*/
	for (i=0; i<N_MEM_BLOCKS; i++)
	{
		mem_blocks[i].prev = NULL;
		mem_blocks[i].next = NULL;
		mem_blocks[i].mem = i * MEM_BLOCK_SIZE + &end;
		mem_blocks[i].size = MEM_BLOCK_SIZE;
		mem_blocks[i].end_block = 0;
		mem_blocks[i].status = MM_FREE;
	}

	calc_mem();

	return(0);
}

/*
 *		void *alloc_block(unsigned long size, unsigned long owner)
 *
 *		allocate "size" bytes of memory at "first fit"
 *  
 */

void *alloc_block(unsigned long size)
{
	unsigned long			i = 0;
	unsigned long			j;
	unsigned long			blocks_found = 0;	
	unsigned long			blocks;
	unsigned long			end_block;

	blocks = size / MEM_BLOCK_SIZE +1;

	while ((blocks_found<blocks) && (i<N_MEM_BLOCKS))
	{
		if (mem_blocks[i].status==MM_FREE)
		{
				blocks_found = 1;

				while ((blocks_found<blocks) && (mem_blocks[i].status==MM_FREE) && (i<N_MEM_BLOCKS))
				{
					blocks_found++;
					i++;
				}
		}
		else
			i++;
	}

	j = blocks;
	end_block = i;

	mem_blocks[i].end_block = end_block;
	mem_blocks[i].status = MM_USED;

	while ((j-1)>0)
	{
		mem_blocks[i].end_block = end_block;
		mem_blocks[i].status = MM_USED;
		j--;
		i--;
	}

	kernel_mem_info.free_blocks -= blocks;
	kernel_mem_info.free_bytes -= blocks * MEM_BLOCK_SIZE;
	kernel_mem_info.used_blocks += blocks;
	kernel_mem_info.used_bytes += blocks * MEM_BLOCK_SIZE;

	return(mem_blocks[i].mem);
}

/*
 *		int dealloc_block(void *addr)
 *
 *		Deallocate block at "addr"
 *
 */

int dealloc_block(void *addr)
{
	unsigned long	first_block = 0;
	unsigned long	last_block = 0;
	unsigned long	i;

	first_block = mm_find_node_by_mem(addr);
	last_block = mem_blocks[first_block].end_block;

	kernel_mem_info.free_blocks += last_block - first_block;
	kernel_mem_info.free_bytes += (last_block - first_block) * MEM_BLOCK_SIZE;
	kernel_mem_info.used_blocks -= last_block - first_block;
	kernel_mem_info.used_bytes -= (last_block - first_block) * MEM_BLOCK_SIZE;

	for (i = first_block; i<=last_block; i++)
	{
		mem_blocks[i].end_block = i;
		mem_blocks[i].status = MM_FREE;
	}

	return(0);
}


/*
 *		unsigned long get_free_mem(void)
 *
 *		Returns all available mem
 *
 */
unsigned long get_free_mem(void)
{
	return(kernel_mem_info.free_bytes);
}

/*
 *		unsigned long get_used_mem(void)
 *
 *		Returns all used mem
 *
 */
unsigned long get_used_mem(void)
{
	return(kernel_mem_info.used_bytes);
}

/*
 *		unsigned long calc_mem(struct mem_block_node *mem_block, unsigned char status)
 *
 *		Refresh all memory info
 *
 */
void 					calc_mem(void)
{
	unsigned long	i;

	kernel_mem_info.free_blocks = 0;
	kernel_mem_info.free_bytes = 0;
	kernel_mem_info.used_blocks = 0;
	kernel_mem_info.used_bytes = 0;

	/*Check each block for its status to calculate free and used mem*/
	for (i=0; i<N_MEM_BLOCKS; i++)
	{
		switch(mem_blocks[i].status = MM_FREE)
		{
			case MM_FREE:	kernel_mem_info.free_blocks++;
							kernel_mem_info.free_bytes += MEM_BLOCK_SIZE;
							break;

			case MM_USED:	kernel_mem_info.used_blocks++;
							kernel_mem_info.used_bytes += MEM_BLOCK_SIZE;
							break;
		}

	}
}
