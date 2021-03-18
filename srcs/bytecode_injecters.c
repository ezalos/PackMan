/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_injecters.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 19:29:02 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/18 11:17:00 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

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

// NEED TO TEST IF NO INCLUDES -> Test main ret 0 only

// #include <sys/mman.h>
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
//						- Insert mem_rights (* 2, if possible (Cleaner) -> NOO)
//						- jump
//						- at least 1 instruction:
//							-> the largest obligatory bytecode
//				else -> change 1 phdr rights:
//					Choose it -> fit all or mem change
//					Redo ope
//		Then can look for Load No_Exe
//		Then No-Load No-Exe
//		Bss ?



void		update_zone(t_zone *zone, t_btc *inst)
{
	zone->offset = zone->offset + inst->size;
	zone->size = zone->size - inst->size;
}

void		undo_update_zone(t_zone *zone, t_btc *inst)
{
	zone->offset = zone->offset - inst->size;
	zone->size = zone->size + inst->size;
}

uint8_t		can_i_write(t_zone *zone, t_btc *inst)
{
	return (zone->size <= inst->size);
}

void		write_btc(t_btc *inst, t_zone *zone, t_packer *packer)
{
	//TODO : todo
	inst++;
	zone++;
	packer++;
	return;
}

// UNSAFE DOES NOT CHECK FOR WRITEABILITY OR SIZE
void    inject_jump(t_packer *packer, uint8_t* dest, int arg1)
{
    uint8_t opcode = 0xe9;
	(void)packer;
    // if (!is_same_endian(packer))
    // {
    //     change_endian(&arg1, sizeof(arg1));
    // }
    dest[0] = opcode;
    ft_memcpy(dest + 1, &arg1, sizeof(arg1));
}


void	inject_write(t_packer *packer, uint8_t* dest)
{
    uint8_t payload[SIZE_WRITE] = {
		0x48, 0x83, 0xec, 0x10,						//sub    rsp,0x10
		0xc7, 0x04, 0x24, 0x2e, 0x2e, 0x2e, 0x2e,	//mov    DWORD PTR [rsp],0x2e2e2e2e
		0xc7, 0x44, 0x24, 0x04, 0x57, 0x4f, 0x4f,	//mov    DWORD PTR [rsp+0x4],0x444f4f57
		0x44, 
		0xc7, 0x44, 0x24, 0x08, 0x59, 0x2e, 0x2e, 	//mov    DWORD PTR [rsp+0x8],0x2e2e2e59
		0x2e,
		0xc7, 0x44, 0x24, 0x0c, 0x2e, 0x0a, 0x00,	//mov    DWORD PTR [rsp+0xc],0xa2e
		0x00, 
		0xbf, 0x01, 0x00, 0x00, 0x00,				//mov    edi,0x1
		0x48, 0x89, 0xe6,							//mov    rsi,rsp
		0xba, 0x0e, 0x00, 0x00, 0x00,				//mov    edx,0xe
		0xb8, 0x01, 0x00, 0x00, 0x00,				//mov    eax,0x1
		0x0f, 0x05									//syscall 
	};
	(void)packer;
	ft_memcpy(dest, payload, SIZE_WRITE);
}

void	inject_def_crypt(t_packer *packer, uint8_t *dest)
{
	uint8_t	payload[SIZE_DEF_CRYPT] = {
		0x55,                     	//push   rbp
		0x48, 0x89, 0xe5,           //mov    rbp,rsp
		0x53,                  		//push   rbx
		0x4d, 0x31, 0xc0,           //xor    r8,r8
		0x48, 0x31, 0xdb,           //xor    rbx,rbx
		0x48, 0x31, 0xc9,           //xor    rcx,rcx
		0x48, 0x31, 0xc0,           //xor    rax,rax
		// 0000000000000058 <_do_cypher>:
		0xfe, 0xc3,					//inc    bl
		0x02, 0x0c, 0x1a,          	//add    cl,BYTE PTR [rdx+rbx*1]
		0x8a, 0x04, 0x1a,           //mov    al,BYTE PTR [rdx+rbx*1]
		0x8a, 0x24, 0x0a,     	    //mov    ah,BYTE PTR [rdx+rcx*1]
		0x88, 0x24, 0x1a,           //mov    BYTE PTR [rdx+rbx*1],ah
		0x88, 0x04, 0x0a,           //mov    BYTE PTR [rdx+rcx*1],al
		0x48, 0x31, 0xc0,           //xor    rax,rax
		0x8a, 0x04, 0x1a,           //mov    al,BYTE PTR [rdx+rbx*1]
		0x02, 0x04, 0x0a,           //add    al,BYTE PTR [rdx+rcx*1]
		0x8a, 0x04, 0x02,           //mov    al,BYTE PTR [rdx+rax*1]
		0x4a, 0x31, 0x04, 0x07,     //xor    QWORD PTR [rdi+r8*1],rax
		0x49, 0xff, 0xc0,           //inc    r8
		0x49, 0x39, 0xf0,           //cmp    r8,rsi
		0x75, 0xd7,                 //jne    58 <_do_cypher>
		0x5b,                      	//pop    rbx
		0xc9,                      	//leave  
		0xc3                      	//ret  
	};



	(void)packer;
	ft_memcpy(dest, payload, SIZE_DEF_CRYPT);
}

void	inject_init_perm(t_packer *packer, uint8_t *dest)
{
	uint8_t	payload[SIZE_INIT_PERM] = {
		0x55,                      					//push   rbp
		0x48, 0x89, 0xe5,               			//mov    rbp,rsp
		0xb9, 0xff, 0x00, 0x00, 0x00,       		//mov    ecx,0xff
		//<_fill_permutations>:
		0x88, 0x0c, 0x0f,                			//mov    BYTE PTR [rdi+rcx*1],cl
		0xe2, 0xfb,                   				//loop   9 <_fill_permutations>
		0x88, 0x0f,                   				//mov    BYTE PTR [rdi],cl
		0xc9,                     					//leave  
		0xc3                     			 		//ret    
	};

	(void)packer;
	ft_memcpy(dest, payload, SIZE_INIT_PERM);
}

void	inject_key_sched(t_packer *packer, uint8_t *dest)
{
	uint8_t payload[SIZE_KEY_SCHED] = {
		0x5,                      					//push   rbp
		0x48, 0x89, 0xe5,                			//mov    rbp,rsp
		0x48, 0x31, 0xc9,                			//xor    rcx,rcx
		0x48, 0x31, 0xd2,                			//xor    rdx,rdx
		//<_do_permutations>:
		0x48, 0x31, 0xc0,                			//xor    rax,rax
		0x88, 0xc8,                   				//mov    al,cl
		0x24, 0x07,                   				//and    al,0x7
		0x8a, 0x04, 0x06,                			//mov    al,BYTE PTR [rsi+rax*1]
		0x00, 0xc2,                   				//add    dl,al
		0x8a, 0x04, 0x0f,                			//mov    al,BYTE PTR [rdi+rcx*1]
		0x00, 0xc2,                   				//add    dl,al
		0x8a, 0x04, 0x0f,                			//mov    al,BYTE PTR [rdi+rcx*1]
		0x8a, 0x24, 0x17,                			//mov    ah,BYTE PTR [rdi+rdx*1]
		0x88, 0x24, 0x0f,                			//mov    BYTE PTR [rdi+rcx*1],ah
		0x88, 0x04, 0x17,               			//mov    BYTE PTR [rdi+rdx*1],al
		0x48, 0xff, 0xc1,                			//inc    rcx
		0x48, 0x81, 0xf9, 0xff, 0x00, 0x00, 0x00,	//cmp    rcx,0xff
		0x7e, 0xd7,                   				//jle    1c <_do_permutations>
		0xc9,                      					//leave  
		0xc3                      					//ret
	};

	(void)packer;
	ft_memcpy(dest, payload, SIZE_KEY_SCHED);
}
