/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_library.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 10:57:23 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/16 13:54:19 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

#define MAX_ARGS 3

#define BYTECODE_JMP 1
#define BYTECODE_MEM_RIGHTS 2
#define BYTECODE_CALL_DECRYPT 3
#define BYTECODE_WRITE 4

#define BIG_ENDIAN ELFDATA2LSB
#define LITLLE_ENDIAN 2

#define ARCHITECTURE_32 ELFCLASS32
#define ARCHITECTURE_64 ELFCLASS64

#define BYTECODE_LIB_LEN 3

typedef struct s_bytecode
{
	int type;
	uint8_t *bytecode;
	size_t size;
	size_t arg_nb;
	size_t arg_len[MAX_ARGS];
	size_t arg_offset[MAX_ARGS];

} t_bytecode;

extern t_bytecode bytecode_lib[BYTECODE_LIB_LEN];

t_bytecode bytecode_lib[BYTECODE_LIB_LEN] = {
	{BYTECODE_JMP,
	 {0xe9, 0x00, 0x00, 0x00, 0x00},
	 5,
	 1,
	 {4},
	 {1},
	 ARCHITECTURE_64},
	{BYTECODE_WRITE,
	 {
		 0x57, // push   rdi
		 0x56, // push   rsi
		 0x52, // push   rdx
		 0x50, // push   rax

		 0x68, 0x2e, 0x2e, 0x2e, 0x2e, // push   0x2e2e2e2e
		 0x48, 0x89, 0xe6,			   // mov    rsi,rsp
		 0xbf, 0x01, 0x00, 0x00, 0x00, // mov    edi,0x1
		 0xba, 0x04, 0x00, 0x00, 0x00, // mov    edx,0x4
		 0xb8, 0x01, 0x00, 0x00, 0x00, // mov    eax,0x1
		 0x0f, 0x05,				   // syscall

		 0x68, 0x57, 0x4f, 0x4f, 0x44, // push   0x444f4f57
		 0x48, 0x89, 0xe6,			   // mov    rsi,rsp
		 0xbf, 0x01, 0x00, 0x00, 0x00, // mov    edi,0x1
		 0xba, 0x04, 0x00, 0x00, 0x00, // mov    edx,0x4
		 0xb8, 0x01, 0x00, 0x00, 0x00, // mov    eax,0x1
		 0x0f, 0x05,				   // syscall

		 0x68, 0x59, 0x2e, 0x2e, 0x2e, // push   0x2e2e2e59
		 0x48, 0x89, 0xe6,			   // mov    rsi,rsp
		 0xbf, 0x01, 0x00, 0x00, 0x00, // mov    edi,0x1
		 0xba, 0x04, 0x00, 0x00, 0x00, // mov    edx,0x4
		 0xb8, 0x01, 0x00, 0x00, 0x00, // mov    eax,0x1
		 0x0f, 0x05,				   // syscall

		 0x68, 0x2e, 0x0a, 0x00, 0x00, // push   0xa2e
		 0x48, 0x89, 0xe6,			   // mov    rsi,rsp
		 0xbf, 0x01, 0x00, 0x00, 0x00, // mov    edi,0x1
		 0xba, 0x02, 0x00, 0x00, 0x00, // mov    edx,0x2
		 0xb8, 0x01, 0x00, 0x00, 0x00, // mov    eax,0x1
		 0x0f, 0x05,				   // syscall

		 0x58, // pop    rax
		 0x58, // pop    rax
		 0x58, // pop    rax
		 0x58, // pop    rax

		 0x58, // pop    rax
		 0x5a, // pop    rdx
		 0x5e, // pop    rsi
		 0x5f  // pop    rdi
	 },
	 0x70,
	 0,
	 NULL,
	 NULL,
	 ARCHITECTURE_64},
}