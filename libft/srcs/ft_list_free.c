/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 19:01:40 by rkirszba          #+#    #+#             */
/*   Updated: 2021/01/14 18:40:52 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_list_free(t_list *head, void (*f)(void*))
{
	t_list	*tmp;

	while (head)
	{
		tmp = head->next;
		f(head->data);
		free(head);
		head = tmp;
	}
}