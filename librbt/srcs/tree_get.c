/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_get.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 20:21:09 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/10 23:02:38 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "librbt.h"

t_rbt	*tree_get_recurse_func(t_rbt *root, void *content, t_rbt_compare *func)
{
	if (root != NULL)
	{
		if (func(root->content, content) > 0)
			return (tree_get_recurse_func(root->left, content, func));
		else if (func(root->content, content) < 0)
			return (tree_get_recurse_func(root->right, content, func));
		else
			return (root);
	}
	return (NULL);
}

t_rbt	*tree_get_recurse_func_ll(t_rbt *root, void *content,
		t_rbt_compare_long_long *func)
{
	if (root != NULL)
	{
		if (func(root->content, content) > 0)
			return (tree_get_recurse_func_ll(root->left, content, func));
		else if (func(root->content, content) < 0)
			return (tree_get_recurse_func_ll(root->right, content, func));
		else
			return (root);
	}
	return (NULL);
}

t_rbt	*tree_get_node_th(t_rbt *root, int *umpteenth)
{
	t_rbt			*node;

	node = NULL;
	if (NULL != root)
	{
		if ((node = tree_get_node_th(root->left, umpteenth)))
			return (node);
		if (!(*umpteenth)--)
			return (root);
		if ((node = tree_get_node_th(root->right, umpteenth)))
			return (node);
	}
	return (NULL);
}

t_rbt	*tree_get_in_order_pred(t_rbt *node)
{
	t_rbt *in_order_pred;

	if (!node)
		return (NULL);
	in_order_pred = node->left;
	while (in_order_pred && in_order_pred->right)
	{
		in_order_pred = in_order_pred->right;
	}
	return (in_order_pred);
}
