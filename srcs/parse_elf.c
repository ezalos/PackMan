/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 16:13:04 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/10 09:31:31 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t	parse_elf(t_packer *packer)
{
	if (!is_secure_access(packer->size, ((Elf64_Ehdr *)packer->content)->e_shoff,
			((uint64_t)((Elf64_Ehdr *)packer->content)->e_shentsize)
			* ((uint64_t)((Elf64_Ehdr *)packer->content)->e_shnum)))
		return (print_error(packer->self_path, FILE_FORMAT_ERROR));

	logging("*** %s Constructing phdr tree\n", __func__);
	packer->phdr_tree = construct_rbt_phdr(packer);
	if (NULL == packer->phdr_tree)
		return (print_error(packer->self_path, NO_PHDR_IN_BINARY));

	logging("*** %s: Constructing all shdr trees\n", __func__);
	construct_rbt_shdr(packer);

	logging("*** %s: Convert tree of trees in array of arrays\n", __func__);
	make_array_of_arrays(packer);

	logging("*** %s: Checking file memory referenced by phdr & shdr\n", __func__);
	if (FALSE == parse_elf_check_phdr(packer))
		return (FAILURE);
	return (SUCCESS);
}