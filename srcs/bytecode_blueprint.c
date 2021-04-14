/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_blueprint.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 20:12:38 by ezalos            #+#    #+#             */
/*   Updated: 2021/04/14 16:43:19 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

// TODO: propagate malloc error
t_dlist		*blueprint_add(t_dlist **blueprint, int btc_type)
{
	t_dlist *inst;
	t_btc	*btc;

	if (NULL == (btc = create_btc(btc_type)))
		return (NULL);
	if (NULL == (inst = ft_dlist_new(btc)))
		return (NULL);
	if (NULL == *blueprint)
		(*blueprint) = inst;
	else
		ft_dlist_append_end(blueprint, inst);
	return (inst);
}

void		set_call_cypher_args(t_btc *inst, t_zone *zone)
{
	inst->args->crypt_plaintext_vaddr =  zone->vaddr;
	inst->args->crypt_plaintext_size = zone->size;
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
	t_dlist	*inst;
	t_list	*to_crypt;

	// (void)packer;
	to_crypt = packer->to_crypt;
	blueprint = NULL;
	blueprint_add(&blueprint, BTC_DEF_FIND_ABS_VADDR);
	blueprint_add(&blueprint, BTC_DEF_INIT_PERM);
	blueprint_add(&blueprint, BTC_DEF_KEY_SCHED);
	blueprint_add(&blueprint, BTC_DEF_CYPHER);
	blueprint_add(&blueprint, BTC_DEF_BEGIN);
	// blueprint_add(&blueprint, BTC_CALL_MPROTECT);
	blueprint_add(&blueprint, BTC_DEF_CYPHER_PREPARE);
	while (to_crypt != NULL)
	{
		inst = blueprint_add(&blueprint, BTC_CALL_CYPHER);
		set_call_cypher_args(((t_btc *)inst->data), (t_zone *)to_crypt->data);
		to_crypt = to_crypt->next;
		//TODO remove line under
		to_crypt = NULL;
	}
	blueprint_add(&blueprint, BTC_DEF_WRITE);
	blueprint_add(&blueprint, BTC_DEF_END);
	blueprint_add(&blueprint, BTC_CALL_JMP);
	return (blueprint);
}

size_t		get_blueprint_inject_size(t_dlist *blueprint)
{
	size_t	size;

	size = 0;
	while (blueprint)
	{
		size += ((t_btc *)blueprint->data)->size;
		blueprint = blueprint->next;
	}
	return (size);
}

// TODOS
// 	Create 1 BTC_CALL_CYPHER by zone_to_crypt
// 		Associate the newly created BTC_CALL_CYPHER args with his zone
// 	Update args of DEF_CYPHER funcs when goign down recursive
// 	Update payload with args

