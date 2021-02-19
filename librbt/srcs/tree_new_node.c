/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_new_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 11:23:32 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/19 13:14:30 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "librbt.h"

t_rbt		*tree_new_node(void *content, t_rbt *new_node)
{
	t_rbt		*node;

	node = new_node;
	node->content = content;
	node->color = RED;
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}
