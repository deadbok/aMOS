/*		mkimage.c

		Merges the final aMOS bootsector, loader, and kernel
		Then update the needed parameters in the bootsector

	History:

		Version 0.03.01	16 july 2000	00:09	ObliVion
		Makeup.

		Version 0.03.00	17 May 2000		05:29	ObliVion
		Preliminary support for kernel image
		Perfected the padding (still slow)

		Version 0.02.03	15 May 2000		21:43	ObliVion
		Added ability to pad the final image to a certain size
		(not perfect)

		Version 0.02.02	06 May 2000		17:15		ObliVion
		All options are now written to the bootsector
		Now stores parameters in sectors

		Version 0.02.01	29 Apr 2000		17:26		ObliVion
		Enabled writting some option to the aMOS loader

		Version 0.02.00	09 Apr 2000		20:15		ObliVion
		Total rewrite.
		The configuration is now loaded from boot.cfg

		Version 0.01.00	07 Apr 2000		00:08		ObliVion
		Initial version.

	BUGS:
		The function find_keyword looks for any part of the substring
		That means that if we have to params called "final" and
		"final sectors" and "final" is the first word in the list of
		known params then "final sectors" will be interpretated as
		"final"
*/

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <boot.h>

#define VERSION_MAJOR	0x00
#define VERSION_MINOR	0x31

unsigned char				*buffer;
char						*boot_filename;
char						*loader_filename;
char						*kernel_filename;
char						*output_filename;
unsigned long				output_sectors;
unsigned long				*kernel_LBA;
unsigned short				*kernel_sectors;

struct boot_header			boot_setup;

/* Keywords for the options in boot.cfg */
char					*option_keywords[] =
						{
							"boot",
							"loader",
							"kernel",
							"final",
							"image sectors",
							"heads",
							"sectors",
							"bytes per sector",
							NULL
						};
						
unsigned char			keywords= 6;


void print_welcome(void)
{
	printf("MKIMAGE the aMOS boot configuration tool\n");
	printf("V0.03.01 by ObliVion\n\n");
}

void die(void)
{
	fprintf(stderr, "Error detected dying...\n");
	exit(1);
}

unsigned int file_size(FILE *fptr)
{
	unsigned int size, position;

	position=ftell(fptr);
	fseek(fptr, 0, SEEK_END);
	size=ftell(fptr);
	fseek(fptr, position, SEEK_SET);

	return(size);
}

FILE	*xopen(char *filename, char *mode)
{
	FILE		*fptr;
	
	fptr=fopen(filename, mode);
	
	if (fptr==NULL)
	{
		fprintf(stderr, "\nXOPEN: error opening %s\n", filename);
		die();
	}
	return(fptr);
}

void	*xmalloc(int size)
{
	void		*ptr;
	
	ptr=(void *)malloc(size);
	
	if (ptr==NULL)
	{
		fprintf(stderr, "\nXMALLOC: error allocating %d bytes\n", size);
		die();
	}
	memset(ptr, 0, size);
	
	return(ptr);
}

int		find_keyword(char *line)
{
	unsigned long	i;
	
	i=0;
	
	while (option_keywords[i]!=NULL)
	{
		if (strstr(line, option_keywords[i])!=NULL)
			return(i);
				
		i++;
	}
	
	return(-1);
}

char	*parse_filename(char *line)
{
	char			*where;
	char			*filename;
	
	where=strchr(line, '=');
	
	if (where==NULL)
	{
		fprintf(stderr, "\nPARSE_FILENAME: no = character found in option\n");
		die();
	}
	filename=(char *)xmalloc(strlen(where));
	
	where++;
	strcpy(filename, where);
	
	if (filename[strlen(filename)-1]==10)
		filename[strlen(filename)-1]=0;
	
	return(filename);
}

unsigned long	parse_ulong(char *line)
{
	char			*where;
	
	where=strchr(line, '=');
	
	if (where==NULL)
	{
		fprintf(stderr, "\nPARSE_FILENAME: no = character found in option\n");
		exit(1);
	}
	
	where++;
	
	return(atol(where));
}

void	parse_config(FILE *fptr)
{
	char 			*line;
	unsigned char	keywords_found;
	
	
	line=(char *)xmalloc(512);
	keywords_found=0;
	
	while (!feof(fptr))
	{
		fgets(line, 512, fptr);
		
		if (isalnum(line[0]))
		{
			switch(find_keyword(line))
			{
				case	-1:	fprintf(stderr, "\nPARSE_CONFIG: unknown option: %s\n", line);
							break;
							
				case	0:	boot_filename=parse_filename(line);
							printf("    Boot image:                      %s\n", boot_filename);
							keywords_found++;
							break;
						
				case	1:	loader_filename=parse_filename(line);
							printf("    Kernel loader image:             %s\n", loader_filename);			
							keywords_found++;
							break;

				case	2:	kernel_filename=parse_filename(line);
							printf("    Kernel image:                    %s\n", kernel_filename);			
							keywords_found++;
							break;
						
				case	3:	output_filename=parse_filename(line);
							printf("    Output image:                    %s\n", output_filename);
							keywords_found++;
							break;

				case	4:	output_sectors=parse_ulong(line);
							printf("    Output sectors:                  %lu\n", output_sectors);
							keywords_found++;
							break;

						
//				case	5:	boot_setup.heads=parse_int(line);
//							printf("    Number of heads:                 %d\n", boot_setup.heads);
//							keywords_found++;
//							break;
						
//				case	6:	boot_setup.sectors=parse_int(line);
//							printf("    Sectors per track:               %d\n", boot_setup.sectors);
//							keywords_found++;
//							break;
						
				case	7:	boot_setup.bytes_per_sector=parse_ulong(line);
							printf("    Bytes per sector:                %d\n", boot_setup.bytes_per_sector);
							keywords_found++;
							break;	
						
				default:	fprintf(stderr, "\nPARSE_CONFIG: unknown option: %s", line);
							break;
			}	
		}
	}
	

	free(line);	
	printf("\n");
	if (keywords_found!=keywords)
	{
		fprintf(stderr, "\nPARSE_CONFIG: missing options in boot.cfg\n");
		die();
	}		
}

unsigned long	byte_to_sector(unsigned long byte_offset)
{
	unsigned long	tmp;

	tmp=byte_offset/boot_setup.bytes_per_sector;
	
	if ((byte_offset%boot_setup.bytes_per_sector)>0)
		tmp+=1;
	
	return(tmp);
}

int main(int argc, char *argv[])
{
	FILE			*config_file, *boot_file, *loader_file, *kernel_file, *output_file;
	unsigned long	size, total_size, i;
	unsigned char	padding = 0;
	
	buffer=(unsigned char *)xmalloc(65535);		/* This sets the limit of bootsector + loader
												   to 64K which i should be enough. complains?*/

	print_welcome();

	if (argc>1 && !strcmp(argv[1], "-p"))
		padding=1;
	
	/* Open and parse config file */
	config_file=xopen("boot.cfg", "rt");
	printf("Reading configuration:\n");
	parse_config(config_file);
	
	/* Open boot image and read it into the buffer */
	boot_file=xopen(boot_filename, "rb");
	printf("Reading boot image: %s\n",boot_filename);
	size=file_size(boot_file);
	if (size>512)
	{
		fprintf(stderr, "\nMAIN: boot image must be exactly 512 bytes\n");
		die();
	}
	fread(buffer, size, 1, boot_file);
	if (!((buffer[510]==0x55) && (buffer[511]==0xAA)))
	{
		fprintf(stderr, "\nMAIN: No boot signature (0x55AA) found\n");
		die();
	}
	boot_setup.boot_sectors=byte_to_sector(size);
	total_size=size;
	fclose(boot_file);

	boot_setup.loader_start=byte_to_sector(total_size);

	/* Open loader image and read it into the buffer */
	loader_file=xopen(loader_filename, "rb");
	printf("Reading kernel loader image: %s\n", loader_filename);
	size=file_size(loader_file);
	fread(&buffer[total_size], size, 1, loader_file);
	fclose(loader_file);
	
	total_size+=size;
	boot_setup.loader_sectors=byte_to_sector(size);
	boot_setup.boot_drive=0;
	boot_setup.kernel_start=byte_to_sector(total_size);

	memset(&buffer[total_size], 0, (boot_setup.kernel_start*512)-total_size);
	total_size=boot_setup.kernel_start*512;

	/* Open kernel image and read it into the buffer */
	kernel_file=xopen(kernel_filename, "rb");
	printf("Reading kernel image: %s\n", kernel_filename);
	size=file_size(kernel_file);
	fread(&buffer[total_size], size, 1, loader_file);
	fclose(kernel_file);

	total_size+=size;

	boot_setup.loader_start++;
	boot_setup.kernel_start++;
	boot_setup.kernel_sectors=byte_to_sector(size);

	printf("\nSaving configuration:\n");
	memcpy(&buffer[3], &boot_setup, sizeof(boot_setup));
/*	printf("    Number of heads:                 %d\n", boot_setup.heads); */
/*	printf("    Sectors per track:               %d\n", boot_setup.sectors); */
	printf("    Bytes per sector:                %hu\n", boot_setup.bytes_per_sector);
	printf("    Boot sectors                     %hhu\n", boot_setup.boot_sectors);
	printf("    Kernel loader start at sector:   %lu\n", boot_setup.loader_start);
	printf("    Kernel loader sectors:           %lu\n", boot_setup.loader_sectors);	
	printf("    Kernel start at sector:          %lu\n", boot_setup.kernel_start);
	printf("    Kernel sectors                   %lu\n", boot_setup.kernel_sectors);
	
	printf("Writing %lu bytes to %s\n", total_size, output_filename);
	output_file=xopen(output_filename, "wb");
	fwrite(buffer, 	total_size, 1, output_file);

	if (padding)
	{
		printf("Padding to %lu bytes\n", (output_sectors*512));
		for(i=total_size; i<output_sectors*512; i++)
			fputc(0, output_file);
	}

	fclose(output_file);
	free(buffer);

	printf("That did'nt hurt too much did it?\n");
	
	return 0;
}

