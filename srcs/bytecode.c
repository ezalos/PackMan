/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/18 15:14:47 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/18 17:35:1 by erkirsabl        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

#define		MINIMAL_WOODY	FALSE

ssize_t		chirurgy(t_packer *packer)
{
	t_dlist	*blueprint;
	ssize_t ret;

	logging("\n*** %s: Creating blueprint\n", __func__);
	if (MINIMAL_WOODY)
	{
		//NO Segfault in woody
		blueprint = blueprint_minimal();
	}
	else
	{
		//Segfault in woody
		blueprint = blueprint_creation(packer);
	}
	logging("\n*** %s: Solving injection\n", __func__);
	if (FAILURE == (ret = solve_bytecodes(packer, packer->caves, blueprint, TRUE)))
		return (FAILURE);

	logging("\n*** %s: Entry Point: %ld [%lx]\n", __func__, ret, ret);
	((Elf64_Ehdr *)packer->content)->e_entry = ret;

	if (!MINIMAL_WOODY)
	{
		logging("\n*** %s: Cypher crypt zones", __func__);
		crypt_zones(packer);
	}

	return (SUCCESS);
}