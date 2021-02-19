/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 16:13:04 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/19 17:47:42 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void 	*ft_memset(void *str, int c, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
		((char *)str)[i++] = (char)c;
	return (str);
}

long long	t_rbt_compare_phdr(void *a, void *b)
{
	t_header *hdr_a = ((t_rbt *)a)->content;
	t_header *hdr_b = ((t_rbt *)b)->content;

	return (hdr_a->phdr->p_offset - hdr_b->phdr->p_offset);
}

int8_t 		create_rbt_hdr(t_rbt **node, t_header **hdr, Elf64_Phdr *phdr, Elf64_Shdr *shdr)
{
	int		ret = FAILURE;

	if (node && hdr)
	{
		if ((*node = malloc(sizeof(t_rbt))))
		{
			if ((*hdr = malloc(sizeof(t_header))))
			{
				ft_memset(*hdr, 0, sizeof(t_header));
				if (phdr)
				{
					(*hdr)->phdr = phdr;
					(*hdr)->type = HDR_TYPE_PHDR;
				}
				else if (shdr)
				{
					(*hdr)->shdr = shdr;
					(*hdr)->type = HDR_TYPE_SHDR;
				}
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

t_rbt 		*construct_rbt_phdr(t_packer *packer)
{
	Elf64_Phdr	*phdr;
	t_header 	*hdr;
	t_rbt		*root;
	t_rbt		*node;
	int 		i;

	i = 0;
	phdr = get_program_header(packer, i);
	while (phdr)
	{
		if (SUCCESS == create_rbt_phdr(&node, &hdr, phdr, NULL))
		{
			root = tree_insert_func_ll(root, node, hdr, t_rbt_compare_phdr);
		}
		else
		{
			//TODO: Cleaner exit if malloc error
			print_error(packer->self_path, MALLOC_ERROR);
			exit(EXIT_FAILURE);
		}
		i++;
		phdr = get_program_header(packer, i);
	}
	return (root);
}

void 	phdr_fill_available_size(t_packer *packer, t_rbt *root)
{
	t_rbt *node;
	t_rbt *next_node = NULL;
	int i = 0;

	node = tree_get_node_th(root, i);
	while (node)
	{
		i++;
		next_node = tree_get_node_th(root, i);

		if (next_node)
			((t_header *)node->content)->available_size = ((t_header *)next_node->content)->phdr->p_offset;
		else
			((t_header *)node->content)->available_size = packer->size;
		//TODO: is p_memsz good ?
		((t_header *)node->content)->available_size -= (((t_header *)node->content)->phdr->p_offset + ((t_header *)node->content)->phdr->p_memsz);

		if (((t_header *)node->content)->available_size < 0)
		{
			printf("ERROR: %s has intersecting sections\n", packer->self_path);
		}
	}
}