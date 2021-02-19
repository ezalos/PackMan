/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_len.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 11:19:07 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/19 13:14:23 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "librbt.h"

static int		count(t_rbt *rbt)
{
	(void)rbt;
	return (1);
}

int				tree_len(t_rbt *tree)
{
	return (tree_inorder(tree, count));
}
