/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gather.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 23:23:07 by ezalos            #+#    #+#             */
/*   Updated: 2021/04/15 17:41:14 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void		zones_add_rights(t_list *zone, Elf64_Word rights)
{
	while (zone)
	{
		((t_zone*)zone->data)->phdr->p_flags |= rights;
		zone = zone->next;
	}
}


int8_t		gather_all_infos(t_packer *packer)
{
	cave_gathering(packer);

	packer->to_crypt = get_zones(packer, PT_LOAD, PF_X | PF_R, &data_filler_zone_to_crypt);
	packer->caves = get_zones(packer, PT_LOAD, PF_X | PF_R, &data_filler_cave);

	if (debug_level == 2)
	{
		print_zones(packer->to_crypt);
		print_zones(packer->caves);
	}

	zones_add_rights(packer->to_crypt, PF_W);

	if (FALSE == (packer->to_crypt && packer->caves))
		return (FAILURE);
	//just for test purpose:
	((t_zone*)(packer->to_crypt->data))->phdr->p_flags = 7;
	return (SUCCESS);
}