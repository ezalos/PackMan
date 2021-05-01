/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gather_find_cave_state.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 22:34:42 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/01 23:56:44 by ezalos           ###   ########.fr       */
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

void	print_state_machine(t_packer *packer, t_state *st)
{
	if (FALSE == packer->print_phdr_gather)
		return ;
	if (st->loop == TRUE || st->state == NORMAL)
	{
		if (st->loop == TRUE)
			printf("%s]%s", _YELLOW, _RESET);
		else
			print_cave_phdr(packer, st->curr);
		if (st->depth == 0 && NULL == st->next)
			printf("->%s%s%s", _BLUE, "..." /*∞♾∞*/, _RESET);
		else
			printf("->%s%lu%s", st->exit_parent ? _MAGENTA : _BLUE, st->size, _RESET);
		if (st->next && !st->exit_parent)
		{
			printf(", ");
			// if (st->depth == 0)
			// 	printf("\n");
		}
	}
	else if (st->state == OVERLAPPED)
	{
		printf("(");
		print_cave_phdr(packer, st->curr);
		printf("%s/!\\%s ", _YELLOW, _RESET);
		print_cave_phdr(packer, st->next);
		printf(")");
	}
	else if (st->state == SUPERPOSED)
	{
		print_cave_phdr(packer, st->curr);
		printf(" %s==%s ", _YELLOW, _RESET);
	}
	else if (st->state == CONTAINED)
	{
		print_cave_phdr(packer, st->curr);
		printf("%s[%s", _YELLOW, _RESET);
	}
}

void	calcul_size(t_packer *packer, t_state *st)
{
	Elf64_Phdr	*ph;
	// Cases between A and B:
	//		Normal:		A[ ] -> x,	B[ ]
	//		Before:		A[			B[ ] -> X	]
	//		After:		A[ C[ ]  ] ->	B[ ]

	if (st->exit_parent == FALSE)
	// This size is printed in blue
	{
		if (st->loop)
			st->depth++;
		if (st->depth == 0 
		|| (is_phdr_contained(st->parent[st->depth], st->curr)
		&& is_phdr_contained(st->parent[st->depth], st->next)))
		// NOMRAL
		{
			ph = st->curr;
		}
		else
		// AFTER
		{
			ph = st->parent[st->depth];
		}
		if (st->loop)
			st->depth--;
		st->size = phdr_space_between(ph, st->next);
		find_t_pheader_from_phdr(packer, ph)->available_size = st->size;
	}
	else if (st->exit_parent == TRUE)
	// This size is printed in magenta
	// BEFORE
	{
		st->size = phdr_space_between_ends(st->parent[st->depth], st->curr);
		find_t_pheader_from_phdr(packer, st->parent[st->depth])->available_size = st->size;
	}
}

void	not_a_state_machine(t_packer *packer)
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
}

