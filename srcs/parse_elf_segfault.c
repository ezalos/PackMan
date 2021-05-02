/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf_segfault.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 22:28:24 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/02 10:29:37 by ezalos           ###   ########.fr       */
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
		if (FALSE == is_secure_access(packer->size, shdr->sh_offset, shdr->sh_size))
		{
			print_error(packer->self_path, SECTION_CONTENT_OUTSIDE_FILE, get_section_header_index(packer, shdr));
			return (FALSE);
		}
	}
	return (TRUE);
}

uint8_t		check_errors_between_loadable_segments(t_packer *packer, Elf64_Phdr *phdr, Elf64_Phdr *phdr_bis)
{
	if (is_phdr_contained(phdr, phdr_bis))
	{
		print_error(packer->self_path,
					LOADABLE_SEGMENT_CONTAINS_LOAD,
					get_program_header_index(packer, phdr),
					get_program_header_index(packer, phdr_bis));
		return (TRUE);
	}
	if (is_phdr_superposed(phdr, phdr_bis))
	{
		print_error(packer->self_path,
					LOADABLE_SEGMENT_SUPERPOSED,
					get_program_header_index(packer, phdr),
					get_program_header_index(packer, phdr_bis));
		return (TRUE);
	}
	if (is_phdr_overlap(phdr, phdr_bis))
	{
		print_error(packer->self_path,
					LOADABLE_SEGMENT_OVERLAP,
					get_program_header_index(packer, phdr),
					get_program_header_index(packer, phdr_bis));
		return (TRUE);
	}
	return (FALSE);
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
		if (FALSE == is_secure_access(packer->size, phdr->p_offset, phdr->p_filesz))
		{
			print_error(packer->self_path, SEGMENT_CONTENT_OUTSIDE_FILE, get_program_header_index(packer, phdr));
			return (FALSE);
		}
		if (phdr->p_type == PT_LOAD)
		{
			if (phdr->p_memsz < phdr->p_filesz)
			{
				print_error(packer->self_path, SEGMENT_MEMSIZE_SMALLER_FILE_SIZE, get_program_header_index(packer, phdr));
				return (FALSE);
			}
			ii = -1;
			while (packer->phdr_array[++ii])
			{
				if (i == ii)
					continue;
				phdr_bis = packer->phdr_array[ii]->phdr;
				if (phdr_bis->p_type == PT_LOAD)
					if (check_errors_between_loadable_segments(packer, phdr, phdr_bis))
						return (FALSE);
			}
		}
		if (FALSE == parse_elf_check_shdr(packer, packer->phdr_array[i]))
			return (FALSE);
	}
	return (TRUE);
}