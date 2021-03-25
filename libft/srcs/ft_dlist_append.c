/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_append.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 22:09:26 by rkirszba          #+#    #+#             */
/*   Updated: 2021/03/24 23:22:22 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlist_append(t_dlist **head, t_dlist *node)
{
	t_dlist *curs;
	t_dlist *prev;
	t_dlist *next;

	if (!head || !node)
		return ;
	if (!(*head))
	{
		*head = node;
		return ;
	}
	curs = *head;
	prev = curs->prev;
	if (prev)
		prev->next = curs;
	next = curs->next;
	if (next)
		next->prev = curs;
	curs->next = next;
	curs->prev = prev;
}
