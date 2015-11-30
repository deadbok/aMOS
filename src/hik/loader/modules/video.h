/*		video.h

		The video output function used by the loader and the micro-kernel

	History:

		Version 0.01.01	12 Sep 	2000	17:26		ObliVion
		Initial version.
*/

#include "asm.h"

#ifndef _LOADER_VIDEO_H
#define _LOADER_VIDEO_H

#define	CRT_INDEX		0x03d4
#define	CRT_DATA		0x03d5

#define HOR_DISP_END	0x01
#define BASE_ADDR_HI	0x0c
#define BASE_ADDR_LO	0x0d
#define CURSOR_POS_HI	0x0e
#define CURSOR_POS_LO	0x0f

#define FG_COLOR		0
#define BG_COLOR		1

#define BLINK			0x80

#define BLACK			0x00
#define BLUE			0x01
#define GREEN			0x02
#define CYAN			0x03
#define RED				0x04
#define MAGENTA			0x05
#define BROWN			0x06
#define WHITE			0x07
#define GRAY			0x08
#define LIGHT_BLUE		0x09
#define LIGHT_GREEN		0x0a
#define	LIGHT_CYAN		0x0b
#define	LIGHT_RED		0x0c
#define	LIGHT_MAGENTA	0x0d
#define LIGHT_BROWN		0x0e
#define YELLOW			0x0e
#define BRIGHT_WHITE	0x0f

extern void	putch(char ch);
extern char	is_monochrome(void);
extern int	get_height(void);
extern int	init_video(void);

#endif
