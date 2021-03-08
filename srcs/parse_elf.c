/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 16:13:04 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/08 13:52:28 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t		create_rbt_phdr(t_rbt **node, t_pheader **hdr, Elf64_Phdr *phdr)
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

t_rbt		*construct_rbt_phdr(t_packer *packer)
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
		if (TRUE || phdr->p_type & PT_LOAD)
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

// void	phdr_fill_available_size(t_packer *packer, t_rbt *root)
// {
// 	t_rbt *node;
// 	t_rbt *next_node = NULL;
// 	int i = 0;
// 	int tmp = i;

// 	node = tree_get_node_th(root, &tmp);
// 	while (node)// && i < ((Elf64_Ehdr *)packer->content)->e_phnum)
// 	{
// 		tmp = ++i;
// 		next_node = tree_get_node_th(root, &tmp);

// 		if (next_node)
// 			((t_pheader *)node->content)->available_size = ((t_pheader *)next_node->content)->phdr->p_offset;
// 		else
// 			((t_pheader *)node->content)->available_size = packer->size;
// 		//TODO: is p_memsz good ?
// 		((t_pheader *)node->content)->available_size -= (((t_pheader *)node->content)->phdr->p_offset + ((t_pheader *)node->content)->phdr->p_filesz);

// 		printf("\n\nPhdr n*%d\n", i);
// 		print_program_header(((t_pheader *)node->content)->phdr);
// 		if ((((t_pheader *)node->content)->phdr->p_type & PT_LOAD)
// 		&& ((t_pheader *)node->content)->available_size < 0
// 		&& (!next_node || (((t_pheader *)next_node->content)->phdr->p_type & PT_LOAD)))
// 		{
// 			printf("ERROR: %s has intersecting segments\n", packer->self_path);
// 			printf("Segment %d:\tstart %lx\tend %lx\n", i - 1, ((t_pheader *)node->content)->phdr->p_offset, ((t_pheader *)node->content)->phdr->p_offset + ((t_pheader *)node->content)->phdr->p_memsz);
// 			if (next_node)
// 				printf("Segment %d:\tstart %lx\tend %lx\n", i, ((t_pheader *)next_node->content)->phdr->p_offset, ((t_pheader *)next_node->content)->phdr->p_offset + ((t_pheader *)next_node->content)->phdr->p_memsz);
// 		}
// 		node = next_node;
// 	}
// }

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
		// else
		// {
			if (hdr->phdr->p_offset <= shdr->sh_offset)
			{
				if (hdr->phdr->p_offset + hdr->phdr->p_filesz >= shdr->sh_offset + shdr->sh_size)
					return (hdr);
			}
		// }
		tmp = ++i;
		node = tree_get_node_th(root, &tmp);
	}
	return (NULL);
}

int8_t	create_rbt_shdr(t_rbt **node, t_sheader **hdr, Elf64_Shdr *shdr, t_pheader *parent)
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
void	construct_rbt_shdr(t_packer *packer)
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
		// printf("Loop\n");
		parent = get_rbt_phdr_from_shdr(packer->phdr_tree, shdr);
		// printf("Loop1\n");
		if (parent)
		{
			printf("Shdr %2d parent is n*%d:\t%s\n", i, get_program_header_index(packer, parent->phdr), get_sec_name(packer, shdr));
			// printf("Loop2\n");
			if (SUCCESS == create_rbt_shdr(&node, &hdr, shdr, parent))
			{
			// 	printf("Loop3\n");
			// 	printf("hdr: %p\n", hdr);
				parent->shdr_tree = tree_insert_func_ll(parent->shdr_tree, node, hdr, t_rbt_compare_shdr);
			}
			else
			{
				// printf("Loop4\n");
				//TODO: Cleaner exit if malloc error
				print_error(packer->self_path, MALLOC_ERROR);
				// Need to take care of all trees, not just this one.
				tree_free(parent->shdr_tree, rbt_free_content);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			printf("Shdr %2d is orphan:\t%s\n", i, get_sec_name(packer, shdr));
		}
		// printf("Loop5\n");
		shdr = get_section_header(packer, ++i);
	}
}

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
		printf("Phdr %d, There is %d shdr %p\n", i, tree_len(packer->phdr_array[i]->shdr_tree), packer->phdr_array[i]->shdr_tree);
		if (NULL == (packer->phdr_array[i]->shdr_array = (t_sheader **)tree_to_array(packer->phdr_array[i]->shdr_tree)))
		{
			if (packer->phdr_array[i]->shdr_tree)
			{
				printf("ERROR: Tree_to_array shdr\n");
				exit(EXIT_FAILURE);
			}
		}
		// printf("\tPtr shdr_arr: %p\n", packer->phdr_array[i]->shdr_array);
		tree_free(packer->phdr_array[i]->shdr_tree, rbt_keep_content);
		packer->phdr_array[i]->shdr_tree = NULL;
		printf("\tPtr shdr_arr:   %p\n", packer->phdr_array[i]->shdr_array);
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
		printf("Phdr %d: %d\n", i_p, packer->phdr_array[i_p]->available_size);
		printf("\tShdr %p\n", packer->phdr_array[i_p]->shdr_array);
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
			printf("\tShdr %d: %d\n", i_s, packer->phdr_array[i_p]->shdr_array[i_s]->available_size);
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
		printf("Phdr %2d\t\tLoad: %s\n", i_p, phdr->p_type & PT_LOAD ? "True" : "False");
		printf("Start 0x%lx\tSegment size   %ld\n", phdr->p_vaddr, phdr->p_memsz);
		printf("End   0x%lx\tAvailable size %d\n\n", phdr->p_memsz + phdr->p_vaddr, packer->phdr_array[i_p]->available_size);
		if (!packer->phdr_array[i_p]->shdr_array)
			continue;
		i_s = -1;
		while (packer->phdr_array[i_p]->shdr_array[++i_s])
		{
			shdr = packer->phdr_array[i_p]->shdr_array[i_s]->shdr;
			printf("\tShdr %d [%d]:\t%s\n", i_s, get_section_header_index(packer, shdr), get_sec_name(packer, shdr));
			printf("\tAlloc: %s\tExec: %s\n", shdr->sh_flags & SHF_ALLOC ? "True" : "False", shdr->sh_flags & SHF_EXECINSTR ? "True" : "False");
			printf("\tStart 0x%lx\tSection size   %ld\n", shdr->sh_offset, shdr->sh_size);
			printf("\tEnd   0x%lx\tAvailable size %d\n\n", shdr->sh_size + shdr->sh_offset, packer->phdr_array[i_p]->shdr_array[i_s]->available_size);
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