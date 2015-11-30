/*		elf.c

		Routines for handling the ELF file format

	History:

		Version 0.01.00		12 Jan 2002		08:50	ObliVion
		Initial version.

	Todo:
		Remember to deallocate all dynamic memory
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "elf.h"

int elf_printf(struct elf_data *elf, const char *format,...);
int elf_fread(void *buffer, size_t size, size_t n_records, FILE *stream);
int elf_read_header(struct elf_data *elf);
int elf_read_program_header(struct elf_data *elf);

int elf_printf(struct elf_data *elf, const char *format,...)
{
	va_list         va;
	int				ret = 0;

	if (elf->verbose)
	{
		va_start(va, format);
		ret = vprintf(format, va);
		va_end(va);
	}

	return(ret);
}

int elf_fread(void *buffer, size_t size, size_t n_records, FILE *stream)
{
	if(fread(buffer, size, n_records, stream))
	{
		if (feof(stream))
			return(ERR_ELF_FILE_PREMATURE_EOF);
		if (ferror(stream))
			return(ERR_ELF_FILE_READ);
	}

	return(0);
}

int elf_read_header(struct elf_data *elf)
{
	int				ret = 0;

	elf_printf(elf, "Reading ELF header...\n");

	ret = elf_fread(&elf->header, sizeof(elf->header), 1, elf->file);
	if (ret)
		return(ERR_ELF_FILE_READ);

	if ((elf->header.e_ident[EI_MAG0] != ELFMAG0) || (elf->header.e_ident[EI_MAG1] != ELFMAG1) || (elf->header.e_ident[EI_MAG2] != ELFMAG2) || (elf->header.e_ident[EI_MAG3] != ELFMAG3))
		return(ERR_ELF_INCOMPATIBLE);

	return(0);
}

int elf_read_program_header(struct elf_data *elf)
{
	int				ret = 0;
	unsigned long	i;

	if (elf->header.e_phnum)
	{
		elf_printf(elf, "Reading %d ELF program headers...\n", elf->header.e_phnum);

		elf->phtab = (Elf32_Phdr *) malloc(elf->header.e_phentsize * elf->header.e_phnum);

		if (!elf->phtab)
			return(ERR_ELF_MEMORY_ALLOCATION);

		fseek(elf->file, elf->header.e_phoff, SEEK_SET);
		for (i = 0; i < elf->header.e_phnum; i++)
		{
			ret = elf_fread(&elf->phtab[i], elf->header.e_phentsize, 1, elf->file);
			if(ret)
				return(ret);
		}
	}
	else
		elf_printf(elf, "Skipping program headers as the table is empty...\n");

	return(0);
}

int elf_read_section_header(struct elf_data *elf)
{
	int				ret = 0;
	unsigned long	i;
	char			*tempstrtab;

	if (elf->header.e_shnum)
	{
		elf_printf(elf, "Reading %d ELF section headers...\n", elf->header.e_shnum);

		elf->shtab = (Elf32_Shdr *) malloc(elf->header.e_shentsize * elf->header.e_shnum);

		if (!elf->shtab)
			return(ERR_ELF_MEMORY_ALLOCATION);

		fseek(elf->file, elf->header.e_shoff, SEEK_SET);
		for (i = 0; i < elf->header.e_shnum; i++)
		{
			ret = elf_fread(&elf->shtab[i], elf->header.e_shentsize, 1, elf->file);
			if (ret)
				return(ret);
		}

		elf_printf(elf, "Reading ELF section strings...\n");

		tempstrtab = (char *)malloc(elf->shtab[elf->header.e_shstrndx].sh_size);

		fseek(elf->file, elf->shtab[elf->header.e_shstrndx].sh_offset, SEEK_SET);
		ret = elf_fread(tempstrtab, elf->shtab[elf->header.e_shstrndx].sh_size, 1, elf->file);
		if (ret)
			return(ret);

		elf->shstrtab = (char **)malloc(elf->header.e_shnum * sizeof(char *));
		for (i = 0; i < elf->header.e_shnum; i++)
		{
			elf->shstrtab[i] = (char *)malloc(strlen(&tempstrtab[elf->shtab[i].sh_name]) * sizeof(char));
			strcpy(elf->shstrtab[i], &tempstrtab[elf->shtab[i].sh_name]);
		}
	}
	else
		elf_printf(elf, "Skipping section headers as the table is empty...\n");

	return(0);
}

int elf_load(struct elf_data *elf)
{
	int				ret = 0;

	elf_printf(elf, "Opening ELF file %s...\n", elf->filename);
	elf->file = fopen(elf->filename, "r");

	if (!elf->file)
		return(ERR_ELF_FILE_OPEN);

	ret = elf_read_header(elf);
	if(ret)
		return(ret);

	ret = elf_read_program_header(elf);
	if(ret)
		return(ret);

	ret = elf_read_section_header(elf);
	if(ret)
		return(ret);

	return(0);
}

Elf32_Half elf_get_type(struct elf_data *elf)
{
	return(elf->header.e_type);
}

void elf_print_header(struct elf_data *elf)
{
	unsigned long		i;

	printf("\nELF Header:\n");

	printf("    Magic:                           ");
	for(i = 0; i < sizeof(elf->header.e_ident); i++)
		printf("0x%02x ", elf->header.e_ident[i]);
	printf("\n");

	printf("    Class:                           ");
	switch(elf->header.e_ident[EI_CLASS])
	{
		case ELFCLASSNONE:	printf("Invalid class\n");
							break;

		case ELFCLASS32:	printf("32-bit object\n");
							break;

		case ELFCLASS64:	printf("64-bit object\n");
							break;

		default:			printf("unknown\n");
							break;
	}

	printf("    Data encoding:                   ");
	switch(elf->header.e_ident[EI_DATA])
	{
		case ELFDATANONE:	printf("Invalid data encoding\n");
							break;

		case ELFDATA2LSB:	printf("2's complement, little endian\n");
							break;

		case ELFDATA2MSB:	printf("2's complement, big endian\n");
							break;

		default:			printf("unknown\n");
							break;
	}


	printf("    Type:                            ");
	switch(elf->header.e_type)
	{

		case ET_NONE:	printf("No type\n");
						break;

		case ET_REL:	printf("Relocatable\n");
						break;

		case ET_EXEC:	printf("Executable\n");
						break;

		case ET_DYN:	printf("Shared object\n");
						break;

		case ET_CORE:	printf("Core\n");
						break;

		default:		if ((elf->header.e_type >= ET_LOPROC) && (elf->header.e_type <= ET_HIPROC))
							printf("Processor specific\n");
						else
							printf("Unknown\n");;
						break;
	}

	printf("    Machine:                         ");
	switch(elf->header.e_machine)
	{
		case EM_NONE:	printf("none\n");
						break;

		case EM_M32:	printf("AT&T WE32100\n");
						break;

		case EM_SPARC:	printf("Sparc\n");
						break;

		case EM_386:	printf("Intel 80386\n");
						break;

		case EM_68K:	printf("Motorola 68000\n");
						break;

		case EM_88K:	printf("Motorola 88000\n");
						break;

		case EM_860:	printf("Intel 80860\n");
						break;

		case EM_MIPS:	printf("MIPS RS3000\n");;
						break;

		default:		printf("unknown\n");
						break;
	}

	printf("    Version:                         %d", elf->header.e_version);
	if (elf->header.e_version == EV_NONE)
		printf(" (Invalid version)\n");
	if (elf->header.e_version == EV_CURRENT)
		printf(" (Current version)\n");
	else
		printf("\n");

	printf("    Entry:                           0x%x\n", elf->header.e_entry);

	printf("    Program header table offset:     0x%x\n", elf->header.e_phoff);

	printf("    Section header table offset:     0x%x\n", elf->header.e_shoff);

	printf("    Processor specific flags:        0x%x\n", elf->header.e_flags);

	printf("    ELF header size:                 %d\n", elf->header.e_ehsize);

	printf("    Program header entry size:       %d\n", elf->header.e_phentsize);

	printf("    Entries in the program header:   %d\n", elf->header.e_phnum);

	printf("    Section header entry size:       %d\n", elf->header.e_shentsize);

	printf("    Entries in the section header:   %d\n", elf->header.e_shnum);

}

void elf_print_sections(struct elf_data *elf)
{
	unsigned long		i;

	printf("\nELF Sections:\n");

	printf("    Nr.:  Name:              Type:\n");

	for(i = 0; i < elf->header.e_shnum; i++)
	{
		printf("    %02d", i);
		printf("    %-19s", elf->shstrtab[i]);

		switch(elf->shtab[i].sh_type)
		{
			case SHT_NULL:		printf("NULL");
								break;

			case SHT_PROGBITS:	printf("PROGBITS");
								break;

			case SHT_SYMTAB:	printf("SYMTAB");
								break;

			case SHT_DYNSYM:	printf("DYNSYM");
								break;

			case SHT_STRTAB:	printf("STRTAB");
								break;

			case SHT_RELA:		printf("RELA");
								break;

			default:			printf("unknown");
								break;
		}

		printf("\n");
	}
}
