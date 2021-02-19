/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_insert_repair.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 20:55:57 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/19 13:14:16 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "librbt.h"

void		tree_insert_repair(t_rbt *n)
{
	if (tree_parent(n) == NULL)
	{
		tree_insert_case_1(n);
	}
	else if (tree_parent(n)->color == BLACK)
	{
		tree_insert_case_2(n);
	}
	else if (tree_uncle(n) != NULL && tree_uncle(n)->color == RED)
	{
		tree_insert_case_3(n);
	}
	else
	{
		tree_insert_case_4(n);
	}
}
