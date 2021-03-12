/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbt_func_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 16:55:52 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/12 10:10:24 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	rbt_free_content(void **content)
{
	if (content && *content)
		free(*content);
}

void	rbt_keep_content(void **content)
{
	(void)content;
}

long long	t_rbt_compare_phdr(void *an, void *bn)
{
	t_pheader *a = (t_pheader *)an;
	t_pheader *b = (t_pheader *)bn;
	long long av = 0;
	long long bv = 0;

	if (a)
	{
		av = ((t_pheader *)a)->phdr->p_offset;
	}
	if (b)
	{
		bv = ((t_pheader *)b)->phdr->p_offset;
	}
	if (a && b && av == bv)
	{
		bv = ((t_pheader *)a)->phdr->p_filesz;
		av = ((t_pheader *)b)->phdr->p_filesz;
	}

	return (av - bv);
}

long long	t_rbt_compare_shdr(void *a, void *b)
{
	t_sheader *hdr_a = a;
	t_sheader *hdr_b = b;

	return (hdr_a->shdr->sh_offset - hdr_b->shdr->sh_offset);
}