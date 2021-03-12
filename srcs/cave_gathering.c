/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cave_gathering.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 21:17:59 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/12 12:09:38 by ezalos           ###   ########.fr       */
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

void		print_cave_size(Elf64_Phdr *a, Elf64_Phdr *b)
{
	if (!a || !b)
		return;
	if (is_phdr_contained(b, a))
		printf("->%s%lld%s", _MAGENTA, phdr_space_between_ends(a, b), _RESET);
	else
		printf("->%s%lld%s", _BLUE, phdr_space_between(a, b), _RESET);
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
	printf("[");
	next = get_phdr_from_array(packer, i + 1);
	while (curr && is_phdr_contained(parent, curr))
	{
		print_cave_phdr(packer, curr);
		if (is_phdr_superposed(curr, next))
			printf("== ");
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
			print_cave_size(curr, get_phdr_from_array(packer, i + 1));
			printf(", ");
		}
		curr = get_phdr_from_array(packer, ++i);
		next = get_phdr_from_array(packer, i + 1);
	}
	print_cave_size(get_phdr_from_array(packer, i - 1) , parent); //, get_phdr_from_array(packer, i + 1));
	printf("]");
	return i - 1;
}

void		print_cave_gathering_legend(void)
{
	printf("Legend: \n");
	printf("\tLOAD:        \t%sTRUE %sFALSE%s\n", _GREEN, _RED, _RESET);
	printf("\t%sn%sb%s:         \tposition of phdr in header declaration order\n", _GREEN, _RED, _RESET);
	printf("\t->%snb%s:       \tcave size\n", _BLUE, _RESET);
	printf("\t->%snb%s:       \tcave size between ends\n", _MAGENTA, _RESET);
	printf("\t(nb /!\\ nb):\tOverlap of phdr\n");
	printf("\tnb == nb:    \tPhdr are superposed\n");
	printf("\tnb [nb]:     \tPhdr is contained\n");
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
				printf("/!\\ ");
				print_cave_phdr(packer, next);
				printf(")");
			}
			if (is_phdr_superposed(curr, next))
			{
				print_cave_phdr(packer, curr);
				printf("== ");
			}
			else if (is_phdr_contained(curr, next))
			{
				print_cave_phdr(packer, curr);
				i = print_phdr_contain(packer, i);
				if (get_phdr_from_array(packer, i + 1)
				&& curr != get_phdr_from_array(packer, i + 1))
				{
					print_cave_size(curr, get_phdr_from_array(packer, i + 1));
					printf(", ");
				}
			}
			else
			{
				print_cave_phdr(packer, curr);
				print_cave_size(curr, get_phdr_from_array(packer, i + 1));
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
	cave_gathering_phdr(packer);
}

// void cave_gathering(t_packer *packer)
// {
// 	int i_p;
// 	int i_s;
// 	int curr_end;
// 	int next_start;

// 	i_p = -1;
// 	while (packer->phdr_array[++i_p])
// 	{
// 		if (packer->phdr_array[i_p + 1])
// 		{
// 			curr_end = packer->phdr_array[i_p]->phdr->p_offset + packer->phdr_array[i_p]->phdr->p_filesz;
// 			next_start = packer->phdr_array[i_p + 1]->phdr->p_offset;
// 			packer->phdr_array[i_p]->available_size = next_start - curr_end;
// 		}
// 		else
// 			packer->phdr_array[i_p]->available_size = 0;
// 		// printf("Phdr %d: %d\n", i_p, packer->phdr_array[i_p]->available_size);
// 		// printf("\tShdr %p\n", packer->phdr_array[i_p]->shdr_array);
// 		if (NULL == packer->phdr_array[i_p]->shdr_array)
// 			continue;
// 		i_s = -1;
// 		while (packer->phdr_array[i_p]->shdr_array[++i_s])
// 		{
// 			if (packer->phdr_array[i_p]->shdr_array[i_s + 1])
// 			{
// 				curr_end = packer->phdr_array[i_p]->shdr_array[i_s]->shdr->sh_offset + packer->phdr_array[i_p]->shdr_array[i_s]->shdr->sh_size;
// 				next_start = packer->phdr_array[i_p]->shdr_array[i_s + 1]->shdr->sh_offset;
// 				packer->phdr_array[i_p]->shdr_array[i_s]->available_size = next_start - curr_end;
// 			}
// 			else if (packer->phdr_array[i_p + 1])
// 				packer->phdr_array[i_p]->shdr_array[i_s]->available_size = packer->phdr_array[i_p]->available_size;
// 			else
// 				packer->phdr_array[i_p]->shdr_array[i_s]->available_size = 0;
// 			// printf("\tShdr %d: %d\n", i_s, packer->phdr_array[i_p]->shdr_array[i_s]->available_size);
// 		}
// 	}
// }