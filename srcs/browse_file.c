/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   browse_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 15:07:58 by rkirszba          #+#    #+#             */
/*   Updated: 2021/02/17 09:26:51 by ezalos           ###   ########.fr       */
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

void chirurgy(t_packer *packer, size_t offset, size_t size, Elf64_Phdr *phdr, Elf64_Shdr *shdr)
{
    Elf64_Ehdr *elf = (Elf64_Ehdr *)packer->content;
    // 1151 : 48 8d 35 ac 0e 00 00     lea rsi, [rip + 0xeac]

    (void)size;
    // size_t lea = 0;
    // size_t ptr = elf->e_entry;
    printf("Entry point is: %lx\n", elf->e_entry);
    size_t code = ((size_t)(((size_t)0x488d35) << 5) | (size_t)elf->e_entry);
    printf("Code is: %lx\n", (size_t)code);

    code = (size_t)0x488d3510600000;
    code = (size_t)0x6010358d48;

    int i = -1;
    while ((size_t)++i < sizeof(size_t))
        packer->content[offset + i] = ((uint8_t*)&code)[i];
    print_symbol_code(packer->content, offset, size); // n_phdr->p_offset - (phdr->p_offset + phdr->p_filesz));
    elf->e_entry = offset;
    phdr->p_filesz += size;
    phdr->p_memsz += size;
    shdr->sh_size += size;

    int fd;
    fd = open("woody.out", O_WRONLY | O_CREAT);
    write(fd, packer->content, packer->size);
}

void    browse_file(t_packer *packer)
{
    Elf64_Ehdr *elf = (Elf64_Ehdr *)packer->content;
    Elf64_Phdr *phdr;
    Elf64_Phdr *n_phdr;
    Elf64_Shdr *shdr;
    Elf64_Shdr *n_shdr;
    Elf64_Shdr *n_n_shdr;
    size_t available_size;
    size_t total_size;
    size_t tmp;

    int id_ph = -1;
    int id_sh;

    print_elf_header(elf);

    while (++id_ph < elf->e_phnum)
    {
        phdr = get_program_header(packer, id_ph);
        if (phdr && phdr->p_flags & PF_X && phdr->p_type == PT_LOAD)
        {
            total_size = 0;
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

                if ((n_shdr = get_section_from_segement(packer, phdr, id_sh)))
                    available_size = (n_shdr->sh_offset) - (shdr->sh_size + shdr->sh_offset);
                else
                    available_size = (phdr->p_filesz + phdr->p_offset) - (shdr->sh_size + shdr->sh_offset);
                print_symbol_code(packer->content, shdr->sh_offset, shdr->sh_size);
                printf("Available_size: %ld\n\n", available_size);
                total_size += available_size;
                tmp = (shdr->sh_size + shdr->sh_offset);
                (void)tmp;
                // printf("3\n");
                id_sh++;
                // printf("4\n");
                n_n_shdr = shdr;
                shdr = n_shdr;
            }
            printf("Total available size = %ld\n", total_size);
            if ((n_phdr = get_program_header(packer, id_ph + 1)))
            {
                printf("Size before next section: %ld\n", n_phdr->p_offset - (phdr->p_offset + phdr->p_filesz));
                chirurgy(packer, (phdr->p_offset + phdr->p_filesz), 8, phdr, n_n_shdr);
            }
        }
    }
}