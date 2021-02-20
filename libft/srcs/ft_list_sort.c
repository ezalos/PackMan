/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/20 18:16:48 by rkirszba          #+#    #+#             */
/*   Updated: 2021/02/20 18:28:26 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_list_sort(t_list *head, int (*f)(void*, void*))
{
	void	*tmp;
	t_list	*curs;
	t_list	*next;

	if (!head)
		return ;
	curs = head;
	next = curs->next;
	while (next)
	{
		if (f(curs, next) > 0)
		{
			tmp = curs->data;
			curs->data = next->data;
			next->data = tmp;
			curs = head;
			next = curs->next;
			continue ;
		}
		curs = next;
		next = curs->next;
	}
}