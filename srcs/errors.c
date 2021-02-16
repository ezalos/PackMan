/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 11:31:22 by rkirszba          #+#    #+#             */
/*   Updated: 2021/02/16 18:30:34 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t	print_error(char *self_path, char *error)
{
	dprintf(2, "%s: error: %s.\n", self_path, error);
	return (FAILURE);
}

int8_t	print_usage(char *self_path)
{
	dprintf(2, "Usage: %s [FILE].\n", self_path);
	return (FAILURE);
}