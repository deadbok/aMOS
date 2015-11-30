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

#ifndef _HIK_BOOT_H_
#define _HIK_BOOT_H_

/* Boot */
#define BOOT_SEG		0x007c0

/* Loader */
#define LOADER_OFF		0x08000
#define LOADER_SEC		0x00010

/*! \brief Address of a buffer segment by the loader */
#define BUF_SEG			0x007c0

#define RM_STACK		0x07b00
#define PM_INIT_STACK	0x001fffff

#define MEM_INFO_LIN	0x00200000

/* kernel */
/*#define KERNEL_SEG		0x010000*/
#define KERNEL_LIN		0x0100000
#define KERNEL_PARAM	0x0500

#ifndef ASM_FILE

/*! \brief This structure follow the jump instruction in the bootsector */
struct boot_header
{
	/*!	The BIOS number of the boot drive */
	unsigned char		boot_drive;
/*	unsigned char		heads;
	unsigned short		sectors;*/
	/*!	Bytes per sector on the boot drive */
	unsigned short		bytes_per_sector;
	/*!	The number of sectors that the boot sector spans */
	unsigned char		boot_sectors;
	/*!	The sector number of the start of the loader */
	unsigned long		loader_start;
	/*!	The number of sectors the loader spans */
	unsigned long		loader_sectors;
	/*!	The sector number of the start of the kernel */
	unsigned long		kernel_start;
	/*!	The number of kernel the loader spans */
	unsigned long		kernel_sectors;
} __attribute__ ((packed));

/* This structure is (was) used by the loader and kernel.
 * It contains information about the video hardware of the system.
 * It's filled by the loader and used by the kernel to init a
 * console as first thing so we can se what the kernel is doing
 */

/*struct screen_info
{
	unsigned short	x;
	unsigned short	y;
	unsigned short	page;
	unsigned long	addr;
	unsigned short	mode;
	unsigned short	res_x;
	unsigned short	res_y;
	unsigned char	monochrome;
	char			type[6];
} __attribute__ ((packed));*/

/* This structure used by the loader and kernel.
 * It contains the initialization for the i386-kernel
 */

struct kernel_param
{
	unsigned char		console_level;
	unsigned long		putch;
} __attribute__ ((packed));


#endif /*ASM_FILE*/

#endif

