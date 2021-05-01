/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gather_find_caves_calcul_sizes.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 21:17:59 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/02 00:07:10 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

long long		phdr_space_between(Elf64_Phdr *a, Elf64_Phdr *b)
{
	long long	a_end;
	long long	b_start;
	long long	size;

	if (!b || !a)
		return 0;

	size = 0;
	b_start = b->p_offset;
	a_end = a->p_offset + a->p_memsz;
	if (b_start >= a_end)
		size = b_start - a_end;
	return size;
}

long long		phdr_space_between_ends(Elf64_Phdr *b, Elf64_Phdr *a)
{
	long long	size;

	// b is last contained & a is parent
	if (!b || !a)
		return 0;
	size = 0;
	if ((b->p_offset + b->p_memsz) >= (a->p_offset + a->p_memsz))
		size = (b->p_offset + b->p_memsz) - (a->p_offset + a->p_memsz);
	return size;
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
