/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_prepend.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 22:18:00 by rkirszba          #+#    #+#             */
/*   Updated: 2021/03/24 17:57:12 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlist_prepend(t_dlist **head, t_dlist *node)
{
	t_dlist	*prev;
	t_dlist	*next;

	if (!head || !node)
		return ;
	if (!(*head))
	{
		*head = node;
		return ;
	}
	prev = (*head)->prev;
	next = (*head);
	node->prev = prev;
	if (prev)
		node->prev->next = node;
	node->next = next;
	if (next)
		node->next->prev = node;
	*head = node;
}
