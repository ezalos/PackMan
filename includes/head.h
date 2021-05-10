/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   head.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 11:15:02 by ldevelle          #+#    #+#             */
/*   Updated: 2021/05/10 09:41:51 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAD_H
# define HEAD_H

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
# include <time.h>
# include <sys/mman.h>
# include <stdarg.h>

# define DEBUG		0

extern int debug_level;

# define SUCCESS	0
# define FAILURE	-1

# define TRUE		1
# define FALSE		0

# define STATIC_KEY FALSE

/*
**	ERROR MANAGEMENT
*/

# define NO_PHDR_IN_BINARY					"The file does not possess any phdr"
# define FILE_NOT_ELF_ERROR					"The file is not of type ELF"
# define FILE_NOT_64_ERROR					"The file architecture is not x86_64"
# define FILE_BIG_ENDIAN_ERROR				"The file is big endian"
# define FILE_NOT_EXECUTABLE_ERROR			"The file is not executable"
# define FILE_FORMAT_ERROR					"The file is malformed or truncated"
# define SEGMENT_CONTENT_OUTSIDE_FILE		"Phdr n*%d has file_size out of file"
# define SEGMENT_MEMSIZE_SMALLER_FILE_SIZE	"Phdr n*%d has file_size > mem_size"
# define LOADABLE_SEGMENT_CONTAINS_LOAD		"Phdr loadable segment %d contains loadable segment %d"
# define LOADABLE_SEGMENT_SUPERPOSED		"Phdr %d and %d are superposed" 
# define LOADABLE_SEGMENT_OVERLAP			"Phdr %d and %d overlap each other" 
# define NO_EXECUTABLE_SEGMENT				"The file does not contain any loadable executable segment"
# define NO_LOADABLE_SEGMENT				"The file does not contain any loadable segment"
# define SECTION_CONTENT_OUTSIDE_FILE		"Shdr %d references memory out of file"
# define LIMIT_PHDR_RECURSIVE_CONTAIN		"File contains too many recursively contained phdr, stopping at : %d"

# define MALLOC_ERROR				"Malloc returned NULL"

# define NAME_OUT_PACKER			"woody.out"

# include "librbt.h"
# include "libft.h"

# define PERM_SIZE			0x100
# define KEY_SIZE			0x8

# define OFFSET_CALL_INIT_PERM	0x22
# define OFFSET_CALL_KEY_SCHED	0x33
# define OFFSET_CALL_CYPHER		(0x32 + 8)
# define OFFSET_CALL_FIND_ABS_ADDR 0x18

typedef enum	e_state
{
	NORMAL,
	OVERLAPPED,
	SUPERPOSED,
	CONTAINED,
}				t_estate;

# define CAVE_GATHER_MAX_PARENT	10000

typedef struct	s_state
{
	Elf64_Phdr	*parent[CAVE_GATHER_MAX_PARENT];
	Elf64_Phdr	*curr;
	Elf64_Phdr	*next;
	uint32_t	depth;
	uint32_t	index;
	t_estate	state;
	uint8_t		error;
	uint8_t		exit_parent;
	uint8_t		loop;
	size_t		size;
}				t_state;

typedef struct	s_zone
{
	size_t		offset;
	uint64_t	vaddr;
	size_t		size;
	uint8_t		used;
	uint8_t		last;
	Elf64_Phdr	*phdr;
}				t_zone;


typedef struct s_sheader	t_sheader;


typedef struct	s_pheader
{
	uint8_t		type;
	Elf64_Phdr 	*phdr;
	t_rbt 		*shdr_tree;
	t_sheader	**shdr_array;
	int64_t		available_size;
}				t_pheader;


typedef struct	s_sheader
{
	uint8_t		type;
	Elf64_Shdr	*shdr;
	t_pheader	*parent_phdr;
	int64_t		available_size;
}				t_sheader;


typedef struct stat t_stat;


typedef struct	s_packer
{
	char 			*self_path;
	char 			*out;
	uint8_t			*content;
	t_rbt			*phdr_tree;
	t_pheader		**phdr_array;
	t_list			*to_crypt;
	t_list			*caves;
	uint8_t			key[KEY_SIZE];
	size_t			new_e_entry;
	size_t			sacred_memory_size;
	uint8_t			strategy;
	uint8_t			strat_loadable;
	uint8_t			extend_crypt_choice;
	uint8_t			print_phdr_gather;
	uint64_t		size;
}					t_packer;

typedef struct	s_btc_args
{
	int			jump;
	uint64_t	mp_addr;
	size_t		mp_len;
	int			mp_prot;
	uint64_t	crypt_plaintext_vaddr;
	uint64_t	crypt_func_def_vaddr;
	uint64_t	crypt_func_init_perm_vaddr;
	uint64_t	crypt_func_key_sched_vaddr;
	uint64_t	crypt_func_find_abs_vaddr_vaddr;
	uint32_t	jmp_init_perm; // vaddr def_init_perm - (vaddr def_cypher_prepare + OFFSET_CALL_INIT_PERM)
	uint32_t	jmp_key_sched; // vaddr def_key_sched - (vaddr def_cypher_prepare + OFFSET_CALL_KEY_SCHED)
	uint32_t	jmp_def_cypher; //vaddr def_cypher - (vaddr call_cypher + OFFSET_CALL_CYPHER)
	uint32_t	jmp_find_abs_addr; //vaddr find_abs_addr - (vaddr call_cyper + OFFSET_CALL_FIND_ABS_ADDR)
	size_t		crypt_plaintext_size;
	uint8_t 	*crypt_key;
}				t_btc_args;


typedef void (*t_write_func)(uint8_t *dest, void *args);

typedef struct	s_btc
{
	int				type;
	size_t			size;
	t_write_func	func_ptr;
	t_btc_args		*args;
}				t_btc;

# define ENDIAN(x) (((Elf64_Ehdr *)x->content)->e_ident[EI_DATA] != ELFDATA2LSB)

/*
**	BYTECODE LIBRARY
*/

# define BYTECODE_LIB_LEN 			11

# define BTC_DEF_BEGIN				0
# define BTC_CALL_MPROTECT			1
# define BTC_DEF_CYPHER_PREPARE		2
# define BTC_CALL_CYPHER			3
# define BTC_DEF_WRITE				4		
# define BTC_DEF_END				5
# define BTC_CALL_JMP				6
# define BTC_DEF_INIT_PERM			7
# define BTC_DEF_KEY_SCHED			8
# define BTC_DEF_CYPHER				9
# define BTC_DEF_FIND_ABS_VADDR		10


# define SIZE_DEF_BEGIN				25
# define SIZE_CALL_MPROTECT			64
# define SIZE_DEF_CYPHER_PREPARE	55
# define SIZE_CALL_CYPHER			66
# define SIZE_DEF_WRITE				59		
# define SIZE_DEF_END				32
# define SIZE_CALL_JMP				5
# define SIZE_DEF_INIT_PERM			18
# define SIZE_DEF_KEY_SCHED			53
# define SIZE_DEF_CYPHER			61
# define SIZE_DEF_FIND_ABS_VADDR	46

# define MINIMAL_WOODY					FALSE
# define NB_STRAT						3
# define STRAT_LOADABLE_EXECUTE			0
# define STRAT_LOADABLE					1
# define STRAT_LOADABLE_LAST_SEGMENT	2

extern t_btc bytecode_lib[BYTECODE_LIB_LEN];

/*
**	COLORS
*/

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

/*
**	PROTOTYPES
*/

# include "prototypes_woody_woodpacker.h"

extern void	init_permutations_asm(uint8_t *permutations);
extern void	schedule_key_asm(uint8_t *permutations, uint8_t *key);
extern void crypt_zone_asm(uint8_t *zone, size_t len, uint8_t *permutations);

#endif
