/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endian.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/18 15:16:12 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/17 19:20:31 by ezalos           ###   ########.fr       */
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

uint8_t		test_endian(void)
{
    int     test_var = 1;
    uint8_t *test_endian = (uint8_t*)&test_var;

    return (test_endian[0] == 0);
}

// Untested
uint8_t		is_same_endian(t_packer *packer)
{
    return (test_endian() == ENDIAN(packer));
}