/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_btree_new.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 19:09:33 by rkirszba          #+#    #+#             */
/*   Updated: 2021/01/12 19:53:22 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_btree	*ft_btree_new(void *data)
{
	t_btree	*new;
	
	if (!(new = (t_btree *)malloc(sizeof(*new))))
		return (NULL);
	new->data = data;
	new->left = NULL;
	new->right = NULL;
	return (new);
}
