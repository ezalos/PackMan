/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 11:13:17 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/07 12:31:17 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	update_arg_def_crypt_calls(t_dlist *inst, t_zone *zone)
{

	if (((t_btc *)inst->data)->type == BTC_DEF_CYPHER)
	{
		logging_recursive("%s:\t%s\n", __func__, btc_to_str(((t_btc *)inst->data)));
		while ((inst = inst->next) && ((t_btc *)inst->data))
			if (((t_btc *)inst->data)->type == BTC_CALL_CYPHER)
				((t_btc *)inst->data)->args->crypt_func_def_vaddr = (uint64_t)((size_t)zone->vaddr);
	}
	else if (((t_btc *)inst->data)->type ==  BTC_DEF_FIND_ABS_VADDR)
	{
		logging_recursive("%s:\t%s\n", __func__, btc_to_str(((t_btc *)inst->data)));
		((t_btc *)inst->data)->args->crypt_func_find_abs_vaddr_vaddr = (uint64_t)((size_t)zone->vaddr);
		while ((inst = inst->next) && ((t_btc *)inst->data))
		{
			if (((t_btc *)inst->data)->type == BTC_CALL_CYPHER)
				((t_btc *)inst->data)->args->crypt_func_find_abs_vaddr_vaddr = (uint64_t)((size_t)zone->vaddr);
		}
	}
	else if (((t_btc *)inst->data)->type == BTC_DEF_INIT_PERM)
	{
		logging_recursive("%s:\t%s\n", __func__, btc_to_str(((t_btc *)inst->data)));
		while ((inst = inst->next) && ((t_btc *)inst->data))
			if (((t_btc *)inst->data)->type == BTC_DEF_CYPHER_PREPARE)
				((t_btc *)inst->data)->args->crypt_func_init_perm_vaddr = (uint64_t)((size_t)zone->vaddr);
	}
	else if (((t_btc *)inst->data)->type == BTC_DEF_KEY_SCHED)
	{
		logging_recursive("%s:\t%s\n", __func__, btc_to_str(((t_btc *)inst->data)));
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
		logging_recursive("%s: Jump value\t\t %x: ret %lx - (vadr %lx + btc %lx)\n", __func__, inst->args->jump, ret, zone->vaddr, inst->size);
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
		logging_recursive("%s: init_perm_vaddr = %lx\n", __func__, inst->args->crypt_func_init_perm_vaddr);
		logging_recursive("%s: init_key_sched = %lx\n", __func__, inst->args->crypt_func_key_sched_vaddr);
		logging_recursive("%s: zone->vaddr = %lx\n", __func__, zone->vaddr);

		inst->args->crypt_key = (uint8_t*)packer->key;
	}
	else if (inst->type == BTC_CALL_CYPHER)
	{
		inst->args->jmp_def_cypher = (uint32_t)((size_t)inst->args->crypt_func_def_vaddr /*to*/) - (zone->vaddr /*from*/ + OFFSET_CALL_CYPHER + 4/*instuction size*/);
		
		inst->args->jmp_find_abs_addr = (uint32_t)((size_t)inst->args->crypt_func_find_abs_vaddr_vaddr /*to*/) - (zone->vaddr /*from*/ + OFFSET_CALL_FIND_ABS_ADDR + 4/*instuction size*/);
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
	return;
}
