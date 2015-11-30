/*		video.h

		Header for the kernel implementation of video-devices

	History:

		Version 0.01.01	08 Sep 	2000	18:30		ObliVion
		Initial version.
*/
/*! \file hik/video.h
	\brief Header for the kernel implementation of video-devices

	This is architecture independant part of the video-device driver
*/
#ifndef _VIDEO_H
#define _VIDEO_H

#include <hik/mailbox.h>

/*!	\brief This is the message type for sending a single character to the video-device
*/
#define VMSG_CHAR			1
/*!	\brief This is the message type for sending a string to the video-device
*/
#define VMSG_STRING			2
/*!	\brief This is the message type for sending setting the foreground color of the video-device
*/
#define VMSG_FCOLOR			3
/*!	\brief This is the message type for sending setting the background color of the video-device
*/
#define VMSG_BCOLOR			4

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

extern mailbox_id_t		video_mailbox;

#endif
