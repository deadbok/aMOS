/*		main.c

		The main "glue" C code for the aMOS loader

	History:

		Version 0.03	21 July	2000	20:53		ObliVion
		Added intitial (EGA?/VGA) video detection, and made a smarter putl function
		This might all need a little makeup
		Version 0.02	15 July	2000	23:55		ObliVion
		Made everything a little more stable (among other things the kernel was loaded twice).
		Version 0.01	04 May 	2000	17:08		ObliVion
		Initial version.
*/

/*		Did you know that you would grow older
		Did you understand what it meant
		Imagine the number og obscure events
		That has happened through the years you've lived
		If you really wonder
		Things have been quite a mess
		Things have changed in eratic ways
		But it's OK isn't it
		You can't get a hold on NOW
		But you know the background
		All trough you life
*/

/*
		Won't you wave me goodbye
*/

/*
		I wanna wake up next to you
		Feel the thrill of skin touching skin
		Dvelwe in in the sweet smell of bodies
		Listen to your quiet breath (Smurf)
		And see your childish smile
		As you sleep gracefully
*/
/*
		Does it really matter anymore
		Are there still pure needs in the core
		You've kept on disapointing me for so long
		All I want is to lay off this charade
		It's only then that we'll see if your strong
		I'm am not in for a trade
		So at this point I'm willing to destroy
		For the benefit of my broken core
		For the shere amusment of the boy
		The boy who I believe in no more

		I won't have you here
		So near
		To my precious core

		You know as well as I
		That I'm the only one
		Stupid enough to believe blindly in forever
*/

#include "common.h"
#include "disk.h"
#include <boot/boot.h>

void printstr (char *str)
{
	while (*str)
	{
		switch(*str)
		{
			case 0x0a:	putch(0x0d);
						putch(0x0a);
						break;
			default:	putch(*str);
						break;
		}
		str++;
	}
}

void putl(unsigned long l, int base)
{
	unsigned long	temp_long;
	char			str[200];
	char			ch;
	unsigned long	j=0;
	unsigned long	end=0;

	if (l==0)
	{
		str[0]='0';
		str[1]='\0';
	}
	else
	{
		while (l>0)
		{
			temp_long=l%base;

			if (temp_long<10)
				str[end++]=temp_long+'0';
			else
				str[end++]=temp_long-10+'a';
		
			l/=base;
		}
	
		/*	The string resulting from the previous processing is 
			reversed so we fix that... */

		for(j=0; j<(end>>1); j++)
		{
			ch=str[j];
			str[j]=str[end-(j+1)];
			str[end-(j+1)]=ch;
		}

		str[end]='\0';
	}
	printstr(str);
}

/*void putl(unsigned long i)
{
	char	str[255];
	char	ch;
	unsigned long	j=0;
	unsigned long	end=0;

	do
	{
		str[end++]=i%10+'0';
		i/=10;
	}
	while(i);
	str[end]='\0';
	
	for(j=0; j<(end>>1); j++)
	{
		ch=str[j];
		str[j]=str[end-(j+1)];
		str[end-(j+1)]=ch;
	}
		
	printstr(str);
}*/

int print_error(unsigned long errno, struct error *errors, char emit_noerror)
{
	unsigned long	i;

/*	I don't like the way of checking if we're of bounds
	The number of errors in the error array should be determined automaticly*/
	for (i=0; ((errno!=errors[i].errno) && (i<N_DISK_ERRORS)); i++);
	
	if (errno)
	{
		printstr("ERROR 0x");
		putl(errno, 0x10);
		printstr(": ");
	}

	if ((!errno) && (!emit_noerror))
		return(errno);
	
	printstr(errors[i].error_msg);
	printstr("\n");

	return(errno);
}


int LBA_to_CHS(unsigned long sector, struct geometry *chs_geometry)
{
	chs_geometry->sector=sector%(disk_geometry.sector);
	chs_geometry->head=sector/disk_geometry.sector;
	chs_geometry->cylinder=chs_geometry->head/disk_geometry.head;
	chs_geometry->head%=disk_geometry.head;
	return(0);
}

int read_kernel(void)
{
	unsigned long		i, sectors, current_sector, buffer_seg;
	unsigned long		sectors_per_bar;
	unsigned char		*kernel_ptr;
	unsigned char		*buffer_ptr;
	struct geometry		chs_offset;
	int					err;

	current_sector=loader_setup.kernel_start;

	buffer_seg=LOAD_BUF_SEG;
	kernel_ptr=(unsigned char *)KERNEL_LIN;
	buffer_ptr=(unsigned char *)(LOAD_BUF_SEG<<4);

	sectors=loader_setup.kernel_sectors;

	sectors_per_bar=sectors/50+1;

	while (sectors>=(current_sector-loader_setup.kernel_start))
	{
		LBA_to_CHS(current_sector, &chs_offset);
		err=disk_read_standard(loader_setup.boot_drive,
								chs_offset.cylinder,
								chs_offset.head,
								chs_offset.sector,
								1,
								buffer_seg);
		if (err)	
			return(err);

		printstr("\r    -|");

		i=0;
		while(i<sectors)
		{
			if (i<=(current_sector-loader_setup.kernel_start))
				printstr("=");
			else
				printstr("-");

			i+=sectors_per_bar;
		}

		printstr("|- ");

		putl(((((current_sector-loader_setup.kernel_start)<<8)/sectors)*100)>>8, 10);

		printstr("%");

		current_sector+=1;

		for (i=0; i<512; i++)
		{
			*kernel_ptr=buffer_ptr[i];
			kernel_ptr++;
		}
	}
	printstr("\n...");
	return(0);

}

/*
 *		void dump_mem(unsigned char *ptr, unsigned long bytes)
 *		dumps memory at "ptr" and the next "bytes" bytes
 *  
 *  
 *  	i:	plex86
 */

void dump_mem(unsigned char *ptr, unsigned long bytes)
{
	unsigned long	i;
	int				j;
	char			ch;
    static char 	hex[] = "0123456789abcdef";
	
	for (i=0; i<bytes; i++)
	{
        if (!(i%16)) 
        {
            for (j=20; j>=0; j-=4)
                putch(hex[((i+(unsigned long)ptr)>>j) & 0x0f]);

            putch(':');
            putch(' ');
        }

        ch=ptr[i];
        putch(hex[(ch>>4) & 0x0f]);
        putch(hex[ch & 0x0f]);
        
        if (!((i+1)%2)) 
            putch(' ');
        if (!((i+1)%16)) 
			printstr("\n");
	}
	printstr("\n");
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

int detect_video(struct screen_info *screen_setup)
{
	unsigned int	video_type;
	unsigned int	video_state;
	
	video_type=basic_detect();
	switch(video_type)
	{
		case VIDEO_TYPE_CGA:	screen_setup->type[0]='C';
								screen_setup->type[1]='G';
								screen_setup->type[2]='A';
								screen_setup->type[3]='\0';
								break;
		case VIDEO_TYPE_EGA:	screen_setup->type[0]='E';
								screen_setup->type[1]='G';
								screen_setup->type[2]='A';
								screen_setup->type[3]='\0';
								break;
		case VIDEO_TYPE_VGA:	screen_setup->type[0]='V';
								screen_setup->type[1]='G';
								screen_setup->type[2]='A';
								screen_setup->type[3]='\0';
								break;
	}

	if ((video_type==VIDEO_TYPE_EGA) || (video_type==VIDEO_TYPE_VGA))
	{
		screen_setup->monochrome=is_monochrome();

		video_state=get_state();
		screen_setup->mode=video_state & 0x7f;
		screen_setup->res_x=(video_state >> 8) & 0xff;
		screen_setup->page=(video_state >> 16) & 0xff;

		screen_setup->addr=0xb8000;

		screen_setup->res_y=get_height();
	}
	printstr("        Adapter type:   ");
	if (screen_setup->monochrome)
		printstr("Monochrome ");
	else
		printstr("Color ");
	printstr(screen_setup->type);
	printstr("\n");

	printstr("        Memory address: 0x");
	putl(screen_setup->addr, 0x10);
	printstr("\n");

	printstr("        Mode:           0x");
	putl(screen_setup->mode, 0x10);
	printstr("\n");

	printstr("        Page:           ");
	putl(screen_setup->page, 10);
	printstr("\n");

	printstr("        X resolution:   ");
	putl(screen_setup->res_x, 10);
	printstr("\n");

	printstr("        Y resolution:   ");
	putl(screen_setup->res_y, 10);
	printstr("\n");


	return(0);
}

int main(void) 
{
	unsigned long		cursor_pos;

	printstr("OK\n");
	printstr("    Detecting video hardware...\n");
	if (detect_video(&kernel_parameters->screen_setup))
	{
		return(1);
	}

	printstr("    Resetting disk...");
	if (print_error(disk_reset(loader_setup.boot_drive), disk_errors, TRUE))
	{
		return(1);
	}

	printstr("    Quering disk info...");
	if (print_error(disk_info_standard(loader_setup.boot_drive,
									   &disk_geometry.cylinder,
									   &disk_geometry.head,
									   &disk_geometry.sector),
					 disk_errors, TRUE))
	{
		return(1);
	}

	printstr("...OK\n");

	printstr("Loading Kernel (");
	putl(loader_setup.kernel_sectors, 10);
	printstr(" sectors)...\n");
	if (print_error(read_kernel(), disk_errors, TRUE))
	{
		return(1);
	}

	printstr("Killing floppy motor...\n");
	stop_floppy();

	printstr("Saving cursor position...\n");
	cursor_pos=get_cursor_pos(kernel_parameters->screen_setup.page);
	kernel_parameters->screen_setup.x=cursor_pos & 0x00ff;
	kernel_parameters->screen_setup.y=(cursor_pos >> 8) & 0x00ff;
	
	printstr("Starting Kernel...");
	run_kernel=(void *)(KERNEL_LIN);
	(*run_kernel)();
	
	printstr("ERROR\n");

	return(0);
}
