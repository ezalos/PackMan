/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 23:16:34 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/25 23:16:54 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t		save_woody(t_packer *packer)
{
	int fd;

	if (-1 == (fd = open(packer->out, O_WRONLY | O_CREAT, 0777)))
		return (FAILURE);
	if (write(fd, packer->content, packer->size) != (ssize_t)packer->size)
		return (FAILURE);
	return (SUCCESS);
}