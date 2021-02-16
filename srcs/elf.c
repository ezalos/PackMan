/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 13:17:41 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/16 14:55:57 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"


void		print_symbol_code(void *data, size_t offset, size_t size)
{
	size_t	len = 16;
	size_t	i;

	i = 0;
	while (i < size)
	{
		if (i % len == 0)
			printf("\t0x%08lx\t", i + offset);
		printf("%02hhx ", ((uint8_t*)data)[i + offset]);
		if ((i + 1) % len == 0)
			printf("\n");
		i++;
	}
	if (i)
		printf("\n");
}

void		*elf_access_section(void *data, char *section_name, int umpteenth)
{
	// Elf64_Shdr
	// A file's section header table lets one locate all the file's sections.
	//	 The section header table is an array of Elf32_Shdr or Elf64_Shdr structures.
	//	 The ELF header's e_shoff member gives the byte offset from the beginning of the file to the section header table.
	//	 e_shnum holds the number of entries the section header table contains. e_shentsize holds the size in bytes of each entry.
	//	 A section header table index is a subscript into this array.
	Elf64_Ehdr		*elf = data;
	int				i;

	// e_shoff
	// This member holds the section header table's file offset in bytes.
	Elf64_Shdr      *section_header = (Elf64_Shdr *) (data + elf->e_shoff); //section names is after data

	// sh_offset
	// 	This member's value holds the byte offset from the beginning of the file to the first byte in the section.
	// e_shstrndx
	//	This member holds the section header table index of the entry associated with the section name string table.
	char            *section_names = (char *) (data + section_header[elf->e_shstrndx].sh_offset);

	// Various sections hold program and control information:
	// we are looking through section names to locate the section of interest (symbol table)

	Elf64_Shdr		*section = NULL;

	i = -1;
	while (++i < elf->e_shnum) // number of entries in the section header table
	{
		if (section_header[i].sh_size) // holds the section's size in bytes
		{
    		if (strcmp(&section_names[section_header[i].sh_name], section_name) == 0)
			{
				if (umpteenth == 0)
				{
	      			section = (Elf64_Shdr *) &section_header[i];
					break ;
				}
				else
					umpteenth--;
			}
  		}
	}
	return (section);

}

uint8_t		crypt_condition(void *data, Elf64_Sym *sym)
{
	uint8_t		retval;
	Elf64_Ehdr	*elf = data;
	Elf64_Shdr  *section_header = (Elf64_Shdr *) (data + elf->e_shoff); //section names is after data
	char        *section_names = (char *) (data + section_header[elf->e_shstrndx].sh_offset);
	Elf64_Shdr  *strtab = elf_access_section(data, ".strtab", 0);
	char		*symbol_names = (char*) (data + strtab->sh_offset);

	retval = FALSE;
	if (sym->st_info != STT_SECTION
	&& sym->st_info != STT_FILE)
	{
		if (0 == strcmp(&section_names[section_header[sym->st_shndx].sh_name], ".text")
		&& (symbol_names + sym->st_name)[0] != '_')
			retval = TRUE;
	}
	return retval;
}

// fonction qui iter sur tous les segement/programme header
// fonction qui iter sur tous les section header

void		new_try(Elf64_Ehdr *elf, void *data, t_packer *packer)
{
	// Elf64_Shdr
	// A file's section header table lets one locate all the file's sections.
	//	 The section header table is an array of Elf32_Shdr or Elf64_Shdr structures.
	//	 The ELF header's e_shoff member gives the byte offset from the beginning of the file to the section header table.
	//	 e_shnum holds the number of entries the section header table contains. e_shentsize holds the size in bytes of each entry.
	//	 A section header table index is a subscript into this array.
	Elf64_Shdr      *symtab = elf_access_section(data, ".symtab", 0);
	// Elf64_Shdr      *shstrtab = elf_access_section(data, ".shstrtab", 0);
	Elf64_Shdr      *strtab = elf_access_section(data, ".strtab", 0);

	int				i;

	// e_shoff
	// This member holds the section header table's file offset in bytes.
	Elf64_Shdr      *section_header = (Elf64_Shdr *) (data + elf->e_shoff); //section names is after data

	// sh_offset
	// 	This member's value holds the byte offset from the beginning of the file to the first byte in the section.
	// e_shstrndx
	//	This member holds the section header table index of the entry associated with the section name string table.
	char            *section_names = (char *) (data + section_header[elf->e_shstrndx].sh_offset);
	// (void)shstrtab;

	// Elf64_Sym
	// 	An object file's symbol table holds information needed to locate and relocate a program's symbolic definitions and references.
	// 	A symbol table index is a subscript into this array.
	//		st_name : holds offset to read in
	Elf64_Sym	*sym = (Elf64_Sym*) (data + symtab->sh_offset);
	char		*symbol_names = (char*) (data + strtab->sh_offset);
	void 		*symbol_data;

	(void)symbol_data;
	(void)packer;
	i = -1;
	while ((size_t)++i < symtab->sh_size / symtab->sh_entsize) /* was 'symtab->sh_size / sizeof(Elf64_Sym)' before */
	{
		if (TRUE == crypt_condition(data, &sym[i]))
		{
				symbol_data = (void*)(&section_header[sym[i].st_shndx] + sym[i].st_value);
				// if (sym[i].st_value)
				// 	printf("%016lx ", sym[i].st_value);// not good yet
				// else
				// 	printf("%16c ", ' ');// not good yet
				printf("Section name should be: %s\n", &section_names[section_header[sym[i].st_shndx].sh_name]);
				printf("Section size should be: %zu\n", section_header[sym[i].st_shndx].sh_size);
				printf("Name of current symbol: %s\n", symbol_names + sym[i].st_name);
				if (sym[i].st_value + sym[i].st_size < (unsigned int)packer->size)
				{
					printf("Code associated with current section: \n");
					print_symbol_code(data, sym[i].st_value, sym[i].st_size);
				}
				printf("\n");
		}

	}
}

// My output du binaire HelloWord
//         0x00001149      f3 0f 1e fa 55 48 89 e5 48 8d 3d ac 0e 00 00 e8 
//         0x00001159      f3 fe ff ff b8 00 00 00 00 5d c3 

// Celuio de objdump
// 0000000000001149 <main>:
//     1149:       f3 0f 1e fa             endbr64 
//     114d:       55                      push   rbp
//     114e:       48 89 e5                mov    rbp,rsp
//     1151:       48 8d 3d ac 0e 00 00    lea    rdi,[rip+0xeac]        # 2004 <_IO_stdin_used+0x4>
//     1158:       e8 f3 fe ff ff          call   1050 <puts@plt>
//     115d:       b8 00 00 00 00          mov    eax,0x0
//     1162:       5d                      pop    rbp
//     1163:       c3                      ret    
//     1164:       66 2e 0f 1f 84 00 00    nop    WORD PTR cs:[rax+rax*1+0x0]
//     116b:       00 00 00 
//     116e:       66 90                   xchg   ax,ax