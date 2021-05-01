/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf_array.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 21:19:30 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/11 11:59:52 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	make_array_of_arrays(t_packer *packer)
{
	int i;

	if (NULL == (packer->phdr_array = (t_pheader **)tree_to_array(packer->phdr_tree)))
	{
		print_error(packer->self_path, MALLOC_ERROR);
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
				print_error(packer->self_path, MALLOC_ERROR);
				exit(EXIT_FAILURE);
			}
		}
		tree_free(packer->phdr_array[i]->shdr_tree, rbt_keep_content);
		packer->phdr_array[i]->shdr_tree = NULL;
	}
}