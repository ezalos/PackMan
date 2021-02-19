/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/18 15:14:47 by ezalos            #+#    #+#             */
/*   Updated: 2021/02/18 17:35:1 by erkirsabl        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

// 0000000000001080 <_start>:
//     1080:	f3 0f 1e fa          	endbr64 
//     1084:	31 ed                	xor    ebp,ebp
//     1086:	49 89 d1             	mov    r9,rdx
//     1089:	5e                   	pop    rsi
//     108a:	48 89 e2             	mov    rdx,rsp
//     108d:	48 83 e4 f0          	and    rsp,0xfffffffffffffff0
//     1091:	50                   	push   rax
//     1092:	54                   	push   rsp
//     1093:	4c 8d 05 a6 01 00 00 	lea    r8,[rip+0x1a6]        # 1240 <__libc_csu_fini>
//     109a:	48 8d 0d 2f 01 00 00 	lea    rcx,[rip+0x12f]        # 11d0 <__libc_csu_init>
//     10a1:	48 8d 3d c1 00 00 00 	lea    rdi,[rip+0xc1]        # 1169 <main>
//     10a8:	ff 15 32 2f 00 00    	call   QWORD PTR [rip+0x2f32]        # 3fe0 <__libc_start_main@GLIBC_2.2.5>
//     10ae:	f4                   	hlt    
//     10af:	90                   	nop

#define PAYLOAD_INIT_SIZE	0x13
uint8_t insert_init(t_packer *packer, int offset)
{
	uint8_t payload[PAYLOAD_INIT_SIZE] = {
		0xf3, 0x0f, 0x1e, 0xfa, // endbr64,
		0x31, 0xed,				// xor ebp, ebp
		0x49, 0x89, 0xd1,		// mov r9, rdx
		0x5e,					// pop rsi
		0x48, 0x89, 0xe2,		// mov rdx, rsp
		0x48, 0x83, 0xe4, 0xf0, // and rsp, 0xfffffffffffffff0
		0x50,					// push rax
		0x54,					// push rsp
	};
	int i;

	i = -1;
	while ((size_t)++i < PAYLOAD_INIT_SIZE)
		packer->content[offset + i] = payload[i];

	return (PAYLOAD_INIT_SIZE);
}

uint8_t	insert_jump(t_packer * packer, int offset_from, int offset_to)
{
	uint8_t instruction = 0xe9;
	int jump = offset_to - (offset_from + sizeof(instruction));
	uint8_t shift = sizeof(instruction) * 8;
	uint8_t written_size = sizeof(jump) + sizeof(instruction);
	size_t bytecode = 0;
	int i = -1;

	bytecode = (jump << shift) | instruction;
	while ((size_t)++i < written_size)
		packer->content[offset_from + i] = ((uint8_t *)&bytecode)[i];

	return (written_size);
}


// 0:   57                      push   rdi
//    1:   56                      push   rsi
//    2:   52                      push   rdx
//    3:   50                      push   rax
//    4:   68 2e 2e 2e 2e          push   0x2e2e2e2e
//    9:   48 89 e6                mov    rsi,rsp
//    c:   bf 01 00 00 00          mov    edi,0x1
//   11:   ba 04 00 00 00          mov    edx,0x4
//   16:   b8 01 00 00 00          mov    eax,0x1
//   1b:   0f 05                   syscall 
//   1d:   68 57 4f 4f 44          push   0x444f4f57
//   22:   48 89 e6                mov    rsi,rsp
//   25:   bf 01 00 00 00          mov    edi,0x1
//   2a:   ba 04 00 00 00          mov    edx,0x4
//   2f:   b8 01 00 00 00          mov    eax,0x1
//   34:   0f 05                   syscall 
//   36:   68 59 2e 2e 2e          push   0x2e2e2e59
//   3b:   48 89 e6                mov    rsi,rsp
//   3e:   bf 01 00 00 00          mov    edi,0x1
//   43:   ba 04 00 00 00          mov    edx,0x4
//   48:   b8 01 00 00 00          mov    eax,0x1
//   4d:   0f 05                   syscall 
//   4f:   68 2e 0a 00 00          push   0xa2e
//   54:   48 89 e6                mov    rsi,rsp
//   57:   bf 01 00 00 00          mov    edi,0x1
//   5c:   ba 02 00 00 00          mov    edx,0x2
//   61:   b8 01 00 00 00          mov    eax,0x1
//   66:   0f 05                   syscall 
//   68:   58                      pop    rax
//   69:   58                      pop    rax
//   6a:   58                      pop    rax
//   6b:   58                      pop    rax
//   6c:   58                      pop    rax
//   6d:   5a                      pop    rdx
//   6e:   5e                      pop    rsi
//   6f:   5f                      pop    rdi

#define PAYLOAD_SIZE	0x70
uint8_t		insert_write(t_packer *packer, int offset)
{
	uint8_t payload[PAYLOAD_SIZE] = {
		0x57, // push   rdi
		0x56, // push   rsi
		0x52, // push   rdx
		0x50, // push   rax

		0x68, 0x2e, 0x2e, 0x2e, 0x2e, // push   0x2e2e2e2e
		0x48, 0x89, 0xe6,			  // mov    rsi,rsp
		0xbf, 0x01, 0x00, 0x00, 0x00, // mov    edi,0x1
		0xba, 0x04, 0x00, 0x00, 0x00, // mov    edx,0x4
		0xb8, 0x01, 0x00, 0x00, 0x00, // mov    eax,0x1
		0x0f, 0x05,					  // syscall

		0x68, 0x57, 0x4f, 0x4f, 0x44, // push   0x444f4f57
		0x48, 0x89, 0xe6,			  // mov    rsi,rsp
		0xbf, 0x01, 0x00, 0x00, 0x00, // mov    edi,0x1
		0xba, 0x04, 0x00, 0x00, 0x00, // mov    edx,0x4
		0xb8, 0x01, 0x00, 0x00, 0x00, // mov    eax,0x1
		0x0f, 0x05,					  // syscall

		0x68, 0x59, 0x2e, 0x2e, 0x2e, // push   0x2e2e2e59
		0x48, 0x89, 0xe6,			  // mov    rsi,rsp
		0xbf, 0x01, 0x00, 0x00, 0x00, // mov    edi,0x1
		0xba, 0x04, 0x00, 0x00, 0x00, // mov    edx,0x4
		0xb8, 0x01, 0x00, 0x00, 0x00, // mov    eax,0x1
		0x0f, 0x05,					  // syscall

		0x68, 0x2e, 0x0a, 0x00, 0x00, // push   0xa2e
		0x48, 0x89, 0xe6,			  // mov    rsi,rsp
		0xbf, 0x01, 0x00, 0x00, 0x00, // mov    edi,0x1
		0xba, 0x02, 0x00, 0x00, 0x00, // mov    edx,0x2
		0xb8, 0x01, 0x00, 0x00, 0x00, // mov    eax,0x1
		0x0f, 0x05,					  // syscall

		0x58, // pop    rax
		0x58, // pop    rax
		0x58, // pop    rax
		0x58, // pop    rax

		0x58, // pop    rax
		0x5a, // pop    rdx
		0x5e, // pop    rsi
		0x5f  // pop    rdi
	};
	// uint8_t payload[PAYLOAD_SIZE] = {
	// 	0xbf, 0x01, 0x00, 0x00, 0x00, // mov    edi,0x1
	// 	0x68, 0x53, 0x6c, 0x74, 0x0a, // push   0xa746c53
	// 	0x48, 0x89, 0xe6,			  // mov    rsi,rsp
	// 	0xba, 0x04, 0x00, 0x00, 0x00, // mov    edx,0x4
	// 	0xb8, 0x01, 0x00, 0x00, 0x00, // mov    eax,0x1
	// 	0x0f, 0x05					  // syscall
	// };
	int 	i;
	
	i = -1;
	while ((size_t)++i < PAYLOAD_SIZE)
		packer->content[offset + i] = payload[i];

	return (PAYLOAD_SIZE);
}



#define BYTECODE_LEN 0x8

void		chirurgy(t_packer *packer, size_t offset, size_t size, Elf64_Phdr *phdr, Elf64_Shdr *shdr)
{
	Elf64_Ehdr 	*elf = (Elf64_Ehdr *)packer->content;
	// 1151 : 48 8d 35 ac 0e 00 00     lea rsi, [rip + 0xeac]

	size_t 		align = ((offset >> 4) << 4) + 16 - offset;

	size_t		written = 0;
	(void) size;
	// size_t lea = 0;
	// size_t ptr = elf->e_entry;
	printf("Entry point is: %lx\n", elf->e_entry);
	printf("Vaddr phdr is: %lx\n", phdr->p_vaddr);

	// written += insert_init(packer, offset + align + written);
	written += insert_write(packer, offset + align + written);
	written += insert_jump(packer, offset + align + written, elf->e_entry);

	print_symbol_code(packer->content, offset + align, written); // n_phdr->p_offset - (phdr->p_offset + phdr->p_filesz));

	elf->e_entry = offset + align;
	printf("New entry point is: %lx\n", elf->e_entry + align);
	phdr->p_filesz += written + align;
	phdr->p_memsz += written + align;
	shdr->sh_size += written + align;
	// shdr->sh_len += size + 3;
}