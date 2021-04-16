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
#define		NB_STRAT 3 // a mettre dans .h
#define		STRAT_LOADABLE_EXECUTE 0
#define		STRAT_LOADABLE 1
#define		STRAT_LOADABLE_LAST_SEGMENT 2

ssize_t		chirurgy(t_packer *packer)
{
	t_dlist	*blueprint;
	size_t	size_blueprints;
	ssize_t ret;

	logging("\n*** %s: Init key: %016llx (in hex)\n", __func__, *(uint64_t *)packer->key);
	init_key(packer->key);
	logging("\n*** %s: Creating blueprint\n", __func__);
	if (MINIMAL_WOODY)
	{
		blueprint = blueprint_minimal();
	}
	else
	{
		blueprint = blueprint_creation(packer);
	}
	logging("\n*** %s: Solving injection\n", __func__);

	while (packer->strategy < NB_STRAT)
	{
		if (packer->strategy == STRAT_LOADABLE_LAST_SEGMENT)
		{
			size_blueprints = get_blueprint_inject_size(blueprint);
			prepare_last_segment_strategy(packer, size_blueprints);
		}

		if (FAILURE != (ret = solve_bytecodes(packer, packer->caves, blueprint, TRUE)))
			break ;
		packer->strategy += 1;
	}
	if (ret == FAILURE)
		return (FAILURE);

	logging("\n*** %s: Entry Point: 0x%lx [%ld]\n", __func__, packer->new_e_entry, packer->new_e_entry);
	((Elf64_Ehdr *)packer->content)->e_entry = packer->new_e_entry;

	if (!MINIMAL_WOODY)
	{
		// Error before loading when used
		// ./woody.out: 14: Syntax error: word unexpected (expecting ")")
		// 		I think it's just because we dont uncrypt (Highly probable)
		logging("\n*** %s: Cypher crypt zones\n", __func__);
		crypt_zones(packer);
		logging("\n*** %s: Cypher key is %016llx (in hex)\n", __func__, *(uint64_t*)packer->key);
	}

	return (SUCCESS);
}