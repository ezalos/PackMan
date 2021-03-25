/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_insert_prev.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 23:08:17 by rkirszba          #+#    #+#             */
/*   Updated: 2021/03/24 23:36:47 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlist_insert_prev(t_dlist **head, t_dlist *curs, t_dlist *new)
{
	if (!head || !curs | !new)
		return ;
	new->next = curs;
	new->prev = curs->prev;
	if (curs->prev)
		curs->prev->next = new;
	curs->prev = new;
	if (*head == curs)
		*head = new;
}
