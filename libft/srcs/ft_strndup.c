/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/19 20:45:33 by rkirszba          #+#    #+#             */
/*   Updated: 2021/01/20 17:38:25 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s1, size_t n)
{
	size_t	len;
	size_t	str_size;
	char	*dup;
	
	if (!s1)
		return (NULL);
	len = ft_strnlen(s1, n);
	str_size = (len < n ? len : n);
	if (!(dup = (char*)malloc(str_size + 1)))
		return (NULL);
	dup[str_size] = '\0';
	return (ft_strncpy(dup, s1, str_size));
}
