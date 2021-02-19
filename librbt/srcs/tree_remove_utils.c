/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_remove_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 14:11:36 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/19 13:15:07 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "librbt.h"

int8_t		tree_which_child(t_rbt *node)
{
	if (!node->parent)
		return (TREE_NONE);
	else if (node == node->parent->left)
		return (TREE_LEFT);
	else if (node == node->parent->right)
		return (TREE_RIGHT);
	else
		return (TREE_ERROR);
}

void		tree_cut_leaf(t_rbt *node)
{
	if (node->parent)
	{
		if (node == node->parent->left)
			node->parent->left = NULL;
		else
			node->parent->right = NULL;
	}
}

void		tree_replace_node(t_rbt *node, t_rbt *child)
{
	if (child)
		child->parent = node->parent;
	if (node->parent)
	{
		if (node == node->parent->left)
			node->parent->left = child;
		else
			node->parent->right = child;
	}
}

void		tree_copy_values(t_rbt *dest, t_rbt *src)
{
	dest->color = src->color;
	dest->parent = src->parent;
	dest->left = src->left;
	dest->right = src->right;
	if (dest->parent)
	{
		if (src == dest->parent->left)
			dest->parent->left = dest;
		else
			dest->parent->right = dest;
	}
	if (dest->left)
		dest->left->parent = dest;
	if (dest->right)
		dest->right->parent = dest;
}

void		tree_permute_nodes(t_rbt *node1, t_rbt *node2)
{
	t_rbt	tmp1;
	t_rbt	tmp2;

	if (!node1 || !node2)
		return ;
	tree_copy_values(&tmp1, node1);
	tree_copy_values(&tmp2, node2);
	tree_copy_values(node1, &tmp2);
	tree_copy_values(node2, &tmp1);
}
