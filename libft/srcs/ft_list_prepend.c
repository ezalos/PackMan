/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_prepend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 18:55:46 by rkirszba          #+#    #+#             */
/*   Updated: 2021/01/12 19:01:02 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_list_prepend(t_list **head, t_list *node)
{
	if (!head || !node)
		return ;
	if (!(*head))
	{
		*head = node;
		return ;
	}
	node->next = *head;
	*head = node;
}
