/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gather_find_cave_state.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 22:34:42 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/01 17:55:05 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

//TODO: remove print, return size
void print_cave_size_dos(Elf64_Phdr *a, Elf64_Phdr *b, t_packer *packer)
{

	long long size;
	char *color;

	if (!a || !b)
		return;
	if (is_phdr_contained(b, a))
	{
		// a is last contained & b is parent
		color = _MAGENTA;
		size = phdr_space_between_ends(a, b);
		find_t_pheader_from_phdr(packer, a)->available_size = -size;
	}
	else
	{
		color = _BLUE;
		size = phdr_space_between(a, b);
		find_t_pheader_from_phdr(packer, a)->available_size = size;
	}
	printf("->%s%lld%s", color, size, _RESET);
}

uint8_t	next_state(t_packer *packer, t_state *st)
{
	st->curr = get_phdr_from_array(packer, st->index++);
	st->next = get_phdr_from_array(packer, st->index);
	if (st->curr)
		return (TRUE);
	st->end = TRUE;
	return (FALSE);
}

uint8_t		case_next_overlap(t_packer *packer, t_state *st)
{
	// Case : (CURR /!\ NEXT)
	// TODO: remove depth check
	if (st->depth == 0 && is_phdr_overlap(st->curr, st->next))
	{
		st->error = TRUE;

		// Print
		printf("(");
		print_cave_phdr(packer, st->curr);
		printf("%s/!\\%s ", _YELLOW, _RESET);
		print_cave_phdr(packer, st->next);
		printf(")");
		return (TRUE);
	}
	return (FALSE);
}

uint8_t		case_next_supperposed(t_packer *packer, t_state *st)
{
	if (is_phdr_superposed(st->curr, st->next))
	{
		// Print
		print_cave_phdr(packer, st->curr);
		printf(" %s==%s ", _YELLOW, _RESET);
		return (TRUE);
	}
	return (FALSE);
}

uint8_t		case_next_contained(t_packer *packer, t_state *st)
{
	if (is_phdr_contained(st->curr, st->next))
	{
		st->depth += 1;
		// TODO
		// if (st.depth >= CAVE_GATHER_MAX_PARENT)
		// 	pbm
		st->parent[st->depth] = st->curr;

		// Print
		print_cave_phdr(packer, st->curr);
		printf("%s[%s", _YELLOW, _RESET);
		return (TRUE);
	}
	return (FALSE);
}

uint8_t		case_next_is_not_contained_anymore(t_packer *packer, t_state *st)
{
	if (st->depth > 0 && (!st->next || !is_phdr_contained(st->parent[st->depth], st->next)))
	{
		// Print
		printf("%s]%s", _YELLOW, _RESET);
		print_cave_size_dos(st->parent[st->depth], st->next, packer);
		if (st->next)
			printf(", ");

		st->depth--;
		return (TRUE);
	}
	return (FALSE);
}


void	not_a_state_machine(t_packer *packer)
{
	t_state		st;

	ft_memset(&st, 0, sizeof(t_state));
	printf("\n-----------------------\n");
	printf("  PHDR CAVE GATHERING  \n");
	printf("-----------------------\n\n");
	next_state(packer, &st);
	while (st.curr)
	{
		if (case_next_overlap(packer, &st))
		{
			;
		}
		else if (case_next_supperposed(packer, &st))
		{
			;
		}
		else if (case_next_contained(packer, &st))
		{
			;
		}
		else
		{
			print_cave_phdr(packer, st.curr);
			print_cave_size_dos(st.curr, st.next, packer);
			if (st.next)
				printf(", ");
		}
		while (case_next_is_not_contained_anymore(packer, &st))
		{
			;
		}

		next_state(packer, &st);
	}
	printf("\n\n");
	print_cave_gathering_legend();
	printf("\n");
}

