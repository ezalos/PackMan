/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 22:26:43 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/25 22:40:02 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	print_zones(t_list *zones)
{
	t_zone *zone;

	printf("PRINT ZONE\n");
	while (zones)
	{
		printf("\n");
		zone = (t_zone *)(zones->data);
		printf("offset : %lu\n", zone->offset);
		printf("vaddr  : %lu\n", zone->vaddr);
		printf("size   : %lu\n", zone->size);
		print_program_header(zone->phdr);
		printf("\n");
		zones = zones->next;
	}
}