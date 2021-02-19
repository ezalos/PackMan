/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   browse_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 15:07:58 by rkirszba          #+#    #+#             */
/*   Updated: 2021/02/17 15:44:331 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

Elf64_Shdr *get_section_from_segment(t_packer *packer, Elf64_Phdr *phdr, uint32_t index)
{
    Elf64_Shdr *shdr;
    size_t min = phdr->p_offset + (size_t)packer->content;
    size_t max = min + phdr->p_filesz;
    int id_sh;

    id_sh = 0;
    shdr = get_section_header(packer, id_sh);
    while (shdr)
    {
        // printf("%lx > %lx ?\n", shdr->sh_offset + (size_t)packer->content, min);
        // printf("%lx < %lx ?\n", shdr->sh_offset + shdr->sh_size + (size_t)packer->content, max);
        if (shdr->sh_offset + (size_t)packer->content > min
        || shdr->sh_offset + shdr->sh_size + (size_t)packer->content < max)
        {
            // printf("In\n");
            if (shdr->sh_offset + (size_t)packer->content > min
            && shdr->sh_offset + shdr->sh_size + (size_t)packer->content < max)
            {
                // printf("GOOD: idx = %d\n", index);
                if (index == 0)
                    return shdr;
                index--;
            }
            else
            {
                // printf("ERROR: Section %d is beetween 2 segemnts\n", id_sh);
            }
        }
        id_sh++;
        shdr = get_section_header(packer, id_sh);
    }
    return shdr;
}


// 0x555555555060 real entry point
// 48 b8 35 08 40 00 00 00 00 00   mov rax, 0x0000000000400835
// ff e0                           jmp rax

// uint64_t construct_jump(int addr_from, int addr_to)
// {
// 	uint8_t 	instruction = 0xe9;
// 	int 		jump = addr_to - (addr_from + sizeof(instruction));
// 	size_t		bytecode = 0;
// 	uint8_t 	shift = sizeof(instruction) * 8;

// 	bytecode = (jump << shift) | instruction;
	
// 	return (bytecode);
// }

// => 0x7ffff7fcf114:	jmp    r12
//  | 0x7ffff7fcf117:	nop    WORD PTR [rax+rax*1+0x0]
//  | 0x7ffff7fcf120:	endbr64
//  | 0x7ffff7fcf124:	add    DWORD PTR [rdi+0x4],0x1
//  | 0x7ffff7fcf128:	ret
//  |->   0x555555555200:	movabs rax,0x555555555060
//        0x55555555520a:	add    BYTE PTR [rax],al
//        0x55555555520c:	add    BYTE PTR [rax],al
//        0x55555555520e:	add    BYTE PTR [rax],al

// write "Slt\n"
//    0:   bf 01 00 00 00          mov    edi,0x1
//    5:   68 53 6c 74 0a          push   0xa746c53
//    a:   48 89 e6                mov    rsi,rsp
//    d:   ba 04 00 00 00          mov    edx,0x4
//   12:   b8 01 00 00 00          mov    eax,0x1
//   17:   0f 05                   syscall 


// #define BYTECODE_LEN    0x8
// #define PAYLOAD_SIZE 	0x1e

// void	chirurgy(t_packer *packer, size_t offset, size_t size, Elf64_Phdr *phdr, Elf64_Shdr *shdr)
// {
//     Elf64_Ehdr *elf = (Elf64_Ehdr *)packer->content;
//     // 1151 : 48 8d 35 ac 0e 00 00     lea rsi, [rip + 0xeac]

// 	size_t align = ((offset >> 4) << 4) + 16 - offset;

// 	(void)size;
//     // size_t lea = 0;
//     // size_t ptr = elf->e_entry;
//     printf("Entry point is: %lx\n", elf->e_entry);
//     printf("Vaddr phdr is: %lx\n", phdr->p_vaddr);

// 	uint64_t code = construct_jump(offset + align, elf->e_entry);
// 	// uint8_t     code[BYTECODE_LEN] = {0x48, 0xb8, 0x0, 0x0, 0x0,
//     //         0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xe0};
//     // uint64_t    addr;
// 	// addr = 0x555555555060;
// 	// addr = 0x1060;
// 	// change_endian(&addr, 8);
//     // * (uint64_t *) (code + 2) = addr;
// 	printf("Code is: %lx\n", (size_t)code);

//     uint8_t payload[PAYLOAD_SIZE] = {
//         0xbf, 0x01, 0x00, 0x00, 0x00, // mov    edi,0x1
//         0x68, 0x53, 0x6c, 0x74, 0x0a, // push   0xa746c53
//         0x48, 0x89, 0xe6,             // mov    rsi,rsp
//         0xba, 0x04, 0x00, 0x00, 0x00, // mov    edx,0x4
//         0xb8, 0x01, 0x00, 0x00, 0x00, // mov    eax,0x1
//         0x0f, 0x05,                   // syscall
//         0xe9, 0x00, 0x00, 0x00, 0x00  // jmp to entrypoint
//     };

//     *(int32_t*)(payload + 26) = jump;
// 	int i = -1;
//     // while ((size_t)++i < 5)
// 		// packer->content[offset + align + i] = ((uint8_t *)&code)[i];
//     while ((size_t)++i < PAYLOAD_SIZE)
// 		packer->content[offset + align + i] = ((uint8_t *)&payload)[i];
// 	print_symbol_code(packer->content, offset + align, BYTECODE_LEN); // n_phdr->p_offset - (phdr->p_offset + phdr->p_filesz));
// 	elf->e_entry = offset + align;
// 	printf("New entry point is: %lx\n", elf->e_entry + align);

// 	phdr->p_filesz += size + align;
// 	phdr->p_memsz += size + align;
// 	shdr->sh_size += size + align;
// 	// shdr->sh_len += size + 3;

// }

// 0x5555555551f8 : jmp 0x555555bbb861

/*
0000000000001060 <_start>:
    1060:	f3 0f 1e fa          	endbr64 
    1064:	31 ed                	xor    ebp,ebp
    1066:	49 89 d1             	mov    r9,rdx
    1069:	5e                   	pop    rsi
    106a:	48 89 e2             	mov    rdx,rsp
    106d:	48 83 e4 f0          	and    rsp,0xfffffffffffffff0
    1071:	50                   	push   rax
    1072:	54                   	push   rsp
*/

int     ft_strcmp(const char *s1, const char *s2)
{
    size_t  i;

    if (!s1 || !s2)
        return (0);
    i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return (s1[i] - s2[i]);
}

void    browse_file(t_packer *packer)
{
    Elf64_Ehdr *elf = (Elf64_Ehdr *)packer->content;
    // Elf64_Phdr *phdr;
    // Elf64_Phdr *n_phdr;
    // Elf64_Shdr *shdr;
    // Elf64_Shdr *n_shdr;
    // Elf64_Shdr *n_n_shdr;
    // size_t available_size;
    // size_t total_size;
    // size_t tmp;

    // int id_ph = -1;
    int id_sh;

    print_elf_header(elf);
    id_sh = -1;
    int n_shdr = elf->e_shnum;
    // shdr = (Elf64_Shdr*)(packer->content + elf->e_shoff);
    while (++id_sh < n_shdr)
        print_section_header(packer, get_section_header(packer, id_sh));

    // while (++id_ph < elf->e_phnum)
    // {
    //     phdr = get_program_header(packer, id_ph);
	// 	printf("Phdr %d-%p\n", id_ph, phdr);//, phdr->p_flags & PF_X, phdr->p_type == PT_LOAD);
	// 	if (phdr && phdr->p_flags & PF_X && phdr->p_type == PT_LOAD)
	// 	{
    //         total_size = 0;
    //         printf("Found section %d which is of great interest\n", id_ph);
    //         print_program_header(phdr);

    //         id_sh = 0;
    //         // printf("0\n");
    //         shdr = get_section_from_segment(packer, phdr, id_sh);
    //         // printf("1\n");
    //         while (shdr)
    //         {
	// 			// printf("Ya\n");
	// 			if (shdr && !ft_strcmp(get_sec_name(packer, shdr), ".text"))
	// 			{
	// 				// printf("Yo\n");
	// 				print_section_header(packer, shdr);
	// 				packer->z_text.offset = shdr->sh_offset;
	// 				packer->z_text.size = shdr->sh_size;
	// 			}
    //             // printf("2\n");
    //             print_section_header(packer, shdr);

    //             if ((n_shdr = get_section_from_segment(packer, phdr, id_sh)))
    //                 available_size = (n_shdr->sh_offset) - (shdr->sh_size + shdr->sh_offset);
    //             else
    //                 available_size = (phdr->p_filesz + phdr->p_offset) - (shdr->sh_size + shdr->sh_offset);
    //             print_symbol_code(packer->content, shdr->sh_offset, shdr->sh_size);
    //             printf("Available_size: %ld\n\n", available_size);
    //             total_size += available_size;
    //             tmp = (shdr->sh_size + shdr->sh_offset);
    //             (void)tmp;
    //             // printf("3\n");
    //             id_sh++;
    //             // printf("4\n");
    //             n_n_shdr = shdr;
    //             shdr = n_shdr;
    //         }
    //         printf("Total available size = %ld\n", total_size);
    //         if ((n_phdr = get_program_header(packer, id_ph + 1)))
    //         {
    //             printf("Size before next section: %ld\n", n_phdr->p_offset - (phdr->p_offset + phdr->p_filesz));
    //             chirurgy(packer, (phdr->p_offset + phdr->p_filesz), 8, phdr, n_n_shdr);
    //         }
    //     }
    // }
}