/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 16:13:04 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/25 22:59:41 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void 	parse_print(t_packer *packer)
{
	Elf64_Phdr *phdr;
	Elf64_Shdr *shdr;
	int i_p;
	int i_s;

	printf("\n-----------------\n");
	printf("PARSE ELF RESULTS\n");
	printf("-----------------\n\n");
	i_p = -1;
	while (packer->phdr_array[++i_p])
	{
		phdr = packer->phdr_array[i_p]->phdr;
		printf("%sPhdr %2d [%2d]%s\t%sLoad: %s%s\n", _GREEN, i_p, get_program_header_index(packer, packer->phdr_array[i_p]->phdr), _RESET, _YELLOW, phdr->p_type & PT_LOAD ? "True" : "False", _RESET);
		printf("Start 0x%lx\tSegment size   %ld\n", phdr->p_offset, phdr->p_filesz);
		printf("End   0x%lx\t%sAvailable size %d%s\n", phdr->p_filesz + phdr->p_offset, _CYAN, packer->phdr_array[i_p]->available_size, _RESET);
		if (!packer->phdr_array[i_p]->shdr_array)
			continue;
		i_s = -1;
		while (packer->phdr_array[i_p]->shdr_array[++i_s])
		{
			shdr = packer->phdr_array[i_p]->shdr_array[i_s]->shdr;
			printf("\t%sShdr %2d [%2d]:\t%s%s\n", _GREEN, i_s, get_section_header_index(packer, shdr), get_sec_name(packer, shdr), _RESET);
			printf("\t%sAlloc: %s\tExec: %s%s\n", _YELLOW,shdr->sh_flags & SHF_ALLOC ? "True" : "False", shdr->sh_flags & SHF_EXECINSTR ? "True" : "False", _RESET);
			printf("\tStart 0x%lx\tSection size   %ld\n", shdr->sh_offset, shdr->sh_size);
			printf("\tEnd   0x%lx\t%sAvailable size %d%s\n", shdr->sh_size + shdr->sh_offset, _CYAN, packer->phdr_array[i_p]->shdr_array[i_s]->available_size, _RESET);
		}
	}
}

int8_t	parse_elf(t_packer *packer)
{
	printf("*** %s Constructing phdr tree\n", __func__);
	packer->phdr_tree = construct_rbt_phdr(packer);
	if (packer->phdr_tree)
	{
		printf("\n*** %s: Constructing all shdr trees\n", __func__);
		construct_rbt_shdr(packer);
		printf("\n*** %s: Convert tree of trees in array of arrays\n", __func__);
		make_array_of_arrays(packer);
		// printf("\n*** %s: Filling available sizes\n", __func__);
		// fill_available_size(packer);
		if (FALSE == parse_elf_check_phdr(packer))
		{
			dprintf(2, "ERROR: Corrupted binary\n");
			return (FAILURE);
		}
		return (SUCCESS);
	}
	return (FAILURE);
}