/*		video.c

		The video output function used by the loader and the micro-kernel

	History:

		Version 0.01.01	12 Sep 	2000	17:26		ObliVion
		Initial version.
*/
#include "common.h"
#include "video.h"

int vga_init(void);
void vga_putch(char ch);
void vga_clear(void);
void vga_set_attr(int attr, int value);
void vga_close(void);
void set_cursor_pos(unsigned short pos);
int	vga_xres(void);
int	vga_yres(void);
void scroll_up(void);
__inline__ void outb(unsigned int port, unsigned char value);

unsigned short	mode;
unsigned short	res_x;
unsigned short	res_y;
unsigned char	monochrome;
unsigned int	video_type = 0; 

unsigned long	vram_addr;
unsigned long	offset;

unsigned char	attributes = 0;
unsigned short	page;

unsigned short	x;
unsigned short	y;
unsigned long	pos;

void putch(char ch)
{
	switch(video_type)
	{
		case	0:				BIOS_putch(ch);
								break;
		case	VIDEO_TYPE_VGA:	vga_putch(ch);
								break;
	}
}

char is_monochrome(void)
{
	unsigned char	*monochrome = (unsigned char *)(0x0487);

	return(((*monochrome) >> 1) & 0x01);
}

int get_height(void)
{
	unsigned char	*height = (unsigned char *)(0x0484);

	return((*height)+1);
}

int init_video(void)
{
	unsigned int	video_state;

	printstr("    Installing video driver...");	
	video_type=basic_detect();

	if ((video_type==VIDEO_TYPE_EGA) || (video_type==VIDEO_TYPE_VGA))
	{
		monochrome=is_monochrome();

		video_state=get_state();
		mode=video_state & 0x7f;
		res_x=(video_state >> 8) & 0xff;
		page=(video_state >> 16) & 0xff;

		vram_addr=0xb8000;

		res_y=get_height();
	}
					
	switch(video_type)
	{
		case VIDEO_TYPE_VGA:	vga_init();
								break;
		default:				printstr("Adapter type not supported!\n");
								break;
	}

	return(0);
}

int vga_init(void)
{
	unsigned long		cursor_pos = 0;
					
	cursor_pos=get_cursor_pos(page);
	x=cursor_pos & 0x00ff;
	y=(cursor_pos >> 8) & 0x00ff;
	pos=(res_x*y)+x;
	offset=pos<<1;

	vga_set_attr(BG_COLOR, BLACK);
	vga_set_attr(FG_COLOR, WHITE);
					
	set_cursor_pos(pos);

/*	vga_clear();*/

	printstr("VGA\n");

	return(0);
}

void vga_putch(char ch)
{
	unsigned char	*vram_ptr;

	switch(ch)
	{
		case '\n':	pos=((pos/res_x)+1)*res_x;
					if (pos>=(res_x*res_y))
					{
						scroll_up();
					}
					offset=pos<<1;
					set_cursor_pos(pos);
					break;

		case '\r':	pos=((pos/res_x))*res_x;
					offset=pos<<1;
					set_cursor_pos(pos);
					break;

		default:	vram_ptr=(unsigned char *)vram_addr;

					vram_ptr[offset++]=ch;
					vram_ptr[offset++]=attributes;

					pos++;;
					set_cursor_pos(pos);

					if (pos>=(((pos/res_x)*res_x)+res_x))
						vga_putch('\n');
					break;
	}

}

void scroll_up(void)
{
	unsigned long	i;
	unsigned char	*vram_ptr;

	vram_ptr=(unsigned char *)vram_addr;
					
	for (i=0; i<(res_x*(res_y-1))*2; i++)
		vram_ptr[i]=vram_ptr[i+res_x*2];

	vram_ptr=(unsigned char *)vram_addr+((res_x*(res_y-1)*2));

	for (i=0; i<res_x; i++)
	{
		(*vram_ptr++)=' ';
		(*vram_ptr++)=attributes;
	}
	pos-=res_x;
	offset=pos<<1;
	set_cursor_pos(pos);
}

void vga_clear(void)
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
	set_cursor_pos(0);
}

void vga_set_attr(int attr, int value)
{
	switch(attr)
	{
		case	0:	attributes= (attributes	& 0xf0) | (value & 0x0f);
					break;
		case	1:	attributes= (attributes	& 0x0f) | ((value & 0x0f)<<4);
					break;
	}
}
	
void vga_close(void)
{
}

void set_cursor_pos(unsigned short pos)
{
	outb(CRT_INDEX, CURSOR_POS_HI);
	outb(CRT_DATA, pos>>8);
	outb(CRT_INDEX, CURSOR_POS_LO);
	outb(CRT_DATA, pos);
}

__inline__ void outb(unsigned int port, unsigned char value)
{
   __asm__ volatile ("outb %%al,%%dx"::"d" (port), "a" (value));
}
