/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_library.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 10:57:23 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/17 19:26:06 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	write_jump(t_btc_args *arg)
{
	arg++;
	return;
}


void	write_woody(t_btc_args *arg)
{
	arg++;
	return;
}


void	write_mem_rights(t_btc_args *arg)
{
	arg++;
	return;
}


void	write_decrypt_call(t_btc_args *arg)
{
	arg++;
	return;
}

void	write_decrypt_definition(t_btc_args *arg)
{
	arg++;
	return;
}


t_btc bytecode_lib[BYTECODE_LIB_LEN] = {
	{
		BTC_JMP,
		SIZE_JMP,
		write_jump,
		NULL,
		NULL
	},
	{
		BTC_MEM_RIGHTS,
		SIZE_MEM_RIGHTS,
		write_mem_rights,
		NULL,
		NULL
	},
	{
		BTC_DECRYPT,
		SIZE_DECRYPT,
		write_decrypt_call,
		NULL,
		NULL
	},
	{
		BTC_WRITE,
		SIZE_WRITE,
		write_woody,
		NULL,
		NULL
	},
	{
		BTC_DEF_CRYPT,
		SIZE_DEF_CRYPT,
		write_decrypt_definition,
		NULL,
		NULL
	}
};