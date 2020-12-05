/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 11:21:04 by ezalos            #+#    #+#             */
/*   Updated: 2020/12/05 15:10:37 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

static int	ft_perror(char *msg)
{
	#if DEBUG
	perror(msg);
	#elif
	printf("Error: %s\n", msg);
	#endif
	return (EXIT_FAILURE);
}

static int	usage(char *binary)
{
	printf("Usage: %s <file>\n", binary);
	return (EXIT_SUCCESS);
}

int8_t		access_file(t_packer *packer, char *file)
{
	if ((packer->fd = open(file, O_RDONLY)) < 0)
	{
		ft_perror("open");
		return (FAILURE);
	}
	if (fstat(packer->fd, &(packer->stat)) < 0)
	{
		ft_perror("fstat");
		return (FAILURE);
	}
	if ((packer->content = (uint8_t*)mmap(0,
				packer->stat.st_size,
				PROT_READ | PROT_WRITE,
				MAP_PRIVATE,
				packer->fd,
				0)) == MAP_FAILED)
		{
			ft_perror("mmap");
			return (FAILURE);
		}
	return (SUCCESS);
}

int			main(int ac, char **av)
{
	t_packer	packer;

	if (ac < 2)
		return usage(av[0]);
	// test_cypher();
	if (FAILURE == access_file(&packer, av[1]))
		return (EXIT_FAILURE);
	new_try((Elf64_Ehdr*)packer.content, packer.content, &packer);
	return (EXIT_SUCCESS);
}
