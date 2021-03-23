/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_append.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 22:09:26 by rkirszba          #+#    #+#             */
/*   Updated: 2021/03/23 22:12:23 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlist_append(t_dlist **head, t_dlist *node)
{
	t_dlist *curs;

	if (!head || !node)
		return ;
	if (!(*head))
	{
		*head = node;
		return ;
	}
	curs = *head;
	while (curs->next)
		curs = curs->next;
	curs->next = node;
	node->prev = curs;
}
