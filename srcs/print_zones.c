/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_zones.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 22:26:43 by ezalos            #+#    #+#             */
/*   Updated: 2021/04/16 17:33:48 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	print_zone(t_zone *zone)
{
	printf("offset : %lu\n", zone->offset);
	printf("vaddr  : %lu\n", zone->vaddr);
	printf("size   : %lu\n", zone->size);
	print_program_header(zone->phdr);
	printf("\n");
}

void	print_zones(t_list *zones)
{
	printf("PRINT ZONE\n");
	while (zones)
	{
		printf("\n");
		print_zone(zones->data);
		zones = zones->next;
	}
}