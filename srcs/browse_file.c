/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   browse_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 15:07:58 by rkirszba          #+#    #+#             */
/*   Updated: 2021/02/16 19:03:32 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

Elf64_Shdr *get_section_from_segement(t_packer *packer, Elf64_Phdr *phdr, uint32_t index)
{
    Elf64_Shdr *shdr;
    size_t min = phdr->p_offset + (size_t)packer->content;
    size_t max = min + phdr->p_filesz;
    int id_sh;

    id_sh = 0;
    shdr = get_section_header(packer, id_sh);
    while (shdr)
    {
        // printf("%lx > %lx ?\n", shdr->sh_offset + (size_t)packer->content, min);
        // printf("%lx < %lx ?\n", shdr->sh_offset + shdr->sh_size + (size_t)packer->content, max);
        if (shdr->sh_offset + (size_t)packer->content > min
        || shdr->sh_offset + shdr->sh_size + (size_t)packer->content < max)
        {
            // printf("In\n");
            if (shdr->sh_offset + (size_t)packer->content > min
            && shdr->sh_offset + shdr->sh_size + (size_t)packer->content < max)
            {
                // printf("GOOD: idx = %d\n", index);
                if (index == 0)
                    return shdr;
                index--;
            }
            else
            {
                // printf("ERROR: Section %d is beetween 2 segemnts\n", id_sh);
            }
        }
        id_sh++;
        shdr = get_section_header(packer, id_sh);
    }
    return shdr;
}

void    browse_file(t_packer *packer)
{
    Elf64_Ehdr *elf = (Elf64_Ehdr *)packer->content;
    Elf64_Phdr *phdr;
    Elf64_Shdr *shdr;

    int id_ph = -1;
    int id_sh;

    print_elf_header(elf);

    while (++id_ph < elf->e_phnum)
    {
        phdr = get_program_header(packer, id_ph);
        if (phdr && phdr->p_flags & PF_X && phdr->p_type == PT_LOAD)
        {
            printf("Found section %d which is of great interest\n", id_ph);
            print_program_header(phdr);

            id_sh = 0;
            // printf("0\n");
            shdr = get_section_from_segement(packer, phdr, id_sh);
            // printf("1\n");
            while (shdr)
            {
                // printf("2\n");
                print_section_header(packer, shdr);
                // printf("3\n");
                id_sh++;
                shdr = get_section_from_segement(packer, phdr, id_sh);
                // printf("4\n");
            }
        }
    }
}