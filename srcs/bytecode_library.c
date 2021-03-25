/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_library.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 10:57:23 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/25 00:55:15 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

t_btc bytecode_lib[BYTECODE_LIB_LEN] = {
	{
		BTC_DEF_BEGIN,
		SIZE_DEF_BEGIN,
		inject_def_begin,
		NULL
	},
	{
		BTC_CALL_MPROTECT,
		SIZE_CALL_MPROTECT,
		inject_call_mprotect,
		NULL
	},
	{
		BTC_DEF_CYPHER_PREPARE,
		SIZE_DEF_CYPHER_PREPARE,
		inject_def_cypher_prepare,
		NULL,
	},
	{
		BTC_CALL_CYPHER,
		SIZE_CALL_CYPHER,
		inject_call_cypher,
		NULL,
	},
	{
		BTC_DEF_WRITE,
		SIZE_DEF_WRITE,
		inject_def_write,
		NULL,
	},
	{
		BTC_DEF_END,
		SIZE_DEF_END,
		inject_def_end,
		NULL
	},
	{
		BTC_CALL_JMP,
		SIZE_CALL_JMP,
		inject_call_jmp,
		NULL,
	},
	{
		BTC_DEF_INIT_PERM,
		SIZE_DEF_INIT_PERM,
		inject_def_init_perm,			//replace by the function
		NULL,
	},
	{
		BTC_DEF_KEY_SCHED,
		SIZE_DEF_KEY_SCHED,
		inject_def_key_sched,			//replace by the function
		NULL,
	},
	{
		BTC_DEF_CYPHER,
		SIZE_DEF_CYPHER,
		inject_def_cypher,
		NULL,
	}
};