/*		console.h

		Console I/O

	History:

		Version 0.01	31 May 2000	14:16		ObliVion
		Initial version.
*/
#ifndef KERNEL_CONSOLE_DEVICE_H
#define KERNEL_CONSOLE_DEVICE_H

/*#include <util/list.h>*/

#define MAX_CONS_DEV	10

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

/* Linux console driver structure
struct consw {
	const char *(*con_startup)(void);
	void	(*con_init)(struct vc_data *, int);
	void	(*con_deinit)(struct vc_data *);
	void	(*con_clear)(struct vc_data *, int, int, int, int);
	void	(*con_putc)(struct vc_data *, int, int, int);
	void	(*con_putcs)(struct vc_data *, const unsigned short *, int, int, int);
	void	(*con_cursor)(struct vc_data *, int);
	int	(*con_scroll)(struct vc_data *, int, int, int, int);
	void	(*con_bmove)(struct vc_data *, int, int, int, int, int, int);
	int	(*con_switch)(struct vc_data *);
	int	(*con_blank)(struct vc_data *, int);
	int	(*con_font_op)(struct vc_data *, struct console_font_op *);
	int	(*con_set_palette)(struct vc_data *, unsigned char *);
	int	(*con_scrolldelta)(struct vc_data *, int);
	int	(*con_set_origin)(struct vc_data *);
	void	(*con_save_screen)(struct vc_data *);
	u8	(*con_build_attr)(struct vc_data *, u8, u8, u8, u8, u8);
	void	(*con_invert_region)(struct vc_data *, u16 *, int);
	u16    *(*con_screen_pos)(struct vc_data *, int);
	unsigned long (*con_getxy)(struct vc_data *, unsigned long, int *, int *);
};
*/

struct console_dev
{
	void	(*init)(void); 
	void	(*putc)(char ch);
	void	(*clear)(void);
	void	(*set_attr)(int attr, int value);
	int		(*get_xres)(void);
	int		(*get_yres)(void);
	void	(*close)(void);
	struct	console_dev	*next;
};

extern	void	console_init(void); 
extern	int 	add_console(struct console_dev *console);
extern	void	console_putc(char ch);
extern	void	console_clear(void);
extern	void	console_set_attr(int attr, int value);
extern	int		console_xres(void);
extern	int		console_yres(void);
extern	void	console_close(void);
extern	int		console_switch(int cons_dev);


#endif
