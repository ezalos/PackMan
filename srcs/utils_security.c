/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   security.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 11:46:45 by rkirszba          #+#    #+#             */
/*   Updated: 2021/02/19 12:24:39 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t	is_secure_access(uint64_t mem_size, uint64_t offset, uint64_t access_size)
{
	if (offset + access_size < offset || offset + access_size >= mem_size)
		return (FALSE);
	return (TRUE);
}
