/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_solver.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 11:11:34 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/07 13:15:52 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int depth = -1;

void		update_zone(t_zone *zone, t_btc *inst)
{
	// printed boundary is included
	debug_recursive("%s: Updating zone:\t%s %zu->%zu\n", __func__, btc_to_str(inst), zone->offset, zone->offset + inst->size);
	zone->offset += inst->size;
	zone->vaddr += inst->size;
	zone->size -= inst->size;
	// debug("to %zu->%zu\n", zone->offset, zone->offset + inst->size);
}

void		undo_update_zone(t_zone *zone, t_btc *inst)
{
	debug_recursive("%s: Updating zone (undo):\t%s %zu->%zu\n", __func__, btc_to_str(inst), zone->offset, zone->offset + inst->size);
	zone->offset -= inst->size;
	zone->vaddr -= inst->size;
	zone->size += inst->size;
	// debug("to %zu->%zu\n", zone->offset, zone->offset + inst->size);

}

uint8_t		can_i_write(t_packer *packer, t_zone *zone, t_btc *inst)
{
	// t_zone		*zone;
	uint8_t		valid_zone;

	// zone = zone_list->data;
	valid_zone = FALSE;
	logging_recursive("%s:\t%zu <= %zu ? Load:%hhd Exec:%hhd Last:%hhd\n", __func__, inst->size, zone->size, (zone->phdr->p_type == PT_LOAD), (zone->phdr->p_flags & PF_X), (TRUE == zone->last));
	if (packer->strategy == STRAT_LOADABLE_EXECUTE)
	{
		logging_recursive("%s:\tSTRAT_LOADABLE_EXECUTE\n", __func__);
		if ((zone->phdr->p_type == PT_LOAD) && (zone->phdr->p_flags & PF_X))
		{
			valid_zone = TRUE;
		}
	}
	else if (packer->strategy == STRAT_LOADABLE)
	{
		logging_recursive("%s:\tSTRAT_LOADABLE\n", __func__);
		if (zone->phdr->p_type == PT_LOAD)
		{
			valid_zone = TRUE;
		}
	}
	else if (packer->strategy == STRAT_LOADABLE_LAST_SEGMENT)
	{
		logging_recursive("%s:\tSTRAT_LOADABLE_LAST_SEGMENT\n", __func__);
		if (TRUE == zone->last)
		{
			valid_zone = TRUE;
		}
	}
	else
	{
		logging_recursive("%s:\tERROR: UNKNOWN STRAT\n", __func__);
	}

	return ((TRUE == valid_zone) && (inst->size)<= zone->size);
}

void		write_btc(t_btc *inst, t_zone *zone, t_packer *packer)
{
	uint8_t		*dest;
	
	dest = zone->offset + packer->content;
	logging_recursive("%s: %s in phdr nb*%d\n", __func__, btc_to_str(inst), get_program_header_index(packer, zone->phdr));
	logging_recursive("%s: localisation: [%lx - %lx] size: %lx\n", __func__, zone->offset, zone->offset + inst->size, inst->size);
	inst->func_ptr(dest, inst->args);
	zone->phdr->p_filesz += inst->size;
	zone->phdr->p_memsz += inst->size;
	return;
}

uint8_t		is_btc_headless(t_btc *btc)
{
	if (btc->type == BTC_CALL_JMP)
		return (TRUE);
	if (btc->type == BTC_DEF_INIT_PERM)
		return (TRUE);
	if (btc->type == BTC_DEF_KEY_SCHED)
		return (TRUE);
	if (btc->type == BTC_DEF_FIND_ABS_VADDR)
		return (TRUE);
	if (btc->type == BTC_DEF_CYPHER)
		return (TRUE);
	return (FALSE);
}

ssize_t		bytecode_inject(t_packer *packer, t_list *zones, t_zone *zone, t_dlist *inst)
{
	uint8_t	headless;
	ssize_t ret;

	update_arg_def_crypt_calls(inst, zone);
	update_zone(zone, inst->data);
	headless = is_btc_headless((t_btc *)inst->data);
	ret = solve_bytecodes(packer, zones, inst->next, headless);
	undo_update_zone(zone, ((t_btc *)inst->data));
	if (ret != FAILURE)
	{
		update_args(packer, ((t_btc *)inst->data), zone, ret);
		write_btc(inst->data, zone, packer);
		zone->used = TRUE;
		ret = zone->offset;
		if (((t_btc*)inst->data)->type == BTC_DEF_BEGIN)
		{
			packer->new_e_entry = zone->vaddr;
			logging_recursive("%s: New e_entry will be\t 0x%lx\n", __func__, packer->new_e_entry);
		}
	}
	depth -= 1;
	logging_recursive("\n");
	return (ret);
}

// depth is used for pretty printing
ssize_t		solve_bytecodes(t_packer *packer, t_list *zones, t_dlist *inst, int headless)
{
	t_list	*zone_list = packer->caves;
	t_zone	*zone;
	ssize_t ret;
	t_dlist	*jmp;

	depth += 1;
	logging_recursive("\n");
	if (inst == NULL)
	{
		logging_recursive("%s: NULL INST -> all bytecodes can be placed. Binary is solved\n\n", __func__);
		depth -= 1;
		return (((Elf64_Ehdr *)packer->content)->e_entry);
	}
	logging_recursive("%s: INST: \t%s\n", __func__, btc_to_str(((t_btc *)inst->data)));
	if (headless == TRUE)
	{
		logging_recursive("%s: HEADLESS:\tTRUE\n", __func__);
		while (zone_list != NULL)
		{
			ret = FAILURE;
			zone = zone_list->data;
			logging_recursive("%s: try offset\t%zu\n", __func__, zone->offset);
			if (can_i_write(packer, zone, inst->data))
			{
				logging_recursive("%s: valid write!\n", __func__);
				ret = bytecode_inject(packer, zone_list, zone, inst);
			}
			if (ret != FAILURE)
			{
				return (ret);
			}
			zone_list = zone_list->next;
		}
		return (FAILURE);
	}
	else
	{
		logging_recursive("%s: HEADLESS:\tFALSE\n", __func__);
		zone = zones->data;
		logging_recursive("%s: Current offset:\t%zu\n", __func__, zone->offset);
		if (can_i_write(packer, zone, inst->data))
		{
			logging_recursive("%s: valid write!\n", __func__);
			return (bytecode_inject(packer, zones, zone, inst));
		}
		else
		{
			logging_recursive("%s: NOT valid write!\n", __func__);
			logging_recursive("%s: Trying jump\n", __func__);
			ret = FAILURE;
			if (NULL == (jmp = ft_dlist_new(create_btc(BTC_CALL_JMP))))
			{
				print_error(packer->self_path, MALLOC_ERROR);
				exit(EXIT_FAILURE);
			}
			ft_dlist_insert_next_wesh(inst->prev, jmp);
			if (can_i_write(packer, zone, jmp->data))
			{
				logging_recursive("%s: Jump ok\n", __func__);
				ret = bytecode_inject(packer, zones, zone, jmp);
			}
			if (ret == FAILURE)
			{
				logging_recursive("%s: Removing jump\n", __func__);
				jmp = ft_dlist_cut_unsafe(jmp);
				free_btc(jmp->data);
				free(jmp);
			}
			return (ret);
		}
	}
}