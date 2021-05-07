/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gather.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 23:23:07 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/07 13:17:42 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void		zones_add_rights_to_crypt(t_list *zone, Elf64_Word rights)
{
	while (zone)
	{
		((t_zone*)zone->data)->phdr->p_flags |= rights;
		zone = zone->next;
		logging("*** %s: rights added to zone to crypt\n", __func__);
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

void		respect_sacred_memory_size(t_packer *packer, t_list **to_crypt)
{
	t_list	*tmp;
	t_zone	*zone;

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
	logging("\n* GATHER\n");
	if (TRUE == cave_gathering(packer))
		return (FAILURE);

	// TODO: in both case check if it has returned null
	// error1: no executable segments
	// error2: no loadable segments
	packer->to_crypt = get_zones(packer, PT_LOAD, PF_X | PF_R, &data_filler_zone_to_crypt);
	// packer->to_crypt = get_zones(packer, PT_LOAD, 0, &data_filler_zone_to_crypt);
	packer->caves = get_zones(packer, PT_LOAD, 0, &data_filler_cave);

	if (FALSE == (packer->to_crypt && packer->caves))
		return (FAILURE); // a revoir
	// TODO: undefine behavior ou on laisse tel quel
	// TODO: louis check system ABI V if 1st load + exec is necessarly .text
	// TODO: sinon free a partir de packer->to_crypt->next + next = NULL
	packer->to_crypt->next = NULL;

	if (debug_level >= 1)
	{
		logging("** %s: Potential insertion zones\n", __func__);
		print_zones(packer->caves);
	}


	// packer->to_crypt->next = NULL;
	logging("** %s: Respecting sacred memory\n", __func__);
	respect_sacred_memory_size(packer, &(packer->to_crypt));

	// Explanation to keep:
	// We had rights now, because it imply the 3rd startegy
	// when we neeed to reallocate a bigger memory size.
	// Because we can reference same zone twice between caves and to_crypt
	// And our method of memory reference update cant work if it is the
	// So we need to not have to use to_crypt after new mmap, making
	// rights update the right thing to do now.
	zones_add_rights_to_crypt(packer->to_crypt, PF_W);

	if (debug_level >= 1)
	{
		logging("** %s: Adjusted zones to crypt\n", __func__);
		print_zones(packer->to_crypt);
	}

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
