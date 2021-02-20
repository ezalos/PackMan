/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_btree_insert.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 19:13:22 by rkirszba          #+#    #+#             */
/*   Updated: 2021/01/26 20:07:55 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_btree_insert(t_btree **head, t_btree *node, int (*f)(void*, void*))
{
	t_btree	*curs;
	int		ret;
	
	if (!head || !node)
		return ;
	if (!(*head))
	{
		*head = node;
		return ;
	}
	curs = *head;
	while (curs)
	{
		ret = f(curs->data, node->data);
		if ((ret >= 0 && !curs->left) || (ret < 0 && !curs->right))
		{
			if (ret >= 0)
				curs->left = node;
			else
				curs->right = node;	
			break ;		
		}
		else
			curs = (ret >= 0) ? curs->left : curs->right;
	}
}
