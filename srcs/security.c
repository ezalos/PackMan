/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   security.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 11:46:45 by rkirszba          #+#    #+#             */
/*   Updated: 2021/02/16 11:58:31 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t	is_secure_access(uint64_t size, uint64_t offset, uint64_t to_read)
{
	if (offset + to_read < offset || offset + to_read >= size)
		return (FALSE);
	return (TRUE);
}
