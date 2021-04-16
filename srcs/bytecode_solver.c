/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_solver.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 11:11:34 by ezalos            #+#    #+#             */
/*   Updated: 2021/04/16 18:14:56 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int depth = -1;

void		update_zone(t_zone *zone, t_btc *inst)
{
	// printed boundary is included
	debug_recursive("%s: zone: %p, inst %p\n", __func__, zone, inst);
	debug_recursive("%s: Updating zone:\t %zu->%zu\n", __func__, zone->offset, zone->offset + inst->size);
	zone->offset += inst->size;
	zone->vaddr += inst->size;
	zone->size -= inst->size;
	// debug("to %zu->%zu\n", zone->offset, zone->offset + inst->size);
}

void		undo_update_zone(t_zone *zone, t_btc *inst)
{
	debug_recursive("%s: Updating zone (undo):\t %zu->%zu\n", __func__, zone->offset, zone->offset + inst->size);
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
	logging_recursive("%s:\t %zu <= %zu ?\n", __func__, inst->size, zone->size);
	// print_zone(zone);
	logging_recursive("%s:\tLoad %hhd Exec %hhd Last %hhd\n", __func__, (zone->phdr->p_type == PT_LOAD), (zone->phdr->p_flags & PF_X), (TRUE == zone->last));
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

	return ((TRUE == valid_zone) && (inst->size <= zone->size));
}

void		write_btc(t_btc *inst, t_zone *zone, t_packer *packer)
{
	//TODO : todo
	uint8_t		*dest;
	
	logging_recursive("Enter write btc\n");
	dest = zone->offset + packer->content;
	logging_recursive("%s:\t\t %s\n", __func__, btc_to_str(inst));
	logging_recursive("VADDR:\t\t\t 0x%lx\n", zone->vaddr);
	logging_recursive("Writing at offset:\t 0x%lx (%ld)\n", zone->offset, zone->offset);
	logging_recursive("Btc localisation:\t [%lx - %lx]\n", zone->offset, zone->offset + inst->size);
	logging_recursive("Size is:\t\t %zu\n", inst->size);
	logging_recursive("It's in phdr nb\t\t %d\n", get_program_header_index(packer, zone->phdr));
	inst->func_ptr(dest, inst->args);
	zone->phdr->p_filesz += inst->size;
	zone->phdr->p_memsz += inst->size;
	logging_recursive("%s: END OF WRITE\n", __func__);
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

	logging_recursive("%s\n", __func__);
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
	logging_recursive("%s: end bytecode_inject\n\n", __func__);
	depth -= 1;
	return (ret);
}

// depth is used for pretty printing
ssize_t		solve_bytecodes(t_packer *packer, t_list *zones, t_dlist *inst, int headless)
{
	t_list	*zone_list = zones;
	t_zone	*zone;
	ssize_t ret;
	t_dlist	*jmp;

	// depth for aestethics 
	depth += 1;
	logging_recursive("\n");
	logging_recursive("%s\n", __func__);
	if (inst == NULL)
	{
		logging_recursive("Inst is NULL -> WE FOUND THE SOLUTION!!\n\n");
		depth -= 1;
		return (((Elf64_Ehdr *)packer->content)->e_entry);
	}
	logging_recursive("Trying to place\t %s\n", btc_to_str(((t_btc *)inst->data)));
	if (headless == TRUE)
	{
		logging_recursive("Headless\t TRUE\n");
		while (zone_list != NULL)
		{
			ret = FAILURE;
			zone = zone_list->data;
			logging_recursive("Try at offset\t %zu\n", zone->offset);
			if (can_i_write(packer, zone, inst->data))
			{
				logging_recursive("We can write\n");
				ret = bytecode_inject(packer, zones, zone, inst);
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
		logging_recursive("Headless\t FALSE\n");
		// ICI EST LE BUG Zone lorsque headless, ne devrait pas estre le premier element mais l'element choisi a l'etape precedente
		zone = zone_list->data;
		logging_recursive("Current offset:\t %zu\n", zone->offset);
		if (can_i_write(packer, zone, inst->data))
		{
			logging_recursive("We can write.\n");
			return (bytecode_inject(packer, zones, zone, inst));
		}
		else
		{
			logging_recursive("Cannot write instruction, trying jump\n");
			ret = FAILURE;
			jmp = ft_dlist_new(create_btc(BTC_CALL_JMP));
			ft_dlist_insert_next_wesh(inst, jmp);
			if (can_i_write(packer, zone, jmp->data))
			{
				logging_recursive("Jump ok\n");
				ret = bytecode_inject(packer, zones, zone, jmp);
			}
			jmp = ft_dlist_cut_unsafe(jmp);
			free_btc(jmp->data);		// TODO: keep jump rights phdr ?
			ft_dlist_free(jmp, NULL);
			return (ret);
		}
	}
}