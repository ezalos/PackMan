/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gather_find_cave_state.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 22:34:42 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/02 00:08:25 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

uint8_t		next_state(t_packer *packer, t_state *st)
{
	st->curr = get_phdr_from_array(packer, st->index++);
	st->next = get_phdr_from_array(packer, st->index);
	if (st->curr)
		return (TRUE);
	return (FALSE);
}

uint8_t		case_exit_parent(t_state *st)
{
	if (st->depth > 0 && (!st->next || !is_phdr_contained(st->parent[st->depth], st->next)))
	{
		return (TRUE);
	}
	return (FALSE);
}

uint8_t		cave_gathering(t_packer *packer)
{
	t_state		state;
	t_state		*st = &state;

	ft_memset(st, 0, sizeof(t_state));
	if (TRUE == packer->print_phdr_gather)
		print_cave_gathering_title();
	while (next_state(packer, st))
	{
		st->loop = FALSE;
		st->exit_parent = case_exit_parent(st);
		if (is_phdr_overlap(st->curr, st->next))
		{
			st->error = TRUE;
			st->state = OVERLAPPED;
		}
		else if (is_phdr_superposed(st->curr, st->next))
		{
			st->state = SUPERPOSED;
		}
		else if (is_phdr_contained(st->curr, st->next))
		{
			st->depth += 1;
			//if (st.depth >= CAVE_GATHER_MAX_PARENT)
			//	TODO
			st->parent[st->depth] = st->curr;
			st->state = CONTAINED;
		}
		else
		{
			calcul_size(packer, st);
			st->state = NORMAL;
		}
		print_state_machine(packer, st);
		st->loop = TRUE;
		while (st->exit_parent)
		{
			st->depth--;
			st->exit_parent = case_exit_parent(st);
			calcul_size(packer, st);
			print_state_machine(packer, st);
		}
	}
	if (TRUE == packer->print_phdr_gather)
		print_cave_gathering_legend();
	return (st->error);
}

