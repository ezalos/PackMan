/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gather_find_caves_print.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 23:59:30 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/02 10:49:19 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void		print_cave_gathering_legend(void)
{
	printf("\n\n");
	printf("Legend: \n");
	printf("\tLOAD:        \t%sTRUE %sFALSE%s\n", _GREEN, _RED, _RESET);
	printf("\t%sn%sb%s:         \tposition of phdr in header declaration order\n", _GREEN, _RED, _RESET);
	printf("\t%sx%s->%snb%s, %sy%s: \tcave mem_size\n", _GREEN, _RESET, _BLUE, _RESET, _GREEN, _RESET);
	printf("\t%sx%s%s[%s..., %sy%s->%snb%s%s]%s: \tcave memsize between ends\n", _GREEN, _RESET, _YELLOW, _RESET, _GREEN, _RESET, _MAGENTA, _RESET, _YELLOW, _RESET);
	printf("\tnb %s/!\\%s nb:\tOverlap of phdr\n", _YELLOW, _RESET);
	printf("\tnb %s==%s nb:    \tPhdr are superposed\n", _YELLOW, _RESET);
	printf("\tnb %s[%snb%s]%s:     \tPhdr is contained\n", _YELLOW, _RESET, _YELLOW, _RESET);
	printf("\n");
}
void		print_cave_gathering_title(void)
{
	printf("\n-----------------------\n");
	printf("  PHDR CAVE GATHERING  \n");
	printf("-----------------------\n\n");
}

void		print_cave_phdr(t_packer *packer, Elf64_Phdr *a)
{
	if (!a)
		return ;
	printf("%s%d%s", (a->p_type == PT_LOAD) ? _GREEN : _RED, get_program_header_index(packer, a), _RESET);
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
			printf("->%s%s%s", _BLUE, " ..." /*∞♾∞*/, _RESET);
		else
			printf("->%s%lu%s", st->exit_parent ? _MAGENTA : _BLUE, st->size, _RESET);
		if (st->next && !st->exit_parent)
		{
			printf(", ");
			if (st->depth == 0)
				printf("\n");
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