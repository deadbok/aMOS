/*		video.c

		The video output function used by the loader and the micro-kernel

	History:
		Version 0.01.02	16 Jul 2000		00:59		ObliVion
		More clever when it comes to positioning the cursor

		Version 0.01.01	12 Sep 	2000	17:26		ObliVion
		Initial version.
*/
#include <string.h>
#include <kernel/ports.h>
#include <kernel/video.h>
#include <kernel/mm.h>
#include <kernel/mailbox.h>

void 			vid_set_attr(int attr, int value);
void 			vid_set_cursor_pos(unsigned short pos);
unsigned long	vid_get_cursor_pos(void);
void			vid_rcv_func(struct msg *message);
void 			vid_update_xy(unsigned short _pos);
void			vid_update_pos(unsigned short _x, unsigned short _y);

mailbox_id_t	video_mailbox = "/dev/video";

unsigned short	xres, yres;
unsigned short	x, y;

unsigned long	vram_addr;
unsigned long	offset;

unsigned char	attributes = 0;
unsigned short	page;

unsigned long	pos;

int vid_init(void)
{
	vram_addr=0xb8000;

	xres=80;
	yres=25;

	vid_set_attr(BG_COLOR, BLACK);
	vid_set_attr(FG_COLOR, WHITE);

	pos=vid_get_cursor_pos();
	vid_update_xy(pos);
	offset=pos<<1;

	create_vid_mailbox();

	return(0);
}

int create_vid_mailbox(void)
{
    create_mb(video_mailbox, vid_rcv_func);
    return(0);
}

void vid_rcv_func(struct msg *message)
{
	unsigned long		i;

	switch (message->type)
	{
		case VMSG_CHAR:		vid_putch(*(char *)message->data);
								break;
		case VMSG_STRING:	i = 0;
								while (((char *)message->data)[i] != '\0')
								{
									vid_putch(((char *)message->data)[i]);
									i++;
								}
								break;
	}
}

void vid_putch(char ch)
{
	unsigned char	*vram_ptr;

	switch(ch)
	{
		case '\n':	y++;
					x = 0;
					vid_update_pos(x, y);
					if (y>=yres)
					{
						vid_scroll_up();
					}
					offset=pos<<1;
					vid_set_cursor_pos(pos);
					break;

		case '\r':	x = 0;
					vid_update_pos(x,y);
					offset=pos<<1;
					vid_set_cursor_pos(pos);
					break;

		default:	vram_ptr=(unsigned char *)vram_addr;

					vram_ptr[offset++]=ch;
					vram_ptr[offset++]=attributes;

					x++;
					vid_update_pos(x, y);
					vid_set_cursor_pos(pos);

					if (x>=xres)
						vid_putch('\n');
					break;
	}
}

void vid_putstr(char *str)
{
	unsigned long	i;
	unsigned long	characters;
	
	characters = strlen(str);
	
	for (i=0; i<characters; i++)
		vid_putch(str[i]);
}


void vid_scroll_up(void)
{
	unsigned long	i;
	unsigned char	*vram_ptr;

	vram_ptr=(unsigned char *)vram_addr;
					
	for (i=0; i<(xres*(yres-1))*2; i++)
		vram_ptr[i]=vram_ptr[i+xres*2];

	vram_ptr=(unsigned char *)vram_addr+((xres*(yres-1)*2));

	for (i=0; i<xres; i++)
	{
		(*vram_ptr++)=' ';
		(*vram_ptr++)=attributes;
	}
	pos-=xres;
	offset=pos<<1;
	vid_update_xy(pos);
	vid_set_cursor_pos(pos);
}

void vid_clear(void)
{
	int				i;
	unsigned char	*vram_ptr;

	vram_ptr=(unsigned char *)vram_addr;

	for (i=0; i<2000; i++)
	{
		(*vram_ptr++)=' ';
		(*vram_ptr++)=attributes;
	}
	pos=0;
	offset=0;
	vid_set_cursor_pos(0);
}

void vid_set_attr(int attr, int value)
{
	switch(attr)
	{
		case	0:	attributes= (attributes	& 0xf0) | (value & 0x0f);
					break;
		case	1:	attributes= (attributes	& 0x0f) | ((value & 0x0f)<<4);
					break;
	}
}
	
void vid_set_cursor_pos(unsigned short new_pos)
{
	outb(CRT_INDEX, CURSOR_POS_HI);
	outb(CRT_DATA, new_pos>>8);
	outb(CRT_INDEX, CURSOR_POS_LO);
	outb(CRT_DATA, new_pos);
}

unsigned long vid_get_cursor_pos(void)
{
	unsigned long	tmp_pos;
					
	outb(CRT_INDEX, CURSOR_POS_HI);
	tmp_pos=inb(CRT_DATA)<<8;
				
	outb(CRT_INDEX, CURSOR_POS_LO);
	tmp_pos+=inb(CRT_DATA);
					
	return(tmp_pos);
}

void vid_update_xy(unsigned short _pos)
{
	y=_pos / xres;
	x=_pos - ( y * xres);
}

void vid_update_pos(unsigned short _x, unsigned short _y)
{
	pos=((xres) * _y)+_x;
}

