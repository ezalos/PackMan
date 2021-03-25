/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 11:13:17 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/25 16:25:18 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	update_arg_crypt_calls(t_dlist *inst, t_zone *zone)
{
	while (inst)
	{
		if (((t_btc*)inst->data)->type == BTC_CALL_CYPHER)
		{
			((t_btc *)inst->data)->args->crypt_func_addr = (void *)((size_t)zone->phdr->p_vaddr + (size_t)zone->offset);
		}
		inst = inst->next;
	}
}

void	update_args(t_btc *inst, t_zone *zone, ssize_t ret)
{
	if (inst->type == BTC_CALL_JMP)
	{
		inst->args->jump = ret - (zone->offset + inst->size/*instuction code (0xe9) size*/);
	}
	else if (inst->type == BTC_CALL_MPROTECT)
	{
		// Actually wrong, should be done before
		
		// Should not be zone of mprotect, but zone to be affected
		inst->args->mp_addr = (void*)((size_t)zone->phdr->p_vaddr + (size_t)zone->phdr->p_offset);
		inst->args->mp_len = getpagesize();
		inst->args->mp_prot = PROT_READ | PROT_EXEC | PROT_WRITE;
	}
	else if (inst->type == BTC_CALL_CYPHER)
	{
		// Should be done before ...

		// inst->args->crypt_addr = NULL;
		// inst->args->crypt_key = NULL;
		// inst->args->crypt_size = 0;
	}
	else if (inst->type == BTC_DEF_WRITE)
	{
		// Nothing
	}
	else if (inst->type == BTC_DEF_CYPHER)
	{
		// Nothing
	}
	else if (inst->type == BTC_DEF_INIT_PERM)
	{
		// Nothing
	}
	else if (inst->type == BTC_DEF_KEY_SCHED)
	{
		// Nothing
	}
	else
	{
		dprintf(2, "Error: Unknown btc type\n");
	}
	return;
}
