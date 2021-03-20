/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/18 15:14:47 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/18 17:35:1 by erkirsabl        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

ssize_t		chirurgy(t_packer *packer)
{
	t_btc	*blueprint;
	int		ret;

	blueprint = create_btc(BTC_DEF_WRITE);
	blueprint->next = create_btc(BTC_CALL_JMP);
	ret = solve_bytecodes(packer, packer->caves, NULL, blueprint, TRUE);
	if (ret != FAILURE)
		((Elf64_Ehdr *)packer->content)->e_entry = ret;
	return (ret);
}