/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_segment_strategy.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 16:36:22 by rkirszba          #+#    #+#             */
/*   Updated: 2021/04/16 17:52:28 by ezalos           ###   ########.fr       */
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
		zone->phdr = (Elf64_Phdr*)((size_t)zone->phdr + offset);
	}
}

void		extend_file(t_packer *packer, size_t extension)
{
	uint8_t	*new_content;

	if ((new_content = (uint8_t *)mmap(0,
				(size_t)packer->size + extension,
				PROT_READ | PROT_WRITE,
				MAP_ANONYMOUS | MAP_PRIVATE,
				-1,
				0)) == MAP_FAILED)
	{
		print_error(packer->self_path, strerror(errno));
		exit(EXIT_FAILURE);
	}
	ft_memcpy(new_content, packer->content, packer->size);
	update_phdr_addr(packer, (int64_t)((size_t)new_content - (size_t)packer->content));
	//TODO munmap
	packer->content = new_content;
	packer->size += extension;
}

void		prepare_last_segment_strategy(t_packer *packer,
	size_t size_blueprints)
{
	t_zone	*zone;
	
	((Elf64_Ehdr*)packer->content)->e_shnum = 0;
	((Elf64_Ehdr*)packer->content)->e_shstrndx = 0;
	((Elf64_Ehdr*)packer->content)->e_shoff = 0;
	if (!(zone = get_last_zone(packer->caves)))
		return ;
	zone->size = packer->size - zone->offset;
	if (zone->offset > packer->size)
	{
		extend_file(packer, (zone->offset - packer->size) + size_blueprints);
		zone->size = size_blueprints;
	}
	else if (size_blueprints > zone->size)
	{
		extend_file(packer, size_blueprints - zone->size);
		zone->size = size_blueprints;
	}
	if (zone->phdr->p_filesz < zone->phdr->p_memsz)
	{
		ft_bzero(packer->content + zone->phdr->p_offset + zone->phdr->p_filesz,
				 zone->phdr->p_memsz - zone->phdr->p_filesz);
		zone->phdr->p_filesz = zone->phdr->p_memsz;
	}
	zone->last = TRUE;
}