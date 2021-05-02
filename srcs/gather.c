/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gather.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 23:23:07 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/02 11:28:19 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void		zones_add_rights_to_crypt(t_list *zone, Elf64_Word rights)
{
	while (zone)
	{
		((t_zone*)zone->data)->phdr->p_flags |= rights;
		zone = zone->next;
		logging("Just added rights to zone to crypt\n");
	}
}

void		zones_add_rights_to_used_caves(t_list *zone, Elf64_Word rights)
{
	while (zone)
	{
		if (((t_zone *)zone->data)->used)
			((t_zone *)zone->data)->phdr->p_flags |= rights;
		zone = zone->next;
	}
}

void		check_zones_to_crypt(t_packer *packer)
{
	t_list	**to_crypt;
	t_list	*tmp;
	t_zone	*zone;

	to_crypt = &packer->to_crypt;
	while (*to_crypt)
	{
		zone = (*to_crypt)->data;
		if (zone->offset < packer->sacred_memory_size)
		{
			if (zone->offset + zone->size < packer->sacred_memory_size)
			{
				tmp = *to_crypt;
				*to_crypt = (*to_crypt)->next;
				free(tmp);
				// TODO remove node avec doubles pointeurs
				continue ;
			}
			zone->size -= (packer->sacred_memory_size - zone->offset);
			zone->offset = packer->sacred_memory_size;
		}
		to_crypt = &(*to_crypt)->next;
	}
}

int8_t		gather_all_infos(t_packer *packer)
{
	if (TRUE == cave_gathering(packer))
		return (FAILURE);

	packer->to_crypt = get_zones(packer, PT_LOAD, PF_X | PF_R, &data_filler_zone_to_crypt);
	// packer->to_crypt = get_zones(packer, PT_LOAD, 0, &data_filler_zone_to_crypt);
	packer->caves = get_zones(packer, PT_LOAD, 0, &data_filler_cave);

	packer->to_crypt->next = NULL;

	if (debug_level == 2)
	{
		// print_zones(packer->to_crypt);
		print_zones(packer->caves);
	}


	if (FALSE == (packer->to_crypt && packer->caves))
		return (FAILURE);
	// packer->to_crypt->next = NULL;
	check_zones_to_crypt(packer);
	zones_add_rights_to_crypt(packer->to_crypt, PF_W);
	//just for test purpose:
	// t_list *curs;

	// curs = packer->to_crypt;
	// while (curs)
	// {
	// 	((t_zone*)(curs->data))->phdr->p_flags = 7;
	// 	curs = curs->next;
	// }
	// curs = packer->caves;
	// while (curs)
	// {
	// 	((t_zone*)(curs->data))->phdr->p_flags = 7;
	// 	curs = curs->next;
	// }
	// ((t_zone*)(packer->to_crypt->data))->phdr->p_flags = 7;
	return (SUCCESS);
}
