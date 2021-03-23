/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_prepend.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 22:18:00 by rkirszba          #+#    #+#             */
/*   Updated: 2021/03/23 22:19:52 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlist_prepend(t_dlist **head, t_dlist *node)
{
	if (!head || !node)
		return ;
	if (!(*head))
	{
		*head = node;
		return ;
	}
	node->next = *head;
	(*head)->prev = node;
	*head = node;
}
