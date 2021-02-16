/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 17:13:08 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/16 19:02:22 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void print_section_header(t_packer *packer, Elf64_Shdr *shdr)
{
    printf("---- SECTION----\n");
    printf("sh_size: %lu\n", shdr->sh_size);
    printf("sh_offset: %lu\n", shdr->sh_offset);
    printf("sh_name: %s\n", get_sec_name(packer, shdr));
    printf("sh_info: %u\n", shdr->sh_info);
    printf("sh_flags: %lu\n", shdr->sh_flags);
    printf("sh_addr: %p\n", (void *)shdr->sh_addr);
    printf("\n");
}

void    print_program_header(Elf64_Phdr *phdr)
{
    printf("---- SEGMENT:----\n");
    printf("p_type  : %u\t %s\n", phdr->p_type, phdr->p_type == PT_LOAD ? "Load" : "");
    printf("p_flags : %u\n\tread %d\n\twrite %d\n\texec %d\n", phdr->p_flags, phdr->p_flags & PF_R, phdr->p_flags & PF_W, phdr->p_flags & PF_X);
    printf("p_offset: %lu\n", phdr->p_offset);
    printf("p_vaddr : %p\n", (void *)phdr->p_vaddr);
    printf("p_paddr : %p\n", (void *)phdr->p_paddr);
    printf("p_filesz: %lu\n", phdr->p_filesz);
    printf("p_memsz : %lu\n", phdr->p_memsz);
    printf("p_align : %lu\n", phdr->p_align);
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