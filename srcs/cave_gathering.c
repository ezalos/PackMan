/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cave_gathering.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 21:17:59 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/18 10:51:05 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

//	For segment in offset order
// 		a = after_end(segment)
//		while offset start next_segment < a:
//			if a is in [next start ; next end]:
//				a = after_end(next_segment)
//			if a < next_start:
//				break;
//		segment.cave_size = next_start - a; 

uint8_t		is_phdr_contained(Elf64_Phdr *a, Elf64_Phdr *b)
{
	size_t a_start;
	size_t a_end;
	size_t b_start;
	size_t b_end;

	if (!b || !a)
		return FALSE;
		
	a_start = a->p_offset;
	b_start = b->p_offset;
	a_end = a->p_offset + a->p_filesz;
	b_end = b->p_offset + b->p_filesz;

	if (a_start <= b_start && a_end >= b_end)
		return TRUE;
		
	return FALSE;
}

uint8_t		is_phdr_superposed(Elf64_Phdr *a, Elf64_Phdr *b)
{
	size_t a_start;
	size_t a_end;
	size_t b_start;
	size_t b_end;

	if (!b || !a)
		return FALSE;

	a_start = a->p_offset;
	b_start = b->p_offset;
	a_end = a->p_offset + a->p_filesz;
	b_end = b->p_offset + b->p_filesz;

	if (a_start == b_start && a_end == b_end)
		return TRUE;

	return FALSE;
}

uint8_t		is_phdr_overlap(Elf64_Phdr *a, Elf64_Phdr *b)
{
	size_t a_start;
	size_t a_end;
	size_t b_start;
	size_t b_end;

	if (!b || !a)
		return FALSE;

	a_start = a->p_offset;
	b_start = b->p_offset;
	a_end = a->p_offset + a->p_filesz;
	b_end = b->p_offset + b->p_filesz;

	if (a_start <= b_start && b_start < a_end && b_end > a_end)
		return TRUE;

	return FALSE;
}

long long	phdr_space_between(Elf64_Phdr *a, Elf64_Phdr *b)
{
	long long a_end;
	long long b_start;

	if (!b || !a)
		return 0;

	b_start = b->p_offset;
	a_end = a->p_offset + a->p_filesz;

	return b_start - a_end;
}

long long	phdr_space_between_ends(Elf64_Phdr *a, Elf64_Phdr *b)
{
	if (!b || !a)
		return 0;

	return ((b->p_offset + b->p_filesz) - (a->p_offset + a->p_filesz));
}

void		print_cave_phdr(t_packer *packer, Elf64_Phdr *a)
{
	if (!a)
		return ;
	printf("%s%d%s", (a->p_type & PT_LOAD) ? _GREEN : _RED, get_program_header_index(packer, a), _RESET);
}

t_pheader	*find_t_pheader_from_phdr(t_packer *packer, Elf64_Phdr *a)
{
	t_pheader	*the_choosen_one;
	int i;

	the_choosen_one = NULL;
	if (packer->phdr_array)
	{
		i = -1;
		while (packer->phdr_array[++i])
			if (packer->phdr_array[i]->phdr == a)
				the_choosen_one = packer->phdr_array[i];
	}
	return (the_choosen_one);
}

void		print_cave_size(Elf64_Phdr *a, Elf64_Phdr *b, t_packer *packer)
{

	long long	size;
	char		*color;
	Elf64_Phdr	*quest;

	(void)quest;
	if (!a || !b)
		return;
	if (is_phdr_contained(b, a))
	{
		color = _MAGENTA;
		size = phdr_space_between_ends(a, b);
		// if (a->p_offset == b->p_offset)
		// {
		// 	if (a->p_filesz > b->p_filesz)
		// 		quest = b;
		// 	else
		// 		quest = a;
		// }
		// else
		// {
		// 	if (a->p_offset > b->p_offset)
		// 		quest = a;
		// 	else
		// 		quest = b;
		// }
		find_t_pheader_from_phdr(packer,  a)->available_size = -size;
	}
	else
	{
		color = _BLUE;
		size = phdr_space_between(a, b);
		find_t_pheader_from_phdr(packer, a)->available_size = size;
	}
	printf("->%s%lld%s", color, size, _RESET);
}

Elf64_Phdr	*get_phdr_from_array(t_packer *packer, int i)
{
	if (packer->phdr_array[i])
		return packer->phdr_array[i]->phdr;
	return NULL;
}

int		print_phdr_contain(t_packer *packer, int i)
{
	Elf64_Phdr *parent;
	Elf64_Phdr *curr;
	Elf64_Phdr *next;

	parent = get_phdr_from_array(packer, i);
	curr = get_phdr_from_array(packer, ++i);
	if (!curr)
		return i - 1;
	printf("%s[%s", _YELLOW, _RESET);
	next = get_phdr_from_array(packer, i + 1);
	while (curr && is_phdr_contained(parent, curr))
	{
		print_cave_phdr(packer, curr);
		if (is_phdr_superposed(curr, next))
			printf(" %s==%s ",_YELLOW, _RESET);
		else if (is_phdr_contained(curr, next))
		{
			i = print_phdr_contain(packer, i);
			if (get_phdr_from_array(packer, i + 1)
			&& curr != get_phdr_from_array(packer, i + 1))
			{
				// print_cave_size(curr, get_phdr_from_array(packer, i + 1));
				printf(", ");
			}
		}
		else if (next && is_phdr_contained(parent, next))
		{
			print_cave_size(curr, get_phdr_from_array(packer, i + 1), packer);
			printf(", ");
		}
		curr = get_phdr_from_array(packer, ++i);
		next = get_phdr_from_array(packer, i + 1);
	}
	print_cave_size(get_phdr_from_array(packer, i - 1), parent, packer); //, get_phdr_from_array(packer, i + 1));
	printf("%s]%s", _YELLOW, _RESET);
	return i - 1;
}

void		print_cave_gathering_legend(void)
{
	printf("Legend: \n");
	printf("\tLOAD:        \t%sTRUE %sFALSE%s\n", _GREEN, _RED, _RESET);
	printf("\t%sn%sb%s:         \tposition of phdr in header declaration order\n", _GREEN, _RED, _RESET);
	printf("\t%sx%s->%snb%s, %sy%s: \tcave size\n", _GREEN, _RESET, _BLUE, _RESET, _GREEN, _RESET);
	printf("\t%sx%s%s[%s..., %sy%s->%snb%s%s]%s: \tcave size between ends\n", _GREEN, _RESET, _YELLOW, _RESET, _GREEN, _RESET, _MAGENTA, _RESET, _YELLOW, _RESET);
	printf("\tnb %s/!\\%s nb:\tOverlap of phdr\n", _YELLOW, _RESET);
	printf("\tnb %s==%s nb:    \tPhdr are superposed\n", _YELLOW, _RESET);
	printf("\tnb %s[%snb%s]%s:     \tPhdr is contained\n", _YELLOW, _RESET, _YELLOW, _RESET);
}

void		cave_gathering_phdr(t_packer *packer)
{
	Elf64_Phdr	*curr;
	Elf64_Phdr	*next;
	int			i;

	printf("\n-----------------------\n");
	printf("  PHDR CAVE GATHERING  \n");
	printf("-----------------------\n\n");
	i = -1;
	while ((curr = get_phdr_from_array(packer, ++i)))
	{
		if ((next = get_phdr_from_array(packer, i + 1)))
		{
			if (is_phdr_overlap(curr, next))
			{
				printf("(");
				print_cave_phdr(packer, curr);
				printf("%s/!\\%s ", _YELLOW, _RESET);
				print_cave_phdr(packer, next);
				printf(")");
			}
			if (is_phdr_superposed(curr, next))
			{
				print_cave_phdr(packer, curr);
				printf(" %s==%s ", _YELLOW, _RESET);
			}
			else if (is_phdr_contained(curr, next))
			{
				print_cave_phdr(packer, curr);
				i = print_phdr_contain(packer, i);
				if (get_phdr_from_array(packer, i + 1)
				&& curr != get_phdr_from_array(packer, i + 1))
				{
					print_cave_size(curr, get_phdr_from_array(packer, i + 1), packer);
					printf(", ");
				}
			}
			else
			{
				print_cave_phdr(packer, curr);
				print_cave_size(curr, get_phdr_from_array(packer, i + 1), packer);
				printf(", ");
			}
		}
	}
	printf("\n\n");
	print_cave_gathering_legend();
	printf("\n");
}

void	cave_gathering(t_packer *packer)
{
	// Assumption packer->phdr_array is sorted by ascending offset, and then by descending filesize.
	cave_gathering_phdr(packer);
}
