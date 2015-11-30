/*		asm.h

		Include file that defines everything exported from asm

	History:

		Version 0.01	4 May 2000	17:08		ObliVion
		Initial version.
*/

#ifndef _LOADER_ASM_H
#define _LOADER_ASM_H

#include <boot/boot.h>

#define D_DATA				0x12   		/*segment is data, read/write */
#define D_CODE    			0x1a		/*segment is code, read/exec */
#define D_PRESENT			0x80		/*Present*/

#define D_4KGRAN			0x80		/*4K Granularity*/
#define D_32BIT				0x40		/*Default to 32 bit mode (USE32)*/

#define CODE_32				0x08		/*4K Granularity*/
#define DATA_32				0x10		/*Default to 32 bit mode (USE32)*/
#define CODE_16				0x18		/*4K Granularity*/
#define DATA_16				0x20		/*Default to 32 bit mode (USE32)*/

#define VIDEO_TYPE_CGA		0x01
#define VIDEO_TYPE_EGA		0x02
#define VIDEO_TYPE_VGA		0x03

#ifndef ASM_FILE

extern void BIOS_putch(char ch);
extern int disk_reset(int drive);
extern int  disk_info_standard(int drive,
							   unsigned long *cylinders,
							   unsigned long *heads,
							   unsigned long *sectors);
extern int disk_read_standard(int disk,
							   unsigned long cylinder,
							   unsigned long head,
							   unsigned long sector,
							   unsigned long sectors,
							   unsigned long segment);
extern void	stop_floppy(void);
extern int	basic_detect(void);
extern int	get_state(void);

/*extern int	get_cursor_pos(char page);*/
extern int	engage(void);
extern int	get_mem_info_e820(unsigned long *table_index, unsigned long *size, unsigned long segment);
extern int	get_mem_info_e801(unsigned long low_ext_mem, unsigned long high_ext_mem);

extern struct boot_header loader_setup;

typedef int (*kernel_entry)(struct kernel_param kernel_parameters, unsigned long param_size);
extern kernel_entry		run_kernel;

extern struct loader_mem_info	*mem_info;
#endif /*ASM_FILE*/

#endif

