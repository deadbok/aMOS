/*		boot.h

		Structures and defines for the aMOS bootsector

	History:

		Version 0.01	09 Apr 2000  18:05     ObliVion
		Initial version.
*/


				/* My wake-up chick */
				/* Play your role */
				/* Play it cold */
				/* Don't hang around for to long */
				/* I'll introduce you to my polish sorrow */
				/* I'll tell you about my foreign wounds */
				/* I will not forgive her */
				/* As we will not let go */

#ifndef _AMOS_BOOT_H
#define _AMOS_BOOT_H

/* Boot */
#define BOOT_SEG		0x007c0

/* Loader */
#define LOADER_OFF		0x08000
#define LOADER_SEC		0x00010
#define LOAD_BUF_SEG	0x007c0

#define RM_STACK		0x07b00
#define PM_INIT_STACK	0x001fffff

/* kernel */
/*#define KERNEL_SEG		0x010000*/
#define KERNEL_LIN		0x0100000
#define KERNEL_PARAM	0x0500

#ifndef ASM_FILE

/* This structure follow the jump instruction in the bootsector */

struct boot_header
{
	unsigned char		boot_drive;
/*	unsigned char		heads;
	unsigned short		sectors;*/
	unsigned short		bytes_per_sector;
	unsigned char		boot_sectors;
	unsigned long		loader_start;
	unsigned long		loader_sectors;
	unsigned long		kernel_start;
	unsigned long		kernel_sectors;
} __attribute__ ((packed));

/* This structure used by the loader and kernel.
 * It contains information about the video hardware of the system.
 * It's filled by the loader and used by the kernel to init a
 * console as first thing so we can se what the kernel is doing
 */

struct screen_info
{
	unsigned short	x;
	unsigned short	y;
	unsigned short	page;
	unsigned long	addr;
	unsigned short	mode;
	unsigned short	res_x;
	unsigned short	res_y;
	unsigned char	monochrome;
	unsigned char	type[6];
} __attribute__ ((packed));

/* This structure used by the loader and kernel.
 * It contains the initialization for the i386-kernel
 */

struct kernel_setup
{
	unsigned char		console_level;
	struct screen_info	screen_setup;
} __attribute__ ((packed));


#endif /*ASM_FILE*/

#endif

