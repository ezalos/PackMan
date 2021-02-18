/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_elf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 17:12:56 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/18 16:03:44 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

char *get_sec_name(t_packer *packer, Elf64_Shdr *shdr)
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

Elf64_Shdr *get_section_header(t_packer *packer, uint32_t index)
{
    Elf64_Ehdr *elf;
    Elf64_Shdr *shdr;

    elf = (Elf64_Ehdr *)packer->content;
    shdr = (Elf64_Shdr *)((size_t)packer->content + (size_t)elf->e_shoff + ((size_t)index * (size_t)elf->e_shentsize));

    if (sizeof(Elf64_Shdr) + (size_t)shdr >= (size_t)packer->content + packer->size)
        shdr = NULL;

    return shdr;
}

Elf64_Phdr *get_program_header(t_packer *packer, uint32_t index)
{
    Elf64_Ehdr *elf;
    Elf64_Phdr *phdr;

    elf = (Elf64_Ehdr *)packer->content;
	phdr = (Elf64_Phdr *)((size_t)packer->content + (size_t)elf->e_phoff + ((size_t)index * (size_t)elf->e_phentsize));

	if (sizeof(Elf64_Phdr) + (size_t)phdr >= (size_t)packer->content + packer->size)
        phdr = NULL;

    return phdr;
}