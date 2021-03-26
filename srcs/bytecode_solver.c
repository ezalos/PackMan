/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_solver.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 11:11:34 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/26 10:39:03 by ezalos           ###   ########.fr       */
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
	return (FALSE);
}

ssize_t		bytecode_inject(t_packer *packer, t_list *zones, t_zone *zone, t_dlist *inst)
{
	uint8_t	headless;
	ssize_t ret;

	if (((t_btc*)inst->data)->type == BTC_DEF_CYPHER)
	{
		update_arg_crypt_calls(inst, zone);
	}
	update_zone(zone, inst->data);
	headless = is_btc_headless((t_btc *)inst->data);
	ret = solve_bytecodes(packer, zones, inst->next, headless);
	undo_update_zone(zone, ((t_btc *)inst->data));
	if (ret != FAILURE)
	{
		update_args(((t_btc *)inst->data), zone, ret);
		write_btc(inst->data, zone, packer);
		ret = zone->offset;
		if (((t_btc*)inst->data)->type == BTC_DEF_BEGIN)
		{
			packer->new_e_entry = zone->vaddr;
			logging_recursive("New e_entry will be\t 0x%lx\n", packer->new_e_entry);
		}
	}
	depth -= 1;
	logging_recursive("\n");
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
				depth -= 1;
				return (ret);
			}
			zone_list = zone_list->next;
		}
		depth -= 1;
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
			ret = FAILURE;
			jmp = ft_dlist_new(create_btc(BTC_CALL_JMP));
			ft_dlist_insert_next_wesh(inst, jmp);
			logging_recursive("Cannot write instruction, trying jump\n");
			if (can_i_write(zone, jmp->data))
			{
				logging_recursive("Jump ok\n");
				ret = bytecode_inject(packer, zones, zone, jmp);
			}
			jmp = ft_dlist_cut_unsafe(jmp);
			free_btc(jmp->data);
			ft_dlist_free(jmp, NULL);
			depth -= 1;
			return (ret);
		}
	}
}