/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 13:17:41 by ezalos            #+#    #+#             */
/*   Updated: 2020/12/05 12:57:20 by ezalos           ###   ########.fr       */
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

void		new_try(Elf64_Ehdr *elf, void *data, t_packer *packer)
{
	// Elf64_Shdr
	// A file's section header table lets one locate all the file's sections.
	//	 The section header table is an array of Elf32_Shdr or Elf64_Shdr structures.
	//	 The ELF header's e_shoff member gives the byte offset from the beginning of the file to the section header table.
	//	 e_shnum holds the number of entries the section header table contains. e_shentsize holds the size in bytes of each entry.
	//	 A section header table index is a subscript into this array.
	Elf64_Shdr      *symtab = elf_access_section(data, ".symtab", 0);
	Elf64_Shdr      *shstrtab = elf_access_section(data, ".shstrtab", 0);
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
	(void)shstrtab;

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
		if (sym[i].st_info != STT_SECTION
		&& sym[i].st_info != STT_FILE)
		{
			if (0 == strcmp(&section_names[section_header[sym[i].st_shndx].sh_name], ".text")
			&& (symbol_names + sym[i].st_name)[0] != '_')
			{
				symbol_data = (void*)(&section_header[sym[i].st_shndx] + sym[i].st_value);
				if (sym[i].st_value)
					printf("%016lx ", sym[i].st_value);// not good yet
				else
					printf("%16c ", ' ');// not good yet
				// printf("%c ", print_type_stack(sym[i], &section_header[sym[i].st_shndx]));

				// if (strcmp(symbol_names + sym[i].st_name, "data_start") == 0)
				{
					// print_st_info(sym[i]);
					printf("%s ", &section_names[section_header[sym[i].st_shndx].sh_name]);
				}
		 		printf("%s\n", symbol_names + sym[i].st_name);
				if (1 && sym[i].st_value + sym[i].st_size < (unsigned int)packer->stat.st_size)
					print_symbol_code(data, sym[i].st_value, sym[i].st_size);
			}
		}

	}
}
