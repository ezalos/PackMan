/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf_segfault.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 22:28:24 by ezalos            #+#    #+#             */
/*   Updated: 2021/04/14 17:01:17 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

uint8_t		parse_elf_check_shdr(t_packer *packer, t_pheader *t_pheader)
{
	Elf64_Shdr	*shdr;
	int			i;

	i = -1;
	while (t_pheader->shdr_array && t_pheader->shdr_array[++i])
	{
		shdr = t_pheader->shdr_array[i]->shdr;
		if (shdr->sh_offset + shdr->sh_size > packer->size)
		{
			dprintf(2, "ERROR: shdr %d refernce memory out of file\n", \
					get_section_header_index(packer, shdr));
			return (FALSE);
		}
	}
	return (TRUE);
}

uint8_t		parse_elf_check_phdr(t_packer *packer)
{
	Elf64_Phdr	*phdr;
	int			i;

	i = -1;
	while (packer->phdr_array && packer->phdr_array[++i])
	{
		phdr = packer->phdr_array[i]->phdr;
		if (phdr->p_filesz + phdr->p_offset > packer->size)
		{
			dprintf(2, "ERROR: Phdr %d refernce memory out of file\n", \
					get_program_header_index(packer, phdr));
			return (FALSE);
		}
		if (phdr->p_type == PT_LOAD && (phdr->p_memsz < phdr->p_filesz))
		{
			dprintf(2, "ERROR: Phdr %d has memsize < filesize\n",
					get_program_header_index(packer, phdr));
			return (FALSE);
		}
		if (FALSE == parse_elf_check_shdr(packer, packer->phdr_array[i]))
			return (FALSE);
	}
	return (TRUE);
}