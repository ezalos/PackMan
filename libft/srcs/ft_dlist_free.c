/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 22:15:07 by rkirszba          #+#    #+#             */
/*   Updated: 2021/03/24 17:08:39 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlist_free(t_dlist *head, void (*f)(void*))
{
	t_dlist	*tmp;

	while (head)
	{
		tmp = head->next;
		if (f)
			f(head->data);
		free(head);
		head = tmp;
	}
}
