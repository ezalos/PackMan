/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 11:13:17 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/28 20:56:58 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	update_arg_def_crypt_calls(t_dlist *inst, t_zone *zone)
{
	logging_recursive("%s\n", __func__);
	if (((t_btc *)inst->data)->type == BTC_DEF_CYPHER)
	{
		while ((inst = inst->next) && ((t_btc *)inst->data))
			if (((t_btc *)inst->data)->type == BTC_CALL_CYPHER)
				((t_btc *)inst->data)->args->crypt_func_def_vaddr = (uint64_t)((size_t)zone->vaddr);
	}
	else if (((t_btc *)inst->data)->type == BTC_DEF_INIT_PERM)
	{
		while ((inst = inst->next) && ((t_btc *)inst->data))
			if (((t_btc *)inst->data)->type == BTC_DEF_CYPHER_PREPARE)
				((t_btc *)inst->data)->args->crypt_func_init_perm_vaddr = (uint64_t)((size_t)zone->vaddr);
	}
	else if (((t_btc *)inst->data)->type == BTC_DEF_KEY_SCHED)
	{
		while ((inst = inst->next) && ((t_btc *)inst->data))
			if (((t_btc *)inst->data)->type == BTC_DEF_CYPHER_PREPARE)
				((t_btc *)inst->data)->args->crypt_func_key_sched_vaddr = (uint64_t)((size_t)zone->vaddr);
	}
}

void	update_args(t_packer *packer, t_btc *inst, t_zone *zone, ssize_t ret)
{
	if (inst->type == BTC_CALL_JMP)
	{
		inst->args->jump = ((size_t)ret/*to*/) - (zone->vaddr/*from*/ + inst->size/*instuction size == 5*/);
		logging_recursive("Jump value\t\t %x: ret %lx - (vadr %lx + btc %lx)\n", inst->args->jump, ret, zone->vaddr, inst->size);
	}
	else if (inst->type == BTC_CALL_MPROTECT)
	{
		// Actually wrong, should be done before
		
		// Should not be zone of mprotect, but zone to be affected
		inst->args->mp_addr = (uint64_t)((size_t)zone->phdr->p_vaddr + (size_t)zone->phdr->p_offset);
		inst->args->mp_len = getpagesize();
		inst->args->mp_prot = PROT_READ | PROT_EXEC | PROT_WRITE;
	}
	else if (inst->type == BTC_DEF_CYPHER_PREPARE)
	{
		inst->args->jmp_init_perm = (inst->args->crypt_func_init_perm_vaddr /*to*/) - (zone->vaddr /*from*/ + OFFSET_CALL_INIT_PERM + 4 /*instuction size*/);
		inst->args->jmp_key_sched = (inst->args->crypt_func_key_sched_vaddr /*to*/) - (zone->vaddr /*from*/ + OFFSET_CALL_KEY_SCHED + 4 /*instuction size*/);
		logging_recursive("init_perm_vaddr = %lx\n", inst->args->crypt_func_init_perm_vaddr);
		logging_recursive("init_key_sched = %lx\n", inst->args->crypt_func_key_sched_vaddr);
		logging_recursive("zone->vaddr = %lx\n", zone->vaddr);

		// inst->args->jmp_init_perm = 0xFFFFFF3B;
		// inst->args->jmp_key_sched = 0xFFFFFF3C;

		inst->args->crypt_key = (uint8_t*)packer->key;
		// Nothing
	}
	else if (inst->type == BTC_CALL_CYPHER)
	{
		inst->args->jmp_def_cypher = (uint32_t)((size_t)inst->args->crypt_func_def_vaddr /*to*/) - (zone->vaddr /*from*/ + OFFSET_CALL_CYPHER + 4/*instuction size*/);
		// Should be done before ...

		// inst->args->crypt_addr = NULL;
		// inst->args->crypt_key = NULL;
		// inst->args->crypt_size = 0;
	}
	else if (inst->type == BTC_DEF_WRITE)
	{
		// Nothing
	}
	else if (inst->type == BTC_DEF_END)
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
	else if (inst->type == BTC_DEF_BEGIN)
	{
		// Nothing
	}
	else
	{
		dprintf(2, "Error: Unknown btc type\n");
	}
	return;
}
