/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf_shdr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 23:56:07 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/10 18:22:02 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t		create_rbt_shdr(t_rbt **node, t_sheader **hdr, Elf64_Shdr *shdr, t_pheader *parent)
{
	int ret = FAILURE;

	if (node && hdr)
	{
		if ((*node = malloc(sizeof(t_rbt))))
		{
			if ((*hdr = malloc(sizeof(t_sheader))))
			{
				(*node)->content = *hdr;
				ft_memset(*hdr, 0, sizeof(t_sheader));
				(*hdr)->shdr = shdr;
				(*hdr)->parent_phdr = parent;
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

//TODO: Should be better protected ? With a return value and by cleaning all trees
void		construct_rbt_shdr(t_packer *packer)
{
	Elf64_Shdr *shdr;
	t_sheader *hdr;
	t_pheader *parent;
	t_rbt *node;
	int i;

	i = 0;
	shdr = get_section_header(packer, i);
	while (shdr)
	{
		parent = get_rbt_phdr_from_shdr(packer->phdr_tree, shdr);
		if (parent)
		{
			// printf("Shdr %2d parent is n*%d:\t%s\n", i, get_program_header_index(packer, parent->phdr), get_sec_name(packer, shdr));
			if (SUCCESS == create_rbt_shdr(&node, &hdr, shdr, parent))
			{
				parent->shdr_tree = tree_insert_func_ll(parent->shdr_tree, node, hdr, t_rbt_compare_shdr);
			}
			else
			{
				print_error(packer->self_path, MALLOC_ERROR);
				tree_free(parent->shdr_tree, rbt_free_content);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			printf("Shdr %2d is orphan:\t%s\n", i, get_sec_name(packer, shdr));
		}
		shdr = get_section_header(packer, ++i);
	}
}

t_pheader	*get_rbt_phdr_from_shdr(t_rbt *root, Elf64_Shdr *shdr)
{
	t_pheader *hdr;
	t_rbt *node;
	int i = 0;
	int tmp = i;

	//TODO: Algorithm complexity is really bad, bc this function is called for every shdr
	// Average complexity is O(n) = (Nb_shdr)^(log(Nb_phdr) * (Nb_phdr / 2))
	// printf("T_2_A: %p\n", tree_to_array(root));
	node = tree_get_node_th(root, &tmp);
	while (node)
	{
		hdr = node->content;
		tmp = shdr->sh_addralign;
		if (tmp == 0)
			tmp = 1;
		if (shdr->sh_type & PT_LOAD)
		{
			if (hdr->phdr->p_vaddr <= shdr->sh_addr + tmp - (shdr->sh_addr % tmp))
			{
				if (hdr->phdr->p_vaddr + hdr->phdr->p_memsz >= shdr->sh_addr + tmp - (shdr->sh_addr % tmp) + shdr->sh_size)
					return (hdr);
			}
		}
		if (hdr->phdr->p_offset <= shdr->sh_offset)
		{
			if (hdr->phdr->p_offset + hdr->phdr->p_filesz >= shdr->sh_offset + shdr->sh_size)
				return (hdr);
		}
		tmp = ++i;
		node = tree_get_node_th(root, &tmp);
	}
	return (NULL);
}