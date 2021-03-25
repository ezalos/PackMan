/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_insert_next.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 23:15:40 by rkirszba          #+#    #+#             */
/*   Updated: 2021/03/24 23:36:31 by rkirszba         ###   ########.fr       */
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
