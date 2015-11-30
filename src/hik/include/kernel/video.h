/*		video.h

		The video output function used by the loader and the micro-kernel

	History:

		Version 0.01.01	12 Sep 	2000	17:26		ObliVion
		Initial version.
*/
/*! \file hik/include/kernel/video.h
	\brief Header for the kernel implementation of video-devices

	This is architecture dependant part of the video-device driver
*/
#ifndef _HIK_VIDEO_H
#define _HIK_VIDEO_H

#include <hik/video.h>

/*! The port number of the CRT index port of the VGA */
#define	CRT_INDEX			0x03d4
/*! The port number of the CRT data port of the VGA */
#define	CRT_DATA			0x03d5

/*! The index number to the horiazntal diplay end register of the VGA */
#define HOR_DISP_END		0x01
/*! The index number to the high part of curser position register on the VGA */
#define CURSOR_POS_HI		0x0e
/*! The index number to the low part of curser position register on the VGA */
#define CURSOR_POS_LO		0x0f

/*! We want to set the foreground color */
#define FG_COLOR			0
/*! We want to set the background color */
#define BG_COLOR			1

/*! Turns on blinking charecters */
#define BLINK				0x80

/*! Color black */
#define BLACK				0x00
/*! Color blue */
#define BLUE				0x01
/*! Color green */
#define GREEN				0x02
/*! Color cyan */
#define CYAN				0x03
/*! Color red */
#define RED					0x04
/*! Color magenta */
#define MAGENTA				0x05
/*! Color brown */
#define BROWN				0x06
/*! Color white */
#define WHITE				0x07
/*! Color gray */
#define GRAY				0x08
/*! Color light blue */
#define LIGHT_BLUE			0x09
/*! Color light green */
#define LIGHT_GREEN			0x0a
/*! Color light cyan */
#define LIGHT_CYAN			0x0b
/*! Color light red */
#define LIGHT_RED			0x0c
/*! Color light magenta */
#define LIGHT_MAGENTA		0x0d
/*! Color light brown */
#define LIGHT_BROWN			0x0e
/*! Color light yellow */
#define YELLOW				0x0e
/*! Color bright white */
#define BRIGHT_WHITE		0x0f

extern int	vid_init(void);
extern int	create_vid_mailbox(void);
extern void	vid_putch(char ch);
extern void vid_putstr(char *str);
extern void	vid_clear(void);
/*extern int	vid_get_xres(void);
extern int	vid_get_yres(void);*/
extern void vid_scroll_up(void);
extern void vid_set_attr(int attr, int value);

#endif
