/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_library.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 10:57:23 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/20 19:47:24 by ezalos           ###   ########.fr       */
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
		BTC_CALL_JMP,
		SIZE_CALL_JMP,
		write_jump,
		NULL,
		NULL
	},
	{
		BTC_CALL_MPROTECT,
		SIZE_CALL_MPROTECT,
		write_mem_rights,
		NULL,
		NULL
	},
	{
		BTC_CALL_CYPHER,
		SIZE_CALL_CYPHER,
		write_decrypt_call,
		NULL,
		NULL
	},
	{
		BTC_DEF_WRITE,
		SIZE_DEF_WRITE,
		write_woody,
		NULL,
		NULL
	},
	{
		BTC_DEF_CYPHER,
		SIZE_DEF_CYPHER,
		write_decrypt_definition,
		NULL,
		NULL
	},
	{
		BTC_DEF_INIT_PERM,
		SIZE_DEF_INIT_PERM,
		NULL,			//replace by the function
		NULL,
		NULL
	},
	{
		BTC_DEF_KEY_SCHED,
		SIZE_DEF_KEY_SCHED,
		NULL,			//replace by the function
		NULL,
		NULL
	}
};