/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_append.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 22:09:26 by rkirszba          #+#    #+#             */
/*   Updated: 2021/03/24 17:31:43 by ezalos           ###   ########.fr       */
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
	node->prev = curs;
	node->next = curs->next;
	if (curs->next)
		curs->prev = node;
	curs->next = node;
}
