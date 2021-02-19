/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_remove_delete_case_a.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 20:21:17 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/19 13:14:57 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "librbt.h"

void		tree_delete_case_1(t_rbt *node)
{
	if (!node->parent)
		return ;
	if (!tree_sibling(node))
		tree_delete_case_1(node->parent);
	else
		tree_delete_case_2(node);
}

void		tree_delete_case_2(t_rbt *node)
{
	if (tree_sibling(node)->color == RED)
		tree_delete_case_2a(node);
	else
		tree_delete_case_2b(node);
}

void		tree_delete_case_2a(t_rbt *node)
{
	t_rbt	*parent;
	t_rbt	*sibling;

	parent = node->parent;
	sibling = tree_sibling(node);
	parent->color = RED;
	sibling->color = BLACK;
	if (tree_which_child(sibling) == TREE_LEFT)
		tree_rot_right(parent);
	else
		tree_rot_left(parent);
	tree_delete_case_1(node);
}

void		tree_delete_case_2b(t_rbt *node)
{
	t_rbt	*parent;
	t_rbt	*sibling;

	parent = node->parent;
	sibling = tree_sibling(node);
	if ((sibling->left && sibling->left->color == RED)
	|| (sibling->right && sibling->right->color == RED))
	{
		if (sibling->left != NULL && sibling->left->color == RED)
			tree_delete_case_2bi_i(node);
		else
			tree_delete_case_2bi_ii(node);
		parent->color = BLACK;
	}
	else
		tree_delete_case_2bii(node);
}
