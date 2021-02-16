/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 11:21:04 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/16 18:36:23 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

// static int	usage(char *binary)
// {
// 	printf("Usage: %s <file>\n", binary);
// 	return (EXIT_SUCCESS);
// }

int8_t		access_file(t_packer *packer, char *file)
{
	int 	fd;
	off_t	size;

	if ((fd = open(file, O_RDONLY)) < 0)
		return (print_error(packer->self_path, strerror(errno)));
	if ((size = lseek(fd, 0L, SEEK_END)) == -1)
		return (print_error(packer->self_path, strerror(errno)));
	packer->size = size;
	if ((packer->content = (uint8_t *)mmap(0,
				(size_t)packer->size,
				PROT_READ | PROT_WRITE,
				MAP_PRIVATE,
				fd,
				0)) == MAP_FAILED)
	{
		close(fd);
		return (print_error(packer->self_path, strerror(errno)));
	}
	close(fd);
	return (SUCCESS);
}

// choper la zone a crypter
// choper la zone ou s'inserer
// modifier le entry point (plus complexe), et changer le jump final dans la zone que l'on insere
// crypter

int8_t		pack_file(t_packer *packer)
{
	(void)packer;
	return (SUCCESS);
}

// memcpy ? c'est pas memcmp plutot ? ==> BIen vu ;)
int			ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((unsigned char *)s1)[i] != ((unsigned char *)s2)[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
		i++;
	}
	return (0);
}

int8_t		check_elf_header(t_packer *packer)
{
	Elf64_Ehdr			*ehdr;
	const char	ElfMagic[] = {0x7f, 'E', 'L', 'F'};

	if (FALSE == is_secure_access(packer->size, 0, sizeof(Elf64_Ehdr)))
		return (print_error(packer->self_path, FILE_FORMAT_ERROR));
	ehdr = (Elf64_Ehdr *)packer->content;
	if (ft_memcmp(ehdr->e_ident, ElfMagic, 4) != 0)
		return (print_error(packer->self_path, FILE_NOT_ELF_ERROR));
	if (ehdr->e_ident[EI_CLASS] == 1)
		return (print_error(packer->self_path, FILE_NOT_64_ERROR));
	if (ehdr->e_ident[EI_DATA] != 1)
		return (print_error(packer->self_path, FILE_BIG_ENDIAN_ERROR));
	return (SUCCESS);
}

int			main(int ac, char **av)
{
	t_packer	packer;
	int			ret;

	if (ac < 2)
		// return usage(av[0]);
		return (print_usage(av[0]));//Should take av[0] as an argument
	// test_cypher();
	packer.self_path = av[0];
	if (FAILURE == access_file(&packer, av[1]))
		return (EXIT_FAILURE);
	ret = check_elf_header(&packer);
	if (ret == SUCCESS)
		ret = pack_file(&packer);
	browse_file(&packer);
	if (munmap((void *)packer.content, (size_t)packer.size) == FAILURE)
		return (print_error(packer.self_path, strerror(errno)));
	//free function
	return (ret == SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE);
	// new_try((Elf64_Ehdr *)packer.content, packer.content, &packer);
	// return (EXIT_SUCCESS);
}
