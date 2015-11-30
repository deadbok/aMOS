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

void printstr (char *str);
void putl(unsigned long l, int base);
int print_error(unsigned long errno, struct error *errors, char emit_noerror);
int LBA_to_CHS (unsigned long sector, struct geometry *chs_geometry);
int read_kernel (void);
void dump_mem(unsigned char *mem, unsigned long size);
int get_height(void);
char is_monochrome(void);
int detect_video(struct screen_info *screen_setup);
int get_cursor_pos(char page);

#endif
