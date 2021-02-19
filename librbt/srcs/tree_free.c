/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 20:21:55 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/19 19:59:19 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "librbt.h"

void				tree_free(t_rbt *root, t_rbt_free_content *func)
{
	if (root != NULL)
	{
		tree_free(root->left, func);
		func(&root->content);
		tree_free(root->right, func);
		free(root);
	}
}
