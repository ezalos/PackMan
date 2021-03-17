/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_injecters.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 00:45:10 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/17 11:45:31 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

#define ENDIAN(x) (((Elf64_Ehdr *)x->content)->e_ident[EI_DATA] != ELFDATA2LSB)
#define IS64(x) (((Elf64_Ehdr *)x->content)->e_ident[EI_CLASS] != ELFCLASS64)

// Cahier des charges data:
//		Update cave size a mesure qu'elles se remplissent
//			-> Se rappeller de quel input l'a remplie et ou
//		Pouvoir calculer les offsets pour:
//			-> Jumps
//				-> connaitre offset pour argument -> only one that need can be in funct
//		Access to datas:
//			-> Size des bytecode
//		~ Eviter les ecritures lourdes:
//			-> Pointeurs sur fonctions ?
//			Liste des index obligatoires dans l'ordre
//			

// typedef struct s_bytecode
// {
// 	int		type;
// 	size_t	size;
// 	size_t	offset; -> pour arg du jump...
// 	void	*func_ptr; -> Si arguments necessaires, complexe
// 	size_t	arg_nb;
// } t_bytecode;

uint8_t test_endian(void)
{
    int     test_var = 1;
    uint8_t *test_endian = (uint8_t*)&test_var;

    return (test_endian[0] == 0);
}

uint8_t is_same_endian(t_packer *packer)
{
    return (test_endian() == ENDIAN(packer));
}

// #include <sys/mman.h>
// int mprotect(void *addr, size_t len, int prot);

// Algo resolution
//		if sorted(av_size)[0] in (Load + Exe) > total:
//			GOOD
//		else:
//			while biggest bytecode + jmp < bigest cave:
//				set cave used
//				set bytecode put (3 total + bonus)
//			if all of them:
//				GOOD
//			else:
//				needs to look again for inserting a mem right
//				if can we inject a mem rights change ?
//					It needs to:
//						- Insert all
//							OR
//						- Insert mem_rights (* 2, if possible (Cleaner))
//						- jump
//						- at least 1 instruction:
//							-> the largest obligatory bytecode
//				else -> change 1 phdr rights:
//					Choose it -> fit all or mem change
//					Redo ope
//		Then can look for Load No_Exe
//		Then No-Load No-Exe
//		Bss ?



// UNSAFE DOES NOT CHECK FOR WRITEABILITY OR SIZE
void    inject_jump(t_packer *packer, uint8_t* dest, int arg1)
{
    uint8_t opcode = 0xe9;
    if (!is_same_endian(packer))
    {
        change_endian(&arg1, sizeof(arg1));
    }
    dest[0] = opcode;
    ft_memcpy(dest + 1, &arg1, sizeof(arg1));
}

#define PAYLOAD_SIZE	0x70
void inject_write(t_packer *packer, uint8_t* dest)
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

	ft_memcpy(dest, payload, PAYLOAD_SIZE);
}