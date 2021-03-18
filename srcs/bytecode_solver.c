/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_solver.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 11:11:34 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/18 11:16:44 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

ssize_t		bytecode_inject(t_packer *packer, t_list *zones, t_zone *zone, t_btc *inst)
{
	ssize_t ret;

	if (inst->type == BTC_DEF_CRYPT)
	{
		update_arg_crypt_calls(inst, zone);
	}
	update_zone(zone, inst);
	ret = solve_bytecodes(packer, zones, zone, inst->next, inst->type == BTC_JMP);
	if (ret != FAILURE)
	{
		update_args(ret, inst);
		write_btc(inst, zone, packer);
	}
	else
	{
		undo_update_zone(zone, inst);
	}
	return (ret);
}

ssize_t		solve_bytecodes(t_packer *packer, t_list *zones, t_zone *current_zone, t_btc *inst, int headless)
{
	t_list *zone_list = zones;
	t_zone *zone;
	ssize_t ret;
	t_btc *jmp;

	if (inst == NULL)
		return (current_zone->offset);

	if (headless)
	{
		while (zone_list != NULL)
		{
			zone = zone_list->data;
			if (can_i_write(zone, inst))
				ret = bytecode_inject(packer, zones, zone, inst);
			if (ret != FAILURE)
				return (ret);
			zone_list = zone_list->next;
		}
		return (FAILURE);
	}
	else
	{
		zone = zone_list->data;
		if (can_i_write(zone, inst))
			return (bytecode_inject(packer, zones, zone, inst));
		else
		{
			ret = FAILURE;
			jmp = create_btc(BTC_JMP);
			jmp->next = inst->next;
			if (can_i_write(zone, jmp))
				ret = bytecode_inject(packer, zones, zone, jmp);
			free_btc(jmp);
			return (ret);
		}
	}
}