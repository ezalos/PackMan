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
	t_dlist	*blueprint;
	ssize_t ret;

	logging("\n*** %s: Creating blueprint\n", __func__);

	//Segfault in woody
	blueprint = blueprint_creation(packer);
	//NO Segfault in woody
	// blueprint = blueprint_minimal();

	logging("\n*** %s: Solving injection\n", __func__);
	ret = solve_bytecodes(packer, packer->caves, blueprint, TRUE);
	if (ret != FAILURE)
	{
		((Elf64_Ehdr *)packer->content)->e_entry = ret;
	}
	logging("\n*** %s: ret= %ld\n", __func__, ret);
	return (ret);
}