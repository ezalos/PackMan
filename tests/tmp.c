// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   tmp.c                                              :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2021/04/16 23:49:16 by ezalos            #+#    #+#             */
// /*   Updated: 2021/04/17 00:03:00 by ezalos           ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "head.h"

// int depth = -1;

// int			is_over_place_bytecode(t_dlist *inst)
// {
// 	if (inst == NULL)
// 	{
// 		logging_recursive("Inst is NULL -> WE FOUND THE SOLUTION!!\n\n");
// 		depth -= 1;
// 		return (TRUE);
// 	}
// 	return (FALSE);
// }

// Elf64_Addr	get_entrypoint_from_packer(t_packer *packer)
// {
// 	return (((Elf64_Ehdr *)packer->content)->e_entry);
// }

// t_btc		*get_previous_inst_btc(t_dlist *inst)
// {
// 	if (inst->prev == NULL)
// 	{
// 		return (NULL);
// 	}
// 	else
// 	{
// 		return (((t_btc *)inst->prev->data));
// 	}
// }

// int			is_currently_headless(t_dlist *inst)
// {
// 	if (get_previous_inst_btc(inst) == NULL || is_btc_headless(get_previous_inst_btc(inst)))
// 	{
// 		return (TRUE);
// 	}
// 	else
// 	{
// 		return (FALSE);
// 	}
// }

// void		do_update_down(t_dlist *inst, t_zone *zone)
// {
// 	logging_recursive("updating zone and crypt call args");
// 	update_arg_def_crypt_calls(inst, zone);
// 	update_zone(zone, inst->data);
// 	// ? validate order update_zone update_args here
// }

// void		undo_update_up(t_dlist *inst, t_zone *zone)
// {
// 	logging_recursive("undoing update zone");
// 	undo_update_zone(zone, ((t_btc *)inst->data));
// 	// TODO : Undo update crypt calls ?
// }

// void		insert_jmp_in_inst_list(t_dlist *inst)
// {
// 	ft_dlist_insert_next(NULL, inst->prev, ft_dlist_new(create_btc(BTC_CALL_JMP)));
// 	// TODO : Check this insert
// }

// void		remove_jump_from_list(t_dlist *inst)
// {
// 	inst->next->prev = inst->prev;
// 	inst->prev->next = inst->next;
// 	// TODO! FREE THE JUMP
// }

// size_t		update_args_and_write_btc(t_packer *packer, t_dlist *inst, t_zone *zone, ssize_t ret)
// {
// 	update_args(packer, ((t_btc *)inst->data), zone, ret);
// 	write_btc(inst->data, zone, packer);
// 	return (zone->offset);
// }

// void		handle_first_def(t_packer *packer, t_zone *zone, t_dlist *inst)
// {
// 	if (((t_btc *)inst->data)->type == BTC_DEF_BEGIN) // TODO : put outside of algo
// 	{
// 		packer->new_e_entry = zone->vaddr;
// 		logging_recursive("New e_entry will be\t 0x%lx\n", packer->new_e_entry);
// 	}
// }

// ssize_t		sexy_place_bytecode(t_packer *packer, t_list *zones, t_dlist *inst)
// {
// 	t_list *zone_list = zones;
// 	t_zone *zone;
// 	ssize_t ret;

// 	logging_recursive("\n");
// 	logging_recursive("%s\n", __func__);
// 	depth += 1;
// 	if (is_over_place_bytecode(inst))
// 		return get_entrypoint_from_packer(packer);
// 	logging_recursive("%s:\tTrying to place\t %s with headless : %d \n", __func__, btc_to_str(((t_btc *)inst->data)), is_currently_headless(inst));
// 	while (zone_list != NULL)
// 	{
// 		zone = zone_list->data;
// 		logging_recursive("%s:\tCurrent offset:\t %zu\n", __func__, zone->offset);
// 		if (can_i_write(zone, inst->data))
// 		{
// 			logging_recursive("%s:\tWe can write.\n", __func__);
// 			do_update_down(inst, zone);
// 			ret = sexy_place_bytecode(packer, zones, inst->next);
// 			undo_update_up(inst, zone);
// 			if (ret != FAILURE)
// 			{
// 				handle_first_def(packer, zone, inst);
// 				depth -= 1;
// 				return (update_args_and_write_btc(packer, inst, zone, ret));
// 			}
// 		}
// 		if (!is_currently_headless(inst) && ((t_btc *)inst->data)->type == BTC_CALL_JMP)
// 			break;
// 		else if (is_currently_headless(inst))
// 			zone_list = zone_list->next;
// 	}
// 	if (!is_currently_headless(inst) && ((t_btc *)inst->data)->type != BTC_CALL_JMP && inst->prev != NULL)
// 	{
// 		insert_jmp_in_inst_list(inst);
// 		inst = inst->prev;
// 		logging_recursive("%s:\tCan't write, trying jump", __func__);
// 		if (can_i_write(zone, inst->data))
// 		{
// 			logging_recursive("%s:\tWe can write.\n", __func__);
// 			do_update_down(inst, zone);
// 			ret = sexy_place_bytecode(packer, zones, inst->next);
// 			if (ret != FAILURE)
// 			{
// 				depth -= 1;
// 				return (update_args_and_write_btc(packer, inst, zone, ret));
// 			}
// 			else
// 			{
// 				undo_update_up(inst, zone);
// 				remove_jump_from_list(inst);
// 				inst = inst->next; // ? Useless but pretty
// 			}
// 		}
// 	}
// 	return (FAILURE);
// }