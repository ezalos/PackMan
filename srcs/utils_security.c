/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_security.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 11:46:45 by rkirszba          #+#    #+#             */
/*   Updated: 2021/05/01 21:30:36 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t	is_secure_access(uint64_t file_size, uint64_t offset, uint64_t access_size)
{
	if (offset + access_size < offset || offset + access_size >= file_size)
		return (FALSE);
	return (TRUE);
}
