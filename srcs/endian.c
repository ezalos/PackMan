/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endian.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/18 15:16:12 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/17 11:57:30 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	change_endian(void *data, int size)
{
	int tmp;
	int i;
	int n;

	i = 0;
	n = size - 1;
	while (i < n)
	{
		tmp = ((char *)data)[i];
		((char *)data)[i] = ((char *)data)[n];
		((char *)data)[n] = tmp;
		i++;
		n--;
	}
}