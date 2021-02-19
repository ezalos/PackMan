/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_remove.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 14:11:36 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/19 13:14:52 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "librbt.h"

/*
**	This function return new root
**	.
**	Case if (node->color == RED)
**		normalement, il est impossible qu'un rouge ait un
**		seul enfant (pas d'enfant rouge + 0 ou 2 enfants noirs)
**	Case if (child)
**		case the node has one child, necessarily RED
**	Case else
**		case node is BLACK and has no child
*/

static t_rbt		*tree_delete_node_next(t_rbt *node)
{
	t_rbt	*child;

	if (node->right == NULL)
		child = node->left;
	else
		child = node->right;
	if (child)
	{
		child->color = BLACK;
		tree_replace_node(node, child);
		return (tree_root(child));
	}
	else
	{
		tree_delete_case_1(node);
		tree_cut_leaf(node);
		return (tree_root(node->parent));
	}
}

t_rbt				*tree_delete_node(t_rbt *node)
{
	t_rbt	*in_order_pred;

	if (node->right && node->left)
	{
		in_order_pred = tree_get_in_order_pred(node);
		tree_permute_nodes(node, in_order_pred);
		return (tree_delete_node(node));
	}
	if (node->color == RED)
	{
		tree_cut_leaf(node);
		return (tree_root(node->parent));
	}
	return (tree_delete_node_next(node));
}
