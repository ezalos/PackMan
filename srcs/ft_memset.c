/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 16:56:51 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/07 16:56:53 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void *ft_memset(void *str, int c, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
		((char *)str)[i++] = (char)c;
	return (str);
}
