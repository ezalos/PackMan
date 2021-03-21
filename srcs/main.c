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
	print_elf_header((Elf64_Ehdr*)packer->content);
	parse_elf(packer);
	cave_gathering(packer);
	printf("\n*** %s: Printing results\n", __func__);
	parse_print(packer);
	printf("\n*** %s: Get zones to inject\n", __func__);
	packer->caves = get_zones(packer, PT_LOAD, PF_X | PF_R, data_filler_cave);
	if (packer->caves)
	{
		printf("\n*** %s: Chirurgy\n", __func__);
		if (chirurgy(packer) != FAILURE)
		{
			packer->out = NAME_OUT_PACKER;
			printf("\n*** %s: Save woody\n", __func__);
			save_woody(packer);
		}
	}
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

void		print_zones(t_list *zones)
{
	t_zone	*zone;

	printf("PRINT ZONE\n");
	while (zones)
	{
		printf("\n");
		zone = (t_zone*)(zones->data);
		printf("offset : %lu\n", zone->offset);
		printf("vaddr  : %lu\n", zone->vaddr);
		printf("size   : %lu\n", zone->size);
		print_program_header(zone->phdr);
		printf("\n");
		zones = zones->next;
	}
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
	// parse_elf(&packer);
	if (ret == SUCCESS)
		ret = pack_file(&packer);
	printf("GET ZONES\n");
	packer.to_crypt = get_zones(&packer, PT_LOAD, PF_R, &data_filler_zone_to_crypt);
	print_zones(packer.to_crypt);
	packer.caves = get_zones(&packer, PT_LOAD, PF_X | PF_R, &data_filler_cave);
	print_zones(packer.caves);
	if (munmap((void *)packer.content, (size_t)packer.size) == FAILURE)
		return (print_error(packer.self_path, strerror(errno)));
	//free function
	return (ret == SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE);
	// new_try((Elf64_Ehdr *)packer.content, packer.content, &packer);
	// return (EXIT_SUCCESS);
}
