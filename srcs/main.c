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
	int			ret;

	if (ac < 2)
		return (print_usage(av[0]));

	if ((ret = init(&packer, av)) != FAILURE)
		if ((ret = gather_all_infos(&packer)) != FAILURE)
			if ((ret = chirurgy(&packer)) != FAILURE)
				ret = save_woody(&packer);

	free_all(&packer);

	return (ret == FAILURE ? EXIT_FAILURE : EXIT_SUCCESS);
}
