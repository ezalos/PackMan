/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 11:21:04 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/19 11:1:0128 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t		gather_all_infos(t_packer *packer)
{
	cave_gathering(packer);

	packer->to_crypt = get_zones(packer, PT_LOAD, PF_R, &data_filler_zone_to_crypt);
	packer->caves = get_zones(packer, PT_LOAD, PF_X | PF_R, &data_filler_cave);

	if (DEBUG == 2)
	{
		print_zones(packer->to_crypt);
		print_zones(packer->caves);
	}

	if (FALSE == (packer->to_crypt && packer->caves))
		return (FAILURE);
	return (SUCCESS);
}

int			main(int ac, char **av)
{
	t_packer	packer;

	if (ac < 2)
		return (print_usage(av[0]));

	if (FAILURE == init(&packer, av))
		return (EXIT_FAILURE);

	if (FAILURE == gather_all_infos(&packer))
		return (EXIT_FAILURE);

	if (FAILURE == chirurgy(&packer))
		return (EXIT_FAILURE);

	if (FAILURE == save_woody(&packer))
		return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}
