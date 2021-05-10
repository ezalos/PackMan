/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf_phdr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 23:50:57 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/10 09:31:44 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t	create_rbt_phdr(t_rbt **node, t_pheader **hdr, Elf64_Phdr *phdr)
{
	int ret = FAILURE;

	if (node && hdr)
	{
		if ((*node = malloc(sizeof(t_rbt))))
		{
			ft_memset(*node, 0, sizeof(t_rbt));
			if (((*hdr) = malloc(sizeof(t_pheader))))
			{
				(*node)->content = *hdr;
				ft_memset(*hdr, 0, sizeof(t_pheader));
				(*hdr)->phdr = phdr;
				ret = SUCCESS;
			}
			else
			{
				free(*node);
			}
		}
	}
	return (ret);
}

t_rbt	*construct_rbt_phdr(t_packer *packer)
{
	Elf64_Phdr *phdr;
	t_pheader *hdr;
	t_rbt *root;
	t_rbt *node;
	int i;

	root = NULL;
	i = 0;
	phdr = get_program_header(packer, i);
	while (phdr)
	{
		hdr = NULL;
		node = NULL;
		if (SUCCESS == create_rbt_phdr(&node, &hdr, phdr))
		{
			root = tree_insert_func_ll(root, node, hdr, t_rbt_compare_phdr);
		}
		else
		{
			print_error(packer->self_path, MALLOC_ERROR);
			// When malloc error, we do not free as internal malloc error might be in cause.
			// We let the kernel take care of restuting the memory
			root = NULL;
			exit(EXIT_FAILURE);
		}
		i++;
		phdr = get_program_header(packer, i);
	}
	return (root);
}