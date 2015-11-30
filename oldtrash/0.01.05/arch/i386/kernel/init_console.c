/*		preinit_stdout.c

		Architecture dependant firts time console driver
		
		The console architecture isn't laid out properly yet
		I've lost myself in endless loops thinking about this
		witch is a sure sign that this is far from finished
		I don't even know if I should use "console" as a 
		description of what this is

	History:

		Version 0.01.00	23 July 2000	09:58		ObliVion
		Initial version.
*/
#include <i386/setup.h>
#include <i386/ports.h>
#include <console/console.h>
#include <boot/boot.h>
#include <string.h>

#define	CRT_INDEX		0x03d4
#define	CRT_DATA		0x03d5

#define HOR_DISP_END	0x01
#define BASE_ADDR_HI	0x0c
#define BASE_ADDR_LO	0x0d
#define CURSOR_POS_HI	0x0e
#define CURSOR_POS_LO	0x0f

struct kernel_setup		*kernel_param = (struct kernel_setup *)(KERNEL_PARAM);
struct console_dev		init_console;

unsigned char		attributes = 0;
unsigned long		vram_base = 0xb8000;
unsigned long		pos;
unsigned long		offset;
unsigned short		x_res;
unsigned short		y_res;

void vga_init(void);
void vga_putc(char ch);
void vga_clear(void);
void vga_set_attr(int attr, int value);
void vga_close(void);
void set_cursor_pos(unsigned short pos);
int	vga_xres(void);
int	vga_yres(void);
void scroll_up(void);

void incpos(unsigned long characters);

int preinit_console(void)
{
	unsigned char	*vram_ptr;

	if (!strcmp(kernel_param->screen_setup.type,"VGA"))
	{

		vram_base=kernel_param->screen_setup.addr;
		vram_ptr=(unsigned char *)vram_base;

		vga_set_attr(BG_COLOR, BLACK);
		vga_set_attr(FG_COLOR, WHITE);
		x_res=kernel_param->screen_setup.res_x;
		y_res=kernel_param->screen_setup.res_y;
		pos=kernel_param->screen_setup.x*x_res+kernel_param->screen_setup.y;
		offset=pos<<1;

		init_console.init=NULL; 
		init_console.putc=vga_putc;
		init_console.clear=vga_clear;
		init_console.set_attr=vga_set_attr;
		init_console.get_xres=vga_xres;
		init_console.get_yres=vga_yres;
		init_console.close=vga_close; 

		if (!add_console(&init_console))
			return(1);

		if (console_switch(0))
			return(1);

		vga_clear();
		return(0);
	}
	return(1);
}

/*void console_init(int cons_dev)
{
	console_set_attr(0, BG_COLOR, BLACK);
	console_set_attr(0, FG_COLOR, WHITE);
	vram_base=get_vram_base();
	x_res=get_x_res();
	console_clear(0);
	vga_putc('C');
	pos=0;
	offset=0;
	set_cursor_pos(0);
}*/

void vga_putc(char ch)
{
	unsigned char	*vram_ptr;

	switch(ch)
	{
		case '\n':	pos=((pos/x_res)+1)*x_res;
					if (pos>=(x_res*y_res))
					{
						scroll_up();
					}
					offset=pos<<1;
					set_cursor_pos(pos);
					break;

		default:	vram_ptr=(unsigned char *)vram_base;

					vram_ptr[offset++]=ch;
					vram_ptr[offset++]=attributes;

					pos++;;
					set_cursor_pos(pos);

					if (pos>=(((pos/x_res)*x_res)+x_res))
						vga_putc('\n');
					break;
	}

}

void scroll_up(void)
{
	unsigned long	i;
	unsigned char	*vram_ptr;

	vram_ptr=(unsigned char *)vram_base;
	memcpy(vram_ptr, vram_ptr+(x_res*2), (x_res*(y_res-1))*2);

	vram_ptr=(unsigned char *)vram_base+((x_res*(y_res-1)*2));

	for (i=0; i<x_res; i++)
	{
		(*vram_ptr++)=' ';
		(*vram_ptr++)=attributes;
	}
	pos-=x_res;
	offset=pos<<1;
	set_cursor_pos(pos);
}

void vga_clear(void)
{
	int				i;
	unsigned char	*vram_ptr;

	vram_ptr=(unsigned char *)vram_base;

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

int vga_xres(void)
{
	return(x_res);
}

int	vga_yres(void)
{
	return(y_res);
}
