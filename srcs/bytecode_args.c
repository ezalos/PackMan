/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 11:13:17 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/18 11:13:28 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	update_arg_crypt_calls(t_btc *inst, t_zone *zone)
{
	while (inst)
	{
		if (inst->type == BTC_DECRYPT)
		{
			inst->args->crypt_func_addr = (void *)((size_t)zone->phdr->p_vaddr + (size_t)zone->offset);
		}
		inst = inst->next;
	}
}

void	update_args(ssize_t ret, t_btc *inst)
{
	//TODO : todo
	(void)ret;
	(void)inst;
	return;
}
