/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_solver.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 11:11:34 by ezalos            #+#    #+#             */
/*   Updated: 2021/04/03 20:01:32 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int depth = -1;

void		print_btc_name(t_btc *inst)
{
	if (inst->type == BTC_CALL_JMP)
		printf("BTC_CALL_JMP\n");
	else if (inst->type == BTC_CALL_MPROTECT)
		printf("BTC_CALL_MPROTECT\n");
	else if (inst->type == BTC_CALL_CYPHER)
		printf("BTC_CALL_CYPHER\n");
	else if (inst->type == BTC_DEF_WRITE)
		printf("BTC_DEF_WRITE\n");
	else if (inst->type == BTC_DEF_CYPHER)
		printf("BTC_DEF_CYPHER\n");
	else if (inst->type == BTC_DEF_INIT_PERM)
		printf("BTC_DEF_INIT_PERM\n");
	else if (inst->type == BTC_DEF_KEY_SCHED)
		printf("BTC_DEF_KEY_SCHED\n");
	else if (inst->type == BTC_DEF_BEGIN)
		printf("BTC_DEF_BEGIN\n");
	else if (inst->type == BTC_DEF_CYPHER_PREPARE)
		printf("BTC_DEF_CYPHER_PREPARE\n");
	else if (inst->type == BTC_DEF_END)
		printf("BTC_DEF_END\n");
	else
		dprintf(2, "Error: Unknown btc type\n");
}

void		update_zone(t_zone *zone, t_btc *inst)
{
	// printed boundary is included
	debug_recursive("Updating zone:\t %zu->%zu ", zone->offset, zone->offset + zone->size);
	zone->offset += inst->size;
	zone->vaddr += inst->size;
	zone->size -= inst->size;
	debug("to %zu->%zu\n", zone->offset, zone->offset + zone->size);
}

void		undo_update_zone(t_zone *zone, t_btc *inst)
{
	debug_recursive("Updating zone (undo):\t %zu->%zu ", zone->offset, zone->offset + zone->size);
	zone->offset -= inst->size;
	zone->vaddr -= inst->size;
	zone->size += inst->size;
	debug("to %zu->%zu\n", zone->offset, zone->offset + zone->size);

}

uint8_t		can_i_write(t_zone *zone, t_btc *inst)
{
	logging_recursive("%s:\t %zu <= %zu ?\n", __func__, inst->size, zone->size);

	return (inst->size <= zone->size);
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
	if (btc->type == BTC_DEF_CYPHER)
		return (TRUE);
	if (btc->type == BTC_DEF_FIND_ABS_VADDR)
		return (TRUE);		
	return (FALSE);
}


int	is_over_place_bytecode(t_dlist *inst)
{
	if (inst == NULL)
	{
		logging_recursive("Inst is NULL -> WE FOUND THE SOLUTION!!\n\n");
		depth -= 1;
		return (TRUE);
	}
	return (FALSE);
}


Elf64_Addr get_entrypoint_from_packer(t_packer *packer)
{
	return (((Elf64_Ehdr *)packer->content)->e_entry);
}


t_btc	*get_previous_inst_btc(t_dlist *inst)
{
	if (inst->prev == NULL)
	{
		return (NULL);
	}
	else
	{
		return (((t_btc *)inst->prev->data));
	}
}


int	is_currently_headless(t_dlist *inst)
{
	if (get_previous_inst_btc(inst) == NULL || is_btc_headless(get_previous_inst_btc(inst)))
	{
		return (TRUE);
	}
	else
	{
		return (FALSE);
	}
}


ssize_t		bytecode_inject(t_packer *packer, t_list *zones, t_zone *zone, t_dlist *inst)
{
	uint8_t	headless;
	ssize_t ret;

	logging_recursive("bytecode_inject\n");
	update_arg_def_crypt_calls(inst, zone);
	update_zone(zone, inst->data);
	headless = is_btc_headless((t_btc *)inst->data);
	ret = solve_bytecodes(packer, zones, inst->next, headless);
	undo_update_zone(zone, ((t_btc *)inst->data));
	if (ret != FAILURE)
	{
		update_args(packer, ((t_btc *)inst->data), zone, ret);
		write_btc(inst->data, zone, packer);
		ret = zone->offset;
		if (((t_btc*)inst->data)->type == BTC_DEF_BEGIN) // TODO : put outside of algo
		{
			packer->new_e_entry = zone->vaddr;
			logging_recursive("New e_entry will be\t 0x%lx\n", packer->new_e_entry);
		}
	}
	logging_recursive("end bytecode_inject\n\n");
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
			zone = zone_list->data;
			logging_recursive("Try at offset\t %zu\n", zone->offset);
			if (can_i_write(zone, inst->data))
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
		zone = zone_list->data;
		logging_recursive("Current offset:\t %zu\n", zone->offset);
		if (can_i_write(zone, inst->data))
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
			if (can_i_write(zone, jmp->data))
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

void	do_update_down(t_dlist *inst,t_zone *zone)
{
	logging_recursive("updating zone and crypt call args");
	update_arg_def_crypt_calls(inst, zone);
	update_zone(zone, inst->data);
	// ? validate order update_zone update_args here
}

void	undo_update_up(t_dlist *inst,t_zone *zone)
{
	logging_recursive("undoing update zone");
	undo_update_zone(zone, ((t_btc *)inst->data));
	// TODO : Undo update crypt calls ?
}


// if (((t_btc*)inst->data)->type == BTC_DEF_BEGIN) // TODO : put outside of algo
// {
// 	packer->new_e_entry = zone->vaddr;
// 	logging_recursive("New e_entry will be\t 0x%lx\n", packer->new_e_entry);
// }

void insert_jmp_in_inst_list(t_dlist *inst)
{
	ft_dlist_insert_next(NULL, inst->prev, ft_dlist_new(create_btc(BTC_CALL_JMP)));
	// TODO : Check this insert
}

void remove_jump_from_list(t_dlist *inst)
{
	inst->next->prev = inst->prev;
	inst->prev->next = inst->next;
	// ! FREE THE JUMP
}

size_t update_args_and_write_btc(t_packer *packer, t_dlist *inst, t_zone *zone, ssize_t ret)
{
	update_args(packer, ((t_btc *)inst->data), zone, ret);
	write_btc(inst->data, zone, packer);
	return (zone->offset);
}


ssize_t	sexy_place_bytecode(t_packer *packer, t_list *zones, t_dlist *inst)
{
	t_list	*zone_list = zones;
	t_zone	*zone;
	ssize_t ret;

	logging_recursive("\n");
	logging_recursive("%s\n", __func__);

	if (is_over_place_bytecode(inst))
		return get_entrypoint_from_packer(packer);

	logging_recursive("Trying to place\t %s with headless : %d \n", btc_to_str(((t_btc *)inst->data)),  is_currently_headless(inst));

	while (zone_list != NULL)
	{
		zone = zone_list->data;
		logging_recursive("Current offset:\t %zu\n", zone->offset);
		if (can_i_write(zone, inst))
		{
			logging_recursive("We can write.\n");
			do_update_down(inst, zone);
			ret = sexy_place_bytecodes(packer, zones, inst->next);
			if (ret != FAILURE)
			{
				if (((t_btc*)inst->data)->type == BTC_DEF_BEGIN) // TODO : put outside of algo
				{
					packer->new_e_entry = zone->vaddr;
					logging_recursive("New e_entry will be\t 0x%lx\n", packer->new_e_entry);
				}
				return (update_args_and_write_btc(packer, inst, zone, ret));
			}
			else
				undo_update_up(inst, zone);
		}
		if (!is_currently_headless(inst) && ((t_btc *)inst->data)->type == BTC_CALL_JMP)
			break;
		else if (is_currently_headless(inst))
			zone_list = zone_list->next;
	}
	if (!is_currently_headless(inst) && ((t_btc *)inst->data)->type != BTC_CALL_JMP && inst->prev != NULL)
	{
		insert_jmp_in_inst_list(inst);
		inst = inst->prev;
		print("should be btc jump %s\n", btc_to_str(((t_btc *)inst->data)));
		logging_recursive("Can't write tring jump");
		if (can_i_write(zone, inst))
		{
			logging_recursive("We can write.\n");
			do_update_down(inst, zone);
			ret = sexy_place_bytecodes(packer, zones, inst->next);
			if (ret != FAILURE)
				return (update_args_and_write_btc(packer, inst, zone, ret));
			else
			{
				undo_update_up(inst, zone);
				remove_jump_from_list(inst);
				inst = inst->next; // ? Useless but pretty
			}
		}
	}
	return (FAILURE);
}