/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 23:16:34 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/07 11:52:02 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t		save_woody(t_packer *packer)
{
	int fd;

	logging("* OUT\n");
	if (-1 == (fd = open(packer->out, O_WRONLY | O_CREAT, 0777)))
	{
		print_error(packer->self_path, strerror(errno));
		return (FAILURE);
	}
	if (write(fd, packer->content, packer->size) != (ssize_t)packer->size)
	{
		print_error(packer->self_path, strerror(errno));
		return (FAILURE);
	}
	printf("key_value: %zx\n", *(uint64_t*)(&(packer->key)));
	return (SUCCESS);
}