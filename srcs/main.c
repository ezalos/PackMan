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

int			main(int ac, char **av)
{
	t_packer	packer;

	if (ac < 2)
		return (print_usage(av[0]));

	packer.print_phdr_gather = TRUE;
	//TODO: Free all at end && waterfall of if not failure
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


// TODO a free:
// btc + btc_args (free_btc)
// caves
// to_crypt
// **phdr_array
// **shdr_array
// a l'interieur du t_pheader, le sheader tree
// *phdr_tree ??
// munmap