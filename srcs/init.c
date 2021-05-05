/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 23:16:07 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/01 21:30:26 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int8_t		access_file(t_packer *packer, char *file)
{
	int 	fd;
	off_t	size;

	if ((fd = open(file, O_RDONLY)) < 0)
		return (print_error(packer->self_path, strerror(errno)));
	if ((size = lseek(fd, 0L, SEEK_END)) == -1)
	{
		close(fd);
		return (print_error(packer->self_path, strerror(errno)));
	}
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

int8_t		check_elf_header(t_packer *packer)
{
	Elf64_Ehdr			*ehdr;

	if (FALSE == is_secure_access(packer->size, 0, sizeof(Elf64_Ehdr)))
		return (print_error(packer->self_path, FILE_FORMAT_ERROR));
	ehdr = (Elf64_Ehdr *)packer->content;
	if (ft_memcmp(ehdr->e_ident, ELFMAG, 4) != 0)
		return (print_error(packer->self_path, FILE_NOT_ELF_ERROR));
	if (ehdr->e_ident[EI_CLASS] != ELFCLASS64)
		return (print_error(packer->self_path, FILE_NOT_64_ERROR));
	if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB)
		return (print_error(packer->self_path, FILE_BIG_ENDIAN_ERROR));
	return (SUCCESS);
}

int8_t		check_sacred_memory_size(t_packer *packer)
{
	packer->sacred_memory_size
		= sizeof(Elf64_Phdr) * ((Elf64_Ehdr *)packer->content)->e_phnum
			+ ((Elf64_Ehdr *)packer->content)->e_phoff;
	if (packer->sacred_memory_size > packer->size)
		return (print_error(packer->self_path, FILE_FORMAT_ERROR));
	return (SUCCESS);
}

void		get_conf(t_packer *packer)
{
	char	*env;
	//TODO Louis: faire les confs
	//getenv renvoie NULL si rien dans l'env, sinon renvoie char *str (qu'il ne faut pas free)
	env = getenv("EXAMPLE_ENV");
	if (env && !ft_strcmp(env, "conf"))
		packer->example_env = 1;

}


int8_t		init(t_packer *packer, char **av)
{
	ft_bzero(packer, sizeof(t_packer));
	packer->self_path = av[0];
	packer->out = NAME_OUT_PACKER;
	if (FAILURE == access_file(packer, av[1]))
		return (FAILURE);
	if (FAILURE == check_elf_header(packer))
		return (FAILURE);
	if (FAILURE == check_sacred_memory_size(packer))
		return (FAILURE);
	logging("**SACRED MEMORY SIZE = %lu\n", packer->sacred_memory_size);	
	if (FAILURE == parse_elf(packer))
		return (FAILURE);
	return (SUCCESS);
}