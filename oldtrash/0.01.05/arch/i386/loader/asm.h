/*		asm.h

		Include file that defines everything exported from asm.asm

	History:

		Version 0.01	4 May 2000	17:08		ObliVion
		Initial version.
*/

#ifndef LOADER_ASM_H
#define LOADER_ASM_H

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

extern void putch(char ch);
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
extern void jump_to_kernel(void);
extern void bochs_debug_out(unsigned long number);
extern void stop_floppy(void);
extern int basic_detect(void);
extern int get_state(void);

extern struct boot_header loader_setup;

struct kernel_setup *kernel_parameters = (struct kernel_setup *)(0x0500);

void (*run_kernel)(void);
int main(void);

#endif /*ASM_FILE*/

#endif