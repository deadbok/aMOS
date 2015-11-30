/*		elf.h

		Include file for the elf file format handling routines

	History:

		Version 0.01.00		12 Jan 2002		08:50	ObliVion
		Initial version.
*/

#ifndef _AMOS_TOOLS_ELF_
#define _AMOS_TOOLS_ELF_

#include <stdio.h>
#include <elf.h>

#define ERR_ELF_FILE_OPEN				1
#define ERR_ELF_FILE_PREMATURE_EOF		2
#define ERR_ELF_FILE_READ				3
#define ERR_ELF_INCOMPATIBLE			4
#define ERR_ELF_MEMORY_ALLOCATION		5

struct elf_data
{
	char			*filename;
	FILE			*file;
	char			verbose;

	Elf32_Ehdr		header;
	Elf32_Phdr		*phtab;
	Elf32_Shdr		*shtab;
	Elf32_Sym		*symtab;
	Elf32_Rel		*roDataReloc;
	Elf32_Rel		*dataReloc;
	Elf32_Rel		*codeReloc;
	char			*strtab;
	char			**shstrtab;
	unsigned int	status;
	unsigned int	nSymb;
	unsigned int	sizeStr;
	unsigned int	nDataReloc;
	unsigned int	nRODataReloc;
	unsigned int	nCodeReloc;

};

extern int elf_load(struct elf_data *elf);

extern Elf32_Half elf_get_type(struct elf_data *elf);
extern void elf_print_header(struct elf_data *elf);
extern void elf_print_sections(struct elf_data *elf);

#endif


