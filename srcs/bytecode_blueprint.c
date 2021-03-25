/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_blueprint.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 00:15:39 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/26 00:15:45 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

//TODO: propagate error
int8_t		blueprint_add(t_dlist **blueprint, int btc_type)
{
	t_dlist *inst;
	t_btc	*btc;

	if (NULL == (btc = create_btc(btc_type)))
		return (FAILURE);
	if (NULL == (inst = ft_dlist_new(btc)))
		return (FAILURE);
	if (NULL == *blueprint)
		(*blueprint) = inst;
	else
		ft_dlist_append_end(blueprint, inst);
	return (SUCCESS);
}


t_dlist		*blueprint_minimal(void)
{
	t_dlist *blueprint;

	blueprint = NULL;
	blueprint_add(&blueprint, BTC_DEF_BEGIN);
	blueprint_add(&blueprint, BTC_DEF_WRITE);
	blueprint_add(&blueprint, BTC_DEF_END);
	blueprint_add(&blueprint, BTC_CALL_JMP);
	return (blueprint);
}

t_dlist		*blueprint_creation(t_packer *packer)
{
	t_dlist *blueprint;

	(void)packer;
	blueprint = NULL;
	blueprint_add(&blueprint, BTC_DEF_INIT_PERM);
	blueprint_add(&blueprint, BTC_DEF_KEY_SCHED);
	blueprint_add(&blueprint, BTC_DEF_CYPHER);
	blueprint_add(&blueprint, BTC_DEF_BEGIN);
	// blueprint_add(&blueprint, BTC_CALL_MPROTECT);
	// blueprint_add(&blueprint, BTC_DEF_CYPHER_PREPARE);
	// for zone in packer->to_crypt
		// blueprint_add(&blueprint, BTC_CALL_CYPHER);
	blueprint_add(&blueprint, BTC_DEF_WRITE);
	blueprint_add(&blueprint, BTC_DEF_END);
	blueprint_add(&blueprint, BTC_CALL_JMP);
	return (blueprint);
}