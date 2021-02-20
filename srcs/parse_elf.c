/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 16:13:04 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/20 13:31:04 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	rbt_free_content(void **content)
{
	if (content && *content)
		free(*content);
}


void 	*ft_memset(void *str, int c, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
		((char *)str)[i++] = (char)c;
	return (str);
}

long long	t_rbt_compare_phdr(void *an, void *bn)
{
	t_pheader *a = (t_pheader *)an;
	t_pheader *b = (t_pheader *)bn;
	long long av = 0;
	long long bv = 0;

	if (a)
	{
		av = ((t_pheader *)a)->phdr->p_offset;
	}
	if (b)
	{
		bv = ((t_pheader *)b)->phdr->p_offset;
	}

	return (av - bv);
}

int8_t 		create_rbt_phdr(t_rbt **node, t_pheader **hdr, Elf64_Phdr *phdr)
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
				// print_program_header(phdr);
				(*hdr)->phdr = phdr;
				// print_program_header((*hdr)->phdr);
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
	t_pheader 	*hdr;
	t_rbt		*root;
	t_rbt		*node;
	int 		i;

	root = NULL;
	i = 0;
	phdr = get_program_header(packer, i);
	while (phdr)
	{
		if (phdr->p_type & PT_LOAD)
		{
			hdr = NULL;
			node = NULL;
			if (SUCCESS == create_rbt_phdr(&node, &hdr, phdr))
			{
				root = tree_insert_func_ll(root, node, hdr, t_rbt_compare_phdr);
			}
			else
			{
				//TODO: Cleaner exit if malloc error
				print_error(packer->self_path, MALLOC_ERROR);
				tree_free(root, rbt_free_content);
				root = NULL;
				exit(EXIT_FAILURE);
			}
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
	int tmp = i;

	node = tree_get_node_th(root, &tmp);
	while (node)
	{
		tmp = ++i;
		next_node = tree_get_node_th(root, &tmp);

		if (next_node)
			((t_pheader *)node->content)->available_size = ((t_pheader *)next_node->content)->phdr->p_offset;
		else
			((t_pheader *)node->content)->available_size = packer->size;
		//TODO: is p_memsz good ?
		((t_pheader *)node->content)->available_size -= (((t_pheader *)node->content)->phdr->p_offset + ((t_pheader *)node->content)->phdr->p_filesz);

		if ((((t_pheader *)node->content)->phdr->p_type & PT_LOAD)
		&& ((t_pheader *)node->content)->available_size < 0)
		{
			// /!\ Carefull, the segment number displayed is the index in the rbtree.
			// As the non Loadable segments are skipped, the numbers are shifted
			printf("ERROR: %s has intersecting segments\n", packer->self_path);
			printf("Segment %d:\tstart %lx\tend %lx\n", i - 1, ((t_pheader *)node->content)->phdr->p_offset, ((t_pheader *)node->content)->phdr->p_offset + ((t_pheader *)node->content)->phdr->p_memsz);
			if (next_node)
				printf("Segment %d:\tstart %lx\tend %lx\n", i, ((t_pheader *)next_node->content)->phdr->p_offset, ((t_pheader *)next_node->content)->phdr->p_offset + ((t_pheader *)next_node->content)->phdr->p_memsz);
		}
		node = next_node;
	}
}

t_pheader *get_rbt_phdr_from_shdr(t_rbt *root, Elf64_Shdr *shdr)
{
	t_pheader *hdr;
	t_rbt *node;
	int i = 0;
	int tmp = i;

	//TODO: Algorithm complexity is really bad, bc this function is called for every shdr
	// Average complexity is O(n) = (Nb_shdr)^(log(Nb_phdr) * (Nb_phdr / 2))
	node = tree_get_node_th(root, &tmp);
	while (node)
	{
		hdr = node->content;
		if (hdr->phdr->p_offset <= shdr->sh_offset)
		{
			if (hdr->phdr->p_offset + hdr->phdr->p_memsz >= shdr->sh_offset + shdr->sh_size)
				return (hdr);
		}
		tmp = ++i;
		node = tree_get_node_th(root, &tmp);
	}
	return (NULL);
}

long long t_rbt_compare_shdr(void *a, void *b)
{
	t_sheader *hdr_a = ((t_rbt *)a)->content;
	t_sheader *hdr_b = ((t_rbt *)b)->content;

	return (hdr_a->shdr->sh_offset - hdr_b->shdr->sh_offset);
}

int8_t create_rbt_shdr(t_rbt **node, t_sheader **hdr, Elf64_Shdr *shdr, t_pheader *parent)
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
void construct_rbt_shdr(t_packer *packer)
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
			if (SUCCESS == create_rbt_shdr(&node, &hdr, shdr, parent))
			{
				parent->shdr_tree = tree_insert_func_ll(parent->shdr_tree, node, hdr, t_rbt_compare_shdr);
			}
			else
			{
				//TODO: Cleaner exit if malloc error
				print_error(packer->self_path, MALLOC_ERROR);
				// Need to take care of all trees, not just this one.
				tree_free(parent->shdr_tree, rbt_free_content);
				exit(EXIT_FAILURE);
			}
		}
		shdr = get_section_header(packer, ++i);
	}
}

void	parse_elf(t_packer *packer)
{
	packer->phdr_tree = construct_rbt_phdr(packer);
	if (packer->phdr_tree)
	{
		printf("%s\n", __func__);
		phdr_fill_available_size(packer, packer->phdr_tree);
		printf("%s\n", __func__);
		construct_rbt_shdr(packer);
		printf("%s\n", __func__);
	}
}