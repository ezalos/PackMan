/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gather_find_caves_phdr_relations.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 21:17:59 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/02 00:04:58 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

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

