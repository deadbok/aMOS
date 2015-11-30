/*		common.h

		Common decleration and defines for the aMOS kernel loader

	History:

		Version 0.01	09 Apr 2000  18:05     ObliVion
		Initial version.
*/

#ifndef LOADER_COMMON_H
#define LOADER_COMMON_H

#include "asm.h"

#undef NULL
#define NULL		((void *) 0)

#undef FALSE
#define FALSE		0
#undef TRUE
#define TRUE		1

extern long end;

struct error
{
	
	unsigned long	errno;
	char			*error_msg;
};

struct geometry
{
	unsigned long	cylinder;
	unsigned long	head;
	unsigned long	sector;
} disk_geometry;

extern void	printstr(char *str);
extern void putl(unsigned long l, int base);
extern int	print_error(unsigned long errno, struct error *errors, unsigned long n_errors, char emit_noerror);
extern int	LBA_to_CHS (unsigned long sector, struct geometry *chs_geometry);
extern int	read_kernel (void);
extern void dump_mem(unsigned char *mem, unsigned long size);

#endif

