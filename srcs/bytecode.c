/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/18 15:14:47 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/07 13:25:54 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

// TODO: clean logging + clean algo ?

ssize_t		chirurgy(t_packer *packer)
{
	t_dlist	*blueprint;
	size_t	size_blueprints;
	ssize_t ret;

	logging("\n* CHIRURGY\n");
	init_key(packer->key);
	logging("** %s: Init key: %016llx (in hex)\n", __func__, *(uint64_t *)packer->key);
	logging("** %s: Creating blueprint\n", __func__);

	blueprint = blueprint_creation(packer);
	logging("** %s: Solving injection\n", __func__);

	packer->strategy = 0;
	while (packer->strategy < NB_STRAT)
	{
		logging("** %s: SOLVE BYTECODE: trying start %d\n", __func__, packer->strategy);
		if (packer->strategy == STRAT_LOADABLE && TRUE == packer->no_strat_loadable)
		{
			logging("** %s: Skipping STRAT_LOADABLE\n", __func__);
			packer->strategy += 1;
			continue;
		}
		if (packer->strategy == STRAT_LOADABLE_LAST_SEGMENT)
		{
			size_blueprints = get_blueprint_inject_size(blueprint);
			logging("** %s: size_blueprints = %lu\n", __func__, size_blueprints);
			logging("** %s: content address before prepare_last_segment = %p\n", __func__, packer->content);
			prepare_last_segment_strategy(packer, size_blueprints);
			logging("** %s: content address  after prepare_last_segment = %p\n", __func__, packer->content);
		}

		if (FAILURE != (ret = solve_bytecodes(packer, packer->caves, blueprint, TRUE)))
		{
			logging("Successful strategy: %d\n", packer->strategy);
			break ;
		}
		packer->strategy += 1;
		if (packer->strategy == 1)
			packer->strategy += 1;
	}

	ft_dlist_free(blueprint, &free_btc);

	if (ret == FAILURE)
		return (FAILURE);

	// TODO: add rights when up recursive and remove line behind ?      
	zones_add_rights_to_used_caves(packer->caves, PF_X);

	logging("\n** %s: Entry Point: 0x%lx [%ld]\n", __func__, packer->new_e_entry, packer->new_e_entry);
	((Elf64_Ehdr *)packer->content)->e_entry = packer->new_e_entry;

	// Error before loading when used
	// ./woody.out: 14: Syntax error: word unexpected (expecting ")")
	//              I think it's just because we dont uncrypt (Highly probable)
	logging("\n** %s: Cypher crypt zones\n", __func__);
	crypt_zones(packer);
	return (SUCCESS);
}
