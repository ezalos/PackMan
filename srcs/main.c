/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 11:21:04 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/19 11:1:0128 by ezalos           ###   ########.fr       */
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

int8_t		get_zones_to_crypt(t_packer *packer)
{
	(void)packer;
	return (SUCCESS);
}

int8_t		get_payload_zones(t_packer *packer)
{
	// recuperer les zones espacees + adresse header des segments + sections auxquelles elles sont rattachees
	// verification qu'il y a suffisamment d'espace
	// si non, verifier qu'il y a de un .NOTE avec suffisamment d'espace

	(void)packer;
	return (SUCCESS);
}

int8_t save_woody(t_packer *packer)
{
	int fd;

	// print_symbol_code(packer->content, packer->z_text.offset, packer->z_text.size);
	// ft_cypher((void*)(((size_t)packer->content) + packer->z_text.offset), packer->z_text.size, 42);

	fd = open(packer->out, O_WRONLY | O_CREAT);
	if (fd != -1 && write(fd, packer->content, packer->size) == (ssize_t)packer->size)
		return (SUCCESS);
	return (FAILURE);
}

int8_t		pack_file(t_packer *packer)
{
	int8_t	ret;

	ret = SUCCESS;
	printf("Content: %p\tsize: %zu\n", packer->content, packer->size);
	// ret = get_zones_to_crypt(packer);
	// if (ret == SUCCESS)
	// 	ret = get_payload_zones(packer);
	// if (ret == SUCCESS)
	// {
	// 	write_payload(packer);
	// 	crypt_file(packer);
	// }
	return (ret);
	packer->out = NAME_OUT_PACKER;
	// browse_file(packer);
	save_woody(packer); 
	return (ret);
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


// int8_t check_elf_header(t_packer *packer)
// {
// 	Elf64_Ehdr *ehdr;
// 	const char ElfMagic[] = {0x7f, 'E', 'L', 'F'};
// 	int8_t retval;

// 	retval  = FAILURE;
// 	if (TRUE == is_secure_access(packer->size, 0, sizeof(Elf64_Ehdr)))
// 	{
// 		ehdr = (Elf64_Ehdr *)packer->content;
// 		if (ft_memcmp(ehdr->e_ident, ElfMagic, 4) != 0)
// 		{
// 			if (ehdr->e_ident[EI_CLASS] == 1)
// 			{
// 				if (ehdr->e_ident[EI_DATA] != 1)
// 					retval = SUCCESS;
// 				else
// 					print_error(packer->self_path, FILE_BIG_ENDIAN_ERROR);
// 			}
// 			else
// 				 print_error(packer->self_path, FILE_NOT_64_ERROR);
// 		}
// 		else
// 			print_error(packer->self_path, FILE_NOT_ELF_ERROR);
// 	} 
// 	else
// 		print_error(packer->self_path, FILE_FORMAT_ERROR);
// 	return (retval);
// }

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
	parse_elf(&packer);
	if (ret == SUCCESS)
		ret = pack_file(&packer);
	if (munmap((void *)packer.content, (size_t)packer.size) == FAILURE)
		return (print_error(packer.self_path, strerror(errno)));
	//free function
	return (ret == SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE);
	// new_try((Elf64_Ehdr *)packer.content, packer.content, &packer);
	// return (EXIT_SUCCESS);
}
