/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_cut.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 19:38:03 by rkirszba          #+#    #+#             */
/*   Updated: 2021/03/24 23:41:20 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_dlist *ft_dlist_cut(t_dlist **head, t_dlist *node)
{
	if (node->prev)
		node->prev->next = node->next;
	else
		(*head) = node->next;
	if (node->next)
		node->next->prev = node->prev;
	return node;
}
