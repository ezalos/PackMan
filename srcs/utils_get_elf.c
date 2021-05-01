/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_get_elf.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 17:12:56 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/01 18:43:20 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

char		*get_sec_name(t_packer *packer, Elf64_Shdr *shdr)
{
    Elf64_Ehdr *elf;
    Elf64_Shdr *shdr_tab;
    char *sec_names;

    elf = (Elf64_Ehdr *)packer->content;
    shdr_tab = (Elf64_Shdr *)(packer->content + elf->e_shoff);
    sec_names = (char*)(packer->content + shdr_tab[elf->e_shstrndx].sh_offset);
    if (shdr)
        return (&sec_names[shdr->sh_name]);
    return NULL;
}

Elf64_Shdr	*get_section_header(t_packer *packer, uint32_t index)
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

int 		get_program_header_index(t_packer *packer, Elf64_Phdr *phdr)
{
	Elf64_Ehdr *elf = (Elf64_Ehdr *)packer->content;
	if (!phdr)
		return -1;
	return (((size_t)phdr) - ((size_t)packer->content + (size_t)elf->e_phoff)) / (size_t)elf->e_phentsize;
}

Elf64_Phdr *get_program_header(t_packer *packer, uint32_t index)
{
    Elf64_Ehdr *elf;
    Elf64_Phdr *phdr;

    elf = (Elf64_Ehdr *)packer->content;
	phdr = (Elf64_Phdr *)((size_t)packer->content + (size_t)elf->e_phoff + ((size_t)index * (size_t)elf->e_phentsize));

	// printf("Phdr start: %p\t end: %p\n", phdr, (void*)(sizeof(Elf64_Phdr) + (size_t)phdr));
	// printf("File start: %p\t end: %p\n", packer->content, (void*)((size_t)packer->content + packer->size));
	if (index >= elf->e_phnum)
        phdr = NULL;

    return phdr;
}

