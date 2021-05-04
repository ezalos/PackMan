/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 19:41:39 by rkirszba          #+#    #+#             */
/*   Updated: 2021/05/04 19:42:00 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

static void	free_data(void *data)
{
	free(data);
}

// static void	free_shdr_array(t_sheader **shdr_array)
// {
// 	int	i;

// 	if (!shdr_array)	
// 		return ;
// 	i = -1;
// 	while (shdr_array[++i])
// 		free(shdr_array[i]);
// 	free(shdr_array);
	
// }

static void free_phdr_array(t_pheader **phdr_array)
{
	int	i;

	if (!phdr_array)
		return ;
	i = -1;
	while (phdr_array[++i])
	{
		// TODO: debug double free problem
		// free_shdr_array(phdr_array[i]->shdr_array);
		free(phdr_array[i]);
	}
	free(phdr_array);
}

void	free_all(t_packer *packer)
{
	ft_list_free(packer->caves, &free_data);
	ft_list_free(packer->to_crypt, &free_data);
	free_phdr_array(packer->phdr_array);
	if (packer->content)
		if (munmap(packer->content, packer->size) == -1)
		{
			print_error(packer->self_path, strerror(errno));
			exit(EXIT_FAILURE);
		}
}