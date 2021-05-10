/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_get_elf.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 17:12:56 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/10 09:43:20 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

char		*get_sec_name(t_packer *packer, Elf64_Shdr *shdr)
{
    Elf64_Ehdr *elf;
    Elf64_Shdr *shdr_tab;
    char *sec_names;

	if (!shdr)
		return (NULL);
    elf = (Elf64_Ehdr *)packer->content;
	if (FALSE == is_secure_access(packer->size, (uint64_t)elf->e_shoff
			+ (uint64_t)(elf->e_shstrndx) * (uint64_t)(elf->e_shentsize),
		sizeof(Elf64_Shdr)))
		return (NULL);
    shdr_tab = (Elf64_Shdr *)(packer->content + elf->e_shoff);
	if (FALSE == is_secure_read(packer->content, packer->size,
		(uint64_t)(shdr_tab[elf->e_shstrndx].sh_offset)
			+ (uint64_t)(shdr->sh_name)))
		return (NULL);
    sec_names = (char*)(packer->content + shdr_tab[elf->e_shstrndx].sh_offset);
  	return (&sec_names[shdr->sh_name]);
}

Elf64_Shdr		*get_section_header(t_packer *packer, uint32_t index)
{
    Elf64_Ehdr *elf;
    Elf64_Shdr *shdr;

    elf = (Elf64_Ehdr *)packer->content;
    shdr = (Elf64_Shdr *)((size_t)packer->content + (size_t)elf->e_shoff + ((size_t)index * (size_t)elf->e_shentsize));

	if (index >= elf->e_shnum)
		shdr = NULL;

    return shdr;
}

int			get_section_header_index(t_packer *packer, Elf64_Shdr *shdr)
{
	Elf64_Ehdr *elf = (Elf64_Ehdr *)packer->content;
	if (!shdr)
		return -1;
	return (((size_t)shdr) - ((size_t)packer->content + (size_t)elf->e_shoff)) / (size_t)elf->e_shentsize;
}

int			get_program_header_index(t_packer *packer, Elf64_Phdr *phdr)
{
	Elf64_Ehdr *elf = (Elf64_Ehdr *)packer->content;
	if (!phdr)
		return -1;
	return (((size_t)phdr) - ((size_t)packer->content + (size_t)elf->e_phoff)) / (size_t)elf->e_phentsize;
}

Elf64_Phdr	*get_program_header(t_packer *packer, uint32_t index)
{
    Elf64_Ehdr *elf;
    Elf64_Phdr *phdr;

    elf = (Elf64_Ehdr *)packer->content;
	phdr = (Elf64_Phdr *)((size_t)packer->content + (size_t)elf->e_phoff + ((size_t)index * (size_t)elf->e_phentsize));

	if (index >= elf->e_phnum)
        phdr = NULL;

    return phdr;
}

Elf64_Phdr	*get_phdr_from_array(t_packer *packer, int i)
{
	if (packer->phdr_array[i])
		return packer->phdr_array[i]->phdr;
	return NULL;
}

t_pheader	*find_t_pheader_from_phdr(t_packer *packer, Elf64_Phdr *a)
{
	t_pheader	*the_choosen_one;
	int i;

	the_choosen_one = NULL;
	if (packer->phdr_array)
	{
		i = -1;
		while (packer->phdr_array[++i])
			if (packer->phdr_array[i]->phdr == a)
				the_choosen_one = packer->phdr_array[i];
	}
	return (the_choosen_one);
}