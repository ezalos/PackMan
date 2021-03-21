/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_btc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 11:11:56 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/21 11:10:51 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

t_btc	*create_btc(int type)
{
	t_btc *btc;
	if (!(btc = malloc(sizeof(t_btc))))
		return (NULL);
	ft_memset(btc, 0, sizeof(t_btc));
	ft_memcpy(btc, &bytecode_lib[type], sizeof(t_btc));
	if (!(btc->args = malloc(sizeof(t_btc_args))))
	{
		free(btc);
		return (NULL);
	}
	ft_memset(btc->args, 0, sizeof(t_btc_args));
	return (btc);
}

void	free_btc(t_btc *btc)
{
	free(btc->args);
	free(btc);
}