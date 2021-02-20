/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_to_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/20 19:21:38 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/20 19:36:51 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "librbt.h"

// Should not be using tree_get_node_th,
// but current project does need this optimization
void	**tree_to_array(t_rbt *root)
{
	void	**array;
	t_rbt	*node;
	int		i;
	int		size;
	int		tmp;

	if (!root)
		return (NULL);
	size = tree_len(root);
	if (!(array = malloc(sizeof(void*) * (size + 1))))
		return (NULL);
	i = 0;
	tmp = i;
	while ((node = tree_get_node_th(root, &tmp)))
	{
		array[i] = node->content;
		tmp = ++i;
	}
	array[i] = NULL;
	return (array);
}