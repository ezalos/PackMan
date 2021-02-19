/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   head.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 11:15:02 by ldevelle          #+#    #+#             */
/*   Updated: 2021/02/19 12:34:40 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAD_H
# define HEAD_H

# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <error.h>
# include <errno.h>
# include <stdio.h>
# include <inttypes.h>
# include <stdlib.h>
# include <elf.h>
# include <string.h>
# include <sys/types.h>
# include <unistd.h>

# define DEBUG		1

# define SUCCESS	0
# define FAILURE	-1

# define TRUE		1
# define FALSE		0

# define FILE_NOT_ELF_ERROR			"The file is not of type ELF"
# define FILE_NOT_64_ERROR			"The file architecture is not x86_64"
# define FILE_BIG_ENDIAN_ERROR		"The file is big endian"
# define FILE_NOT_EXECUTABLE_ERROR	"The file is not executable"
# define FILE_FORMAT_ERROR			"The file is malformed or truncated"

# define NAME_OUT_PACKER			"woody.out"

typedef struct	s_zone
{
	size_t		offset;
	size_t		size;
}				t_zone;

typedef struct	s_header
{
	uint8_t		h_type;
	Elf64_Phdr *phdr;
	Elf64_Shdr	*shdr;
	size_t		available_size;
}				t_header;

typedef struct stat t_stat;

typedef struct	s_packer
{
	// int			fd;
	char 			*self_path;
	char 			*out;
	uint8_t			*content;
	t_zone			z_text;
	// t_stat		stat;
	uint64_t		size;
}					t_packer;

# include "prototypes_woody_woodpacker.h"

void		ft_cypher(char *data, int len, char key);
void 		print_section_header(t_packer *packer, Elf64_Shdr *shdr);
void 		print_program_header(Elf64_Phdr *phdr);
void 		print_elf_header(Elf64_Ehdr *elf);
char 		*get_sec_name(t_packer *packer, Elf64_Shdr *shdr);
Elf64_Shdr 	*get_section_header(t_packer *packer, uint32_t index);
Elf64_Phdr 	*get_program_header(t_packer *packer, uint32_t index);
void 		browse_file(t_packer *packer);
int8_t		print_error(char *self_path, char *error);
int8_t		print_usage(char *self_path);


#endif
