/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_zones.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 17:54:59 by rkirszba          #+#    #+#             */
/*   Updated: 2021/03/25 21:11:14 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void			data_filler_zone_to_crypt(t_pheader *hdr, t_zone *zone)
{
	zone->offset = hdr->phdr->p_offset;
	zone->vaddr = hdr->phdr->p_vaddr;
	zone->size = hdr->phdr->p_filesz;
	zone->phdr = hdr->phdr; // probablement inutile dans le cas des zones a crypter
}

void			data_filler_cave(t_pheader *hdr, t_zone *zone)
{
	zone->offset = hdr->phdr->p_offset + hdr->phdr->p_filesz;
	zone->vaddr = hdr->phdr->p_vaddr + hdr->phdr->p_filesz; // hesitation avec + p_memsz
	zone->size = hdr->available_size;
	zone->phdr = hdr->phdr;
}

static t_list	*create_zone(t_pheader *hdr, void (*data_filler)(t_pheader*, t_zone*))
{
	t_list	*node;
	t_zone	*zone;

	if (!(zone = (t_zone*)malloc(sizeof(*zone))))
		return (NULL);
	ft_memset(zone, 0, sizeof(t_zone));
	data_filler(hdr, zone);
	if (!(node = ft_list_new((void*)zone)))
	{
		free(zone);
		return (NULL);
	}
	return (node);
}

t_list			*get_zones(t_packer *packer, uint8_t type, uint8_t flags,
	void (*data_filler)(t_pheader*, t_zone*))
{
	uint8_t		i;
	t_pheader	*hdr;
	t_list		*zones;
	t_list		*zone;

	zones = NULL;
	i = -1;
	while ((hdr = (packer->phdr_array)[++i]))
	{
		if (hdr->phdr->p_type == type
			&& (hdr->phdr->p_flags & flags) == flags)
		{
			if (!(zone = create_zone(hdr, data_filler)))
			{
				print_error(packer->self_path, strerror(errno));
				//fonction qui free tout
				exit(EXIT_FAILURE);
			}
			ft_list_append(&zones, zone);
		}
	}
	return (zones);
}
