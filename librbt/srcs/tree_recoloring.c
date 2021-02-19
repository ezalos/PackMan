/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_recoloring.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 20:44:09 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/19 13:14:48 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "librbt.h"

void		tree_recoloring(t_rbt *node)
{
	if (node->parent == NULL)
		node->color = BLACK;
	else if (node->parent->color != BLACK)
	{
		if (tree_uncle(node)->color == RED)
		{
			node->parent->color = BLACK;
			tree_uncle(node)->color = BLACK;
		}
	}
}
