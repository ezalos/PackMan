/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_inrorder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 21:01:07 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/19 13:16:17 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "librbt.h"

int					tree_inrorder(t_rbt *root, t_rbt_inorder *func)
{
	int				value;

	value = 0;
	if (root != NULL)
	{
		value += tree_inrorder(root->right, func);
		value += func(root);
		value += tree_inrorder(root->left, func);
	}
	return (value);
}

void				*tree_inrorder_ptr(t_rbt *root, t_rbt_inorder_ptr *func)
{
	long long		value;

	value = 0;
	if (root != NULL)
	{
		value += (long long)tree_inrorder_ptr(root->right, func);
		value += (long long)func(root);
		value += (long long)tree_inrorder_ptr(root->left, func);
	}
	return ((void*)value);
}
