/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_append.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 18:49:02 by rkirszba          #+#    #+#             */
/*   Updated: 2021/01/12 19:00:33 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_list_append(t_list **head, t_list *node)
{
	t_list	*curs;

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
}
