/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   head.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 11:15:02 by ldevelle          #+#    #+#             */
/*   Updated: 2021/03/10 19:05:48 by ezalos           ###   ########.fr       */
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

# define MALLOC_ERROR				"Malloc returned NULL"

# define NAME_OUT_PACKER			"woody.out"

# include "librbt.h"
# include "libft.h"

# define HDR_TYPE_NONE		0
# define HDR_TYPE_PHDR		1
# define HDR_TYPE_SHDR		2

typedef struct	s_zone
{
	size_t		offset;
	uint64_t	vaddr;
	size_t		size;
}				t_zone;

typedef struct s_sheader	t_sheader;


typedef struct	s_pheader
{
	uint8_t		type;
	Elf64_Phdr 	*phdr;
	t_rbt 		*shdr_tree;
	t_sheader	**shdr_array;
	int			available_size; //int64_t ?
}				t_pheader;

typedef struct	s_sheader
{
	uint8_t		type;
	Elf64_Shdr	*shdr;
	t_pheader	*parent_phdr;
	int			available_size; // int64_t
}				t_sheader;


typedef struct stat t_stat;

typedef struct	s_packer
{
	// int			fd;
	char 			*self_path;
	char 			*out;
	uint8_t			*content;
	t_zone			z_text;
	t_rbt			*phdr_tree;
	t_pheader		**phdr_array;
	t_list			*to_crypt;  //list of program headers of segments we want to crypt
	t_list			*to_inject; //list of zones we can inject code in, ordered by dec size
	// t_stat		stat;

	uint64_t		size;
}					t_packer;

#define _RED "\x1b[31m"
#define _GREEN "\x1b[32m"
#define _BLUE "\x1b[34m"
#define _YELLOW "\x1b[33m"
#define _MAGENTA "\x1b[35m"
#define _CYAN "\x1b[36m"
#define _RESET "\x1b[0m"
#define _BROWN "\x1b[38;2;238;205;163m"
#define _PINK "\x1b[38;2;239;100;100m"
#define _PURPLE "\x1b[38;2;101;78;163m"
#define _ORANGE "\x1b[38;2;155;75;43m"
#define _TURQUOISE "\x1b[38;2;68;140;121m"
#define CU_RESET "\033[0;0H"
#define CU_LOAD "\033[u"
#define CU_SAVE "\033[s"
#define CL_SCREEN "\033[2J"

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
