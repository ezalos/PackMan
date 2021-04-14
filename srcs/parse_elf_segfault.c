/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf_segfault.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 22:28:24 by ezalos            #+#    #+#             */
/*   Updated: 2021/04/14 17:49:07 by ezalos           ###   ########.fr       */
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
	Elf64_Phdr	*phdr_bis;
	int			i;
	int			ii;

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
		if (phdr->p_type == PT_LOAD)
		{
			if (phdr->p_memsz < phdr->p_filesz)
			{
				dprintf(2, "ERROR: Phdr %d has memsize < filesize\n",
						get_program_header_index(packer, phdr));
				return (FALSE);
				ii = -1;
				while (packer->phdr_array[++ii])
				{
					if (i == ii)
						continue;
					phdr_bis = packer->phdr_array[ii]->phdr;
					if (phdr_bis->p_type == PT_LOAD)
					{
						if (is_phdr_contained(phdr, phdr_bis))
						{
							dprintf(2, "ERROR: is_phdr_contained between %d and %d\n",
									get_program_header_index(packer, phdr),
									get_program_header_index(packer, phdr_bis));
							return (FALSE);
						}
						if (is_phdr_superposed(phdr, phdr_bis))
						{
							dprintf(2, "ERROR: is_phdr_superposed between %d and %d\n",
									get_program_header_index(packer, phdr),
									get_program_header_index(packer, phdr_bis));
							return (FALSE);
						}
						if (is_phdr_overlap(phdr, phdr_bis))
						{
							dprintf(2, "ERROR: is_phdr_overlap between %d and %d\n",
									get_program_header_index(packer, phdr),
									get_program_header_index(packer, phdr_bis));
							return (FALSE);
						}
					}
				}
			}
		}
		if (FALSE == parse_elf_check_shdr(packer, packer->phdr_array[i]))
			return (FALSE);
	}
	return (TRUE);
}