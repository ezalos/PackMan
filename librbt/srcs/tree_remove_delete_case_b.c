/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_remove_delete_case_b.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 20:21:23 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/19 13:15:02 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "librbt.h"

void		tree_delete_case_2bi_i(t_rbt *node)
{
	t_rbt	*parent;
	t_rbt	*sibling;

	parent = node->parent;
	sibling = tree_sibling(node);
	if (tree_which_child(sibling) == TREE_LEFT)
	{
		sibling->left->color = sibling->color;
		sibling->color = parent->color;
		tree_rot_right(parent);
	}
	else
	{
		sibling->left->color = parent->color;
		tree_rot_right(sibling);
		tree_rot_left(parent);
	}
}

void		tree_delete_case_2bi_ii(t_rbt *node)
{
	t_rbt	*parent;
	t_rbt	*sibling;

	parent = node->parent;
	sibling = tree_sibling(node);
	if (tree_which_child(sibling) == TREE_LEFT)
	{
		sibling->right->color = parent->color;
		tree_rot_left(sibling);
		tree_rot_right(parent);
	}
	else
	{
		sibling->right->color = sibling->color;
		sibling->color = parent->color;
		tree_rot_left(parent);
	}
}

void		tree_delete_case_2bii(t_rbt *node)
{
	t_rbt	*parent;
	t_rbt	*sibling;

	parent = node->parent;
	sibling = tree_sibling(node);
	sibling->color = RED;
	if (parent->color == BLACK)
		tree_delete_case_1(parent);
	else
		parent->color = BLACK;
}
