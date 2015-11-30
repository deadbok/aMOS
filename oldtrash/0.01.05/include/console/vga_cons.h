/*		vga_cons.h

		Include file for the VGA console driver

	History:

		Version 0.01	31 May 2000	14:16		ObliVion
		Initial version.
*/

#ifndef VGA_CONSOLE_H
#define VGA_CONSOLE_H

/*defines for the CRT Controller Register*/
#define	CRT_INDEX		0x03d4
#define	CRT_DATA		0x03d5

#define HOR_DISP_END	0x01
#define BASE_ADDR_HI	0x0c
#define BASE_ADDR_LO	0x0d
#define CURSOR_POS_HI	0x0e
#define CURSOR_POS_LO	0x0f

extern	void vga_init(void); 
extern	void vga_putc(char ch);
extern	void vga_clear(void);
extern	void vga_set_attr(int attr, int value);
extern	void vga_close(void);
extern	unsigned short 	get_x_res(void);
extern	unsigned short 	get_y_res(void);


#endif