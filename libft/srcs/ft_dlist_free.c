/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 22:15:07 by rkirszba          #+#    #+#             */
/*   Updated: 2021/03/23 22:17:20 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlist_free(t_dlist *head, void (*f)(void*))
{
	t_dlist	*tmp;

	while (head)
	{
		tmp = head->next;
		f(head->data);
		free(head);
		head = tmp;
	}
}
