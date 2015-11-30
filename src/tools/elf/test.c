#include <stdio.h>
#include "elf.h"

int main(int argc, char *argv[])
{
	struct elf_data		elf;
	int					ret;

	printf("\n");

	if (argc < 2)
	{
		printf("Please specify an ELF file on the command-line\n\n");
		printf("    test \"elffile\"\n");
		printf("    where \"elffile\" is the filename of the ELF file to investigate\n");
	}

	elf.filename = argv[1];
	elf.verbose = 1;

	ret = elf_load(&elf);
	if (ret)
	{
		switch(ret)
		{
			case ERR_ELF_FILE_OPEN:				printf("File open failed\n");
												break;

			case ERR_ELF_FILE_PREMATURE_EOF:	printf("File open failed\n");
												break;

			case ERR_ELF_FILE_READ:				printf("File open failed\n");
												break;

			case ERR_ELF_INCOMPATIBLE:			printf("Incompatibke file\n");
												break;

			case ERR_ELF_MEMORY_ALLOCATION:		printf("Memory allocation error\n");
												break;

			default:							printf("Unknown error: %d\n", ret);
												break;
		}
		return(ret);
	}

	elf_print_header(&elf);

	elf_print_sections(&elf);

	return(0);
}
