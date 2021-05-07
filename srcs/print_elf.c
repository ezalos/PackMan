/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 17:13:08 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/07 11:58:32 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void print_section_header(t_packer *packer, Elf64_Shdr *shdr)
{
    printf("%s%-10s%s ", _YELLOW, get_sec_name(packer, shdr), _RESET);
	printf("offset: %s%-8lu%s ", _BLUE, shdr->sh_offset, _RESET);
	printf("size: %s%-8lu%s ", _BLUE, shdr->sh_size, _RESET);
	printf("info %3u|%-3lu flags \t", shdr->sh_info, shdr->sh_flags);
    printf("addr: %p\n", (void *)shdr->sh_addr);
    // printf("\n");
}

void    print_program_header(Elf64_Phdr *phdr)
{
    printf("SEGMENT: ");// 9
	if (phdr->p_type & PT_LOAD)
		printf("%s%s%s ", _GREEN, " Load ", _RESET);
	else
		printf("%s%s%s ", _RED, "NoLoad", _RESET);// 16
	printf("%s%c%c%c%s\n", _YELLOW, phdr->p_flags & PF_R ? 'R' : '-', phdr->p_flags & PF_W ? 'W' : '-', phdr->p_flags & PF_X ? 'X' : '-', _RESET);
	printf("%10c", ' ');
	printf("offset: %s%-10lu%s ", _YELLOW, phdr->p_offset, _RESET);
    printf("filesz: %-10lu\n", phdr->p_filesz);
	printf("%10c", ' ');
	printf("vaddr : %-10p ", (void *)phdr->p_vaddr);
	printf("memsz : %s%-10lu%s\n", _YELLOW, phdr->p_memsz, _RESET);
	printf("%10c", ' ');
	printf("paddr : %-10p ", (void *)phdr->p_paddr);
	printf("align : %-10lu\n", phdr->p_align);
    printf("\n");
}

void    print_elf_header(Elf64_Ehdr *elf)
{
    printf("---- Elf Header ----\n");
    printf("e_ident: %s\n", elf->e_ident);
    printf("\tEI_MAG %02x\n", elf->e_ident[EI_MAG0]);
    printf("\tEI_MAG %c\n", elf->e_ident[EI_MAG1]);
    printf("\tEI_MAG %c\n", elf->e_ident[EI_MAG2]);
    printf("\tEI_MAG %c\n", elf->e_ident[EI_MAG3]);
    printf("\tEI_CLASS %d -> %s\n", elf->e_ident[EI_CLASS], elf->e_ident[EI_CLASS] == 1 ? "32bits" : "64bits");
    printf("\tEI_DATA %d -> %s \n", elf->e_ident[EI_DATA], elf->e_ident[EI_DATA] == 1 ? "little endian" : "big endian");

    printf("e_entry: 0x%lx\n", elf->e_entry);
    printf("e_phoff: %ld\n", elf->e_phoff);
    printf("e_phentsize: %d\n", elf->e_phentsize);
    printf("e_phnum: %d\n", elf->e_phnum);
    printf("e_shentsize: %d\n", elf->e_shentsize);
    printf("e_shnum: %d\n", elf->e_shnum);
    printf("e_shoff: %ld\n", elf->e_shoff);
    printf("e_shstrndx: %d\n", elf->e_shstrndx);
    printf("\n");
}