/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 11:31:22 by rkirszba          #+#    #+#             */
/*   Updated: 2021/05/03 19:40:16 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t	print_error(char *self_path, const char *fmt , ...)
{
	va_list	args;

	dprintf(2, "%s: error: ", self_path);
	va_start(args, fmt);
    vdprintf(2, fmt, args);
    va_end(args);
	dprintf(2, "\n");
	return (FAILURE);
}

int8_t	print_usage(char *self_path)
{
	dprintf(2, "Usage: %s [FILE].\n", self_path);
	return (FAILURE);
}