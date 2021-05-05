/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gather.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 23:23:07 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/03 19:16:23 by ezalos           ###   ########.fr       */
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
		logging("Just added rights to cave\n");
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
	if (TRUE == cave_gathering(packer))
		return (FAILURE);

	packer->to_crypt = get_zones(packer, PT_LOAD, PF_X | PF_R, &data_filler_zone_to_crypt);
	if (!(packer->to_crypt))
		return (print_error(packer->self_path, NO_EXECUTABLE_SEGMENT));
	// packer->to_crypt = get_zones(packer, PT_LOAD, 0, &data_filler_zone_to_crypt);
	packer->caves = get_zones(packer, PT_LOAD, 0, &data_filler_cave);
	if (!(packer->caves))
		return (print_error(packer->self_path, NO_LOADABLE_SEGMENT));

	// TODO: undefine behavior ou on laisse tel quel
	// TODO: louis check system ABI V if 1st load + exec is necessarly .text
	
	ft_list_free(packer->to_crypt->next, &free_data);
	packer->to_crypt->next = NULL;

	if (debug_level == 2)
	{
		// print_zones(packer->to_crypt);
		print_zones(packer->caves);
	}


	// packer->to_crypt->next = NULL;
	respect_sacred_memory_size(packer, &(packer->to_crypt));
	// We had rights now, because it imply the 3rd startegy
	// when we neeed to reallocate a bigger memory size.
	// Because we can reference same zone twice between caves and to_crypt
	// And our method of memory reference update cant work if it is the
	// So we need to not have to use to_crypt after new mmap, making
	// rights update the right thing to do now. 
	zones_add_rights_to_crypt(packer->to_crypt, PF_W);
	return (SUCCESS);
}
