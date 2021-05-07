/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_zones.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 22:26:43 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/07 12:02:25 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	print_zone(t_zone *zone)
{
	printf("ZONE:     ");
	printf("offset: %-10lu ", zone->offset);
	printf("vaddr : %-10lu ", zone->vaddr);
	printf("size : %s%-10lu%s\n", _YELLOW, zone->size, _RESET);
	print_program_header(zone->phdr);
}

void	print_zones(t_list *zones)
{
	while (zones)
	{
		print_zone(zones->data);
		zones = zones->next;
	}
}