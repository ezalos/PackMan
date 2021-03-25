/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_insert_next.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 23:15:40 by rkirszba          #+#    #+#             */
/*   Updated: 2021/03/25 02:08:35 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlist_insert_next(t_dlist **head, t_dlist *curs, t_dlist *new)
{
	if (!head || !curs || !new)
		return ;
	new->prev = curs;
	new->next = curs->next;
	if (curs->next)
		curs->next->prev = new;
	curs->next = new;
}

t_dlist	*ft_dlist_insert_next_wesh(t_dlist *curs, t_dlist *new)
{
	t_dlist *head;
	
	head = curs;
	while (head->prev)
		head = head->prev;
	ft_dlist_insert_next(&head, curs, new);
	return(head);
}
