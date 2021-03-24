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

t_dlist		*blueprint_creation(t_packer *packer)
{
	t_dlist *blueprint;
	t_dlist *inst;

	(void)packer;
	blueprint = ft_dlist_new(create_btc(BTC_DEF_WRITE));
	inst = ft_dlist_new(create_btc(BTC_CALL_JMP));
	ft_dlist_append(*blueprint, inst);
	return (blueprint);
}

ssize_t		chirurgy(t_packer *packer)
{
	t_dlist	*blueprint;
	ssize_t ret;

	printf("\n*** %s: Creating blueprint\n", __func__);
	blueprint = blueprint_creation(packer);

	printf("\n*** %s: Solving injection\n", __func__);
	ret = solve_bytecodes(packer, packer->caves, NULL, blueprint, TRUE);
	if (ret != FAILURE)
		((Elf64_Ehdr *)packer->content)->e_entry = ret;
	printf("\n*** %s: ret= %ld\n", __func__, ret);
	return (ret);
}