/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gather.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 23:23:07 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/28 20:31:36 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t		gather_all_infos(t_packer *packer)
{
	cave_gathering(packer);

	packer->to_crypt = get_zones(packer, PT_LOAD, PF_X | PF_R, &data_filler_zone_to_crypt);
	packer->caves = get_zones(packer, PT_LOAD, PF_X | PF_R, &data_filler_cave);

	if (debug_level == 2)
	{
		print_zones(packer->to_crypt);
		print_zones(packer->caves);
	}

	if (FALSE == (packer->to_crypt && packer->caves))
		return (FAILURE);
	return (SUCCESS);
}