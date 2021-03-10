/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 16:13:04 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/10 12:17:27 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	make_array_of_arrays(t_packer *packer)
{
	int		i;

	if (NULL == (packer->phdr_array = (t_pheader **)tree_to_array(packer->phdr_tree)))
	{
		printf("ERROR: Tree_to_array phdr\n");
		exit(EXIT_FAILURE);
	}
	tree_free(packer->phdr_tree, rbt_keep_content);
	packer->phdr_tree = NULL;
	i = -1;
	while (packer->phdr_array[++i])
	{
		if (!packer->phdr_array[i]->shdr_tree)
			continue;
		if (NULL == (packer->phdr_array[i]->shdr_array = (t_sheader **)tree_to_array(packer->phdr_array[i]->shdr_tree)))
		{
			if (packer->phdr_array[i]->shdr_tree)
			{
				printf("ERROR: Tree_to_array shdr\n");
				exit(EXIT_FAILURE);
			}
		}
		tree_free(packer->phdr_array[i]->shdr_tree, rbt_keep_content);
		packer->phdr_array[i]->shdr_tree = NULL;
	}
}

void 	fill_available_size(t_packer *packer)
{
	int i_p;
	int i_s;
	int curr_end;
	int next_start;

	i_p = -1;
	while (packer->phdr_array[++i_p])
	{
		if (packer->phdr_array[i_p + 1])
		{
			curr_end = packer->phdr_array[i_p]->phdr->p_vaddr + packer->phdr_array[i_p]->phdr->p_memsz;
			next_start = packer->phdr_array[i_p + 1]->phdr->p_vaddr;
			packer->phdr_array[i_p]->available_size = next_start - curr_end;
		}
		else
			packer->phdr_array[i_p]->available_size = 0;
		// printf("Phdr %d: %d\n", i_p, packer->phdr_array[i_p]->available_size);
		// printf("\tShdr %p\n", packer->phdr_array[i_p]->shdr_array);
		if (NULL == packer->phdr_array[i_p]->shdr_array)
			continue;
		i_s = -1;
		while (packer->phdr_array[i_p]->shdr_array[++i_s])
		{
			if (packer->phdr_array[i_p]->shdr_array[i_s + 1])
			{
				curr_end = packer->phdr_array[i_p]->shdr_array[i_s]->shdr->sh_offset + packer->phdr_array[i_p]->shdr_array[i_s]->shdr->sh_size;
				next_start = packer->phdr_array[i_p]->shdr_array[i_s + 1]->shdr->sh_offset;
				packer->phdr_array[i_p]->shdr_array[i_s]->available_size = next_start - curr_end;
			}
			else if (packer->phdr_array[i_p + 1])
				packer->phdr_array[i_p]->shdr_array[i_s]->available_size = packer->phdr_array[i_p]->available_size;
			else
				packer->phdr_array[i_p]->shdr_array[i_s]->available_size = 0;
			// printf("\tShdr %d: %d\n", i_s, packer->phdr_array[i_p]->shdr_array[i_s]->available_size);
		}
	}
}

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
		printf("Start 0x%lx\tSegment size   %ld\n", phdr->p_vaddr, phdr->p_memsz);
		printf("End   0x%lx\t%sAvailable size %d%s\n\n", phdr->p_memsz + phdr->p_vaddr, _CYAN, packer->phdr_array[i_p]->available_size, _RESET);
		if (!packer->phdr_array[i_p]->shdr_array)
			continue;
		i_s = -1;
		while (packer->phdr_array[i_p]->shdr_array[++i_s])
		{
			shdr = packer->phdr_array[i_p]->shdr_array[i_s]->shdr;
			printf("\t%sShdr %2d [%2d]:\t%s%s\n", _GREEN, i_s, get_section_header_index(packer, shdr), get_sec_name(packer, shdr), _RESET);
			printf("\t%sAlloc: %s\tExec: %s%s\n", _YELLOW,shdr->sh_flags & SHF_ALLOC ? "True" : "False", shdr->sh_flags & SHF_EXECINSTR ? "True" : "False", _RESET);
			printf("\tStart 0x%lx\tSection size   %ld\n", shdr->sh_offset, shdr->sh_size);
			printf("\tEnd   0x%lx\t%sAvailable size %d%s\n\n", shdr->sh_size + shdr->sh_offset, _CYAN, packer->phdr_array[i_p]->shdr_array[i_s]->available_size, _RESET);
		}
	}
}

void	parse_elf(t_packer *packer)
{
	printf("*** %s Constructing phdr tree\n", __func__);
	packer->phdr_tree = construct_rbt_phdr(packer);
	if (packer->phdr_tree)
	{
		printf("\n*** %s: Constructing all shdr trees\n", __func__);
		construct_rbt_shdr(packer);
		printf("\n*** %s: Convert tree of trees in array of arrays\n", __func__);
		make_array_of_arrays(packer);
		printf("\n*** %s: Filling available sizes\n", __func__);
		fill_available_size(packer);
		printf("\n*** %s: Printing results\n", __func__);
		parse_print(packer);
	}
}