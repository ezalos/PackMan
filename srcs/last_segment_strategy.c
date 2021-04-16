/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_segment_strategy.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 16:36:22 by rkirszba          #+#    #+#             */
/*   Updated: 2021/04/14 16:36:22 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

// il faut redefinir le calcul des sizes et offset par rapport à memsz et pas filesz

t_zone		*get_last_zone(t_list *caves)
{
	while (caves)
	{
		if (!caves->next)
			return ((t_zone*)(caves->data));
		caves = caves->next;
	}
	return (NULL);
}

void		update_phdr_addr(t_packer *packer, int64_t offset)
{
	// attention faire tres attention aux duplications !
	// si on ne s'occupe que des caves, il faut update les droits des zones to_crypt auparavant

	//TODO: dans l'optique de mofifs ulterieures, mettre a jour t_pheader


	t_list	*curs;
	t_zone	*zone;

	curs = packer->caves;
	while (curs)
	{
		zone = (t_zone*)(curs->data);
		zone->phdr += offset;
		curs = curs->next;
	}
}

void		extend_file(t_packer *packer, size_t extension)
{
	uint8_t	*new_content;

	if ((new_content = (uint8_t *)mmap(0,
				(size_t)packer->size + extension,
				PROT_READ | PROT_WRITE,
				MAP_PRIVATE,
				-1,
				0)) == MAP_FAILED)
	{
		//TODO appel fonction erreur + free
		exit(EXIT_FAILURE);
	}
	ft_memcpy(new_content, packer->content, packer->size);
	update_phdr_addr(packer, (int64_t(new->content - content));
	//TODO munmap
	packer->content = new_content;
	packer->size += extension;
}

int8_t		prepare_last_segment_strategy(t_packer *packer,
	size_t size_blueprints)
{
	t_zone	*zone;
	
	if (!(zone = get_last_zone(packer->caves)))
		return (FAILURE);
	
	zone->size = packer->size - zone->offset;
	if (zone->offset > packer->size)
	{
		extend_file(packer, (zone_offset - packer->size) + size_blueprints)
		zone->size = size_blueprints;
	}
	else if (size_blueprints > zone->size)
	{
		extend_file(packer, size_blueprints - zone->size);
		zone->size = size_blueprints;
	}
	
	// alternative si on est surs qu'il y a un type int128_t

	// int128_t	remaining_size;
	// remaining_size = packer->size - zone->offset;
	// if (size_blueprints > remaining_size)
	// {
	// 	extend_file(packer, size_blueprints - remaining_size);
	// 	zone->size = size_blueprints;
	// }
	// else
	// 	zone->size = reamining_size;

		// fin alternative
	if (zone->phdr->filesz < zone->phdr->memsz)
	{
		ft_bzero(packer->content + zone->phdr->offset + zone->phdr->filesz,
			zone->phdr->memsz - zone->phdr->filesz);
		zone->phdr->filesz = zone->phdr->memsz;
	}

}