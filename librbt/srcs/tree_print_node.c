/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_print_node.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 18:09:35 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/19 13:14:42 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "librbt.h"

void	tree_print_node_color(t_rbt *node)
{
	return ;
	if (node == NULL)
		printf("\033[33m");
	else if (node->color == RED)
		printf("\033[31m");
	else
		printf("\033[33m");
}

void	tree_print_node(t_rbt *node)
{
	if (!node)
	{
		printf("VOID NODE\n");
		return ;
	}
	tree_print_node_color(node->parent);
	printf("%*sParent: %p\n", 10 + 8, "", node->parent);
	if (node->parent)
		printf("%*s  Val:  %zu\n", 10 + 8, "", (size_t)node->parent->content);
	tree_print_node_color(node);
	printf("%*sSelf:   %p\n", 10 + 8, "", node);
	printf("%*s  Val:  %zu\n", 10 + 8, "", (size_t)node->content);
	tree_print_node_color(node->left);
	printf("Left:   %p", node->left);
	tree_print_node_color(node->right);
	printf("%*sRight:  %p\n", 10 + 8, "", node->right);
	tree_print_node_color(node->left);
	if (node->left)
		printf("  Val:  %-10zu", (size_t)node->left->content);
	else
		printf("        %10s", "");
	tree_print_node_color(node->right);
	if (node->right)
		printf("%*s  Val:  %-10zu", 10 + 10, "", (size_t)node->right->content);
	printf("\n\033[00m");
}
