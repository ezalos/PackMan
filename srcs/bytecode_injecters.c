/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_injecters.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 19:29:02 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/25 17:00:16 by ezalos           ###   ########.fr       */
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


void	inject_def_begin(t_packer *packer, uint8_t *dest, void *args)
{
	uint8_t payload[SIZE_DEF_BEGIN] = {
	0x54,               //push   rsp
	0x55,               //push   rbp
	0x50,               //push   rax
	0x53,               //push   rbx
	0x51,               //push   rcx
	0x52,               //push   rdx
	0x56,               //push   rsi
	0x57,               //push   rdi
	0x41, 0x50,         //push   r8
	0x41, 0x51,         //push   r9
	0x41, 0x52,         //push   r10
	0x41, 0x53,         //push   r11
	0x41, 0x54,         //push   r12
	0x41, 0x55,         //push   r13
	0x41, 0x56,         //push   r14
	0x41, 0x57,   		//push   r15
	0x9c	        	//pushf  
	};

	(void)packer;
	(void)args;
	ft_memcpy(dest, payload, SIZE_DEF_BEGIN);
}

void	inject_call_mprotect(t_packer *packer, uint8_t *dest, void *args)
{
	uint8_t payload[SIZE_CALL_MPROTECT] = {
		0x48, 0x83, 0xec, 0x10,             		//sub    rsp,0x10
		0xc7, 0x04, 0x24, 0x00, 0x00, 0x00, 0x00,  	//mov    DWORD PTR [rsp],0x0
		0xc7, 0x44, 0x24, 0x04, 0x00, 0x00, 0x00,   //mov    DWORD PTR [rsp+0x4],0x0
		0x00, 
		0x48, 0x8b, 0x3c, 0x24,             		//mov    rdi,QWORD PTR [rsp]
		0xc7, 0x44, 0x24, 0x08, 0x00, 0x00, 0x00,   //mov    DWORD PTR [rsp+0x8],0x0
		0x00, 
		0xc7, 0x44, 0x24, 0x0c, 0x00, 0x00, 0x00,   //mov    DWORD PTR [rsp+0xc],0x0
		0x00, 
		0x48, 0x8b, 0x36,                			//mov    rsi,QWORD PTR [rsi]
		0x48, 0x31, 0xd2,                			//xor    rdx,rdx
		0xba, 0x03, 0x00, 0x00, 0x00,          		//mov    edx,0x3
		0x48, 0x31, 0xc0,                			//xor    rax,rax
		0xb8, 0x0a, 0x00, 0x00, 0x00,          		//mov    eax,0xa
		0x0f, 0x05,                   				//syscall 
		0x48, 0x83, 0xc4, 0x10,             		//add    rsp,0x10
	};

	(void)packer;
	(void)args;
	ft_memcpy(dest, payload, SIZE_CALL_MPROTECT);
}

void	inject_def_cypher_prepare(t_packer *packer, uint8_t *dest, void *args)
{
	uint8_t payload[SIZE_DEF_CYPHER_PREPARE] = {
		0x48, 0x81, 0xec, 0x08, 0x01, 0x00, 0x00,   //sub    rsp,0x108
		0xc7, 0x84, 0x24, 0x00, 0x01, 0x00, 0x00,   //mov    DWORD PTR [rsp+0x100],0xffffffff
		0xff, 0xff, 0xff, 0xff, 
		0xc7, 0x84, 0x24, 0x04, 0x01, 0x00, 0x00,   //mov    DWORD PTR [rsp+0x104],0xffffffff
		0xff, 0xff, 0xff, 0xff, 
		0x48, 0x8d, 0x3c, 0x24,             		//lea    rdi,[rsp]
		0xe8, 0xfc, 0xff, 0xff, 0xff,         		//call   22 <btc_def_cypher_prepare+0x22>
		0x48, 0x8d, 0x3c, 0x24,             		//lea    rdi,[rsp]
		0x48, 0x8d, 0xbc, 0x24, 0x00, 0x01, 0x00,  	//lea    rdi,[rsp+0x100]
		0x00, 
		0xe8, 0xfc, 0xff, 0xff, 0xff          		//call   33 <btc_def_cypher_prepare+0x33>
	};

	
	(void)args;
	ft_memcpy(payload + 0xe, packer->key, 0x4);
	ft_memcpy(payload + 0x16, packer->key + 0x4, 0x4);
	ft_memcpy(dest, payload, SIZE_DEF_CYPHER_PREPARE);
}

void	inject_call_cypher(t_packer *packer, uint8_t *dest, void *args)
{
	uint8_t payload[SIZE_CALL_CYPHER] = {
		0x48, 0x83, 0xec, 0x10,             		//sub    rsp,0x10
		0xc7, 0x04, 0x24, 0xff, 0xff, 0xff, 0xff,   //mov    DWORD PTR [rsp],0xffffffff
		0xc7, 0x44, 0x24, 0x04, 0xff, 0xff, 0xff,   //mov    DWORD PTR [rsp+0x4],0xffffffff
		0xff, 
		0x48, 0x8b, 0x3c, 0x24,             		//mov    rdi,QWORD PTR [rsp]
		0xc7, 0x44, 0x24, 0x08, 0xff, 0xff, 0xff,   //mov    DWORD PTR [rsp+0x8],0xffffffff
		0xff, 
		0xc7, 0x44, 0x24, 0x0c, 0xff, 0xff, 0xff,   //mov    DWORD PTR [rsp+0xc],0xffffffff
		0xff, 
		0x48, 0x8b, 0x74, 0x24, 0x08,          		//mov    rsi,QWORD PTR [rsp+0x8]
		0x48, 0x8d, 0x54, 0x24, 0x10,          		//lea    rdx,[rsp+0x10]
		0xe8, 0xfc, 0xff, 0xff, 0xff,          		//call   32 <btc_call_cypher+0x32>
		0x48, 0x83, 0xc4, 0x10             			//add    rsp,0x10
	};

	(void)packer;
	(void)args;

	// a priori les informations d'adresse et len seront dans args
	// pour l'adresse (il faut qu'elle soit absolue, moyen de le savoir avant l'execution du woody ?)
	// uint8_t *addr;
	// uint8_t *len;
	// addr = (uint8_t*)(&(args->addr));
	// len = (uint8_t*)(&(args->len));
	// ft_memcpy(payload + 0x7, addr, 0x4);
	// ft_memcpy(payload + 0xf, addr + 0x4, 0x4);
	// ft_memcpy(payload + 0x1b, len, 0x4);
	// ft_memcpy(payload + 0x23, len + 0x4, 0x4);

	ft_memcpy(dest, payload, SIZE_CALL_CYPHER);
}

void	inject_def_write(t_packer *packer, uint8_t* dest, void *args)
{
	uint8_t payload[SIZE_DEF_WRITE] = {
		0x48, 0x83, 0xec, 0x10,					  	//sub    rsp,0x10
		0xc7, 0x04, 0x24, 0x2e, 0x2e, 0x2e, 0x2e, 	//mov    DWORD PTR [rsp],0x2e2e2e2e
		0xc7, 0x44, 0x24, 0x04, 0x57, 0x4f, 0x4f, 	//mov    DWORD PTR [rsp+0x4],0x444f4f57
		0x44,
		0xc7, 0x44, 0x24, 0x08, 0x59, 0x2e, 0x2e, 	//mov    DWORD PTR [rsp+0x8],0x2e2e2e59
		0x2e,
		0xc7, 0x44, 0x24, 0x0c, 0x2e, 0x0a, 0x00, 	//mov    DWORD PTR [rsp+0xc],0xa2e
		0x00,
		0xbf, 0x01, 0x00, 0x00, 0x00, 				//mov    edi,0x1
		0x48, 0x89, 0xe6,			  				//mov    rsi,rsp
		0xba, 0x0e, 0x00, 0x00, 0x00, 				//mov    edx,0xe
		0xb8, 0x01, 0x00, 0x00, 0x00, 				//mov    eax,0x1
		0x0f, 0x05,					  				//syscall
		0x48, 0x83, 0xc4, 0x10          			//add    rsp,0x10

	};

	// revoir en faisant des xor rdi, rdi   xor rdx, rdx    xor rax, rax
	(void)packer;
	(void)args;
	ft_memcpy(dest, payload, SIZE_DEF_WRITE);
}

void	inject_def_end(t_packer *packer, uint8_t *dest, void *args)
{
	uint8_t payload[SIZE_DEF_END] = {
		// 0x48, 0x81, 0xc4, 0x18, 0x01, 0x00, 0x00,	//add    rsp,0x118
		0x9d,                      					//popf   
		0x41, 0x5f,                   				//pop    r15
		0x41, 0x5e,                   				//pop    r14
		0x41, 0x5d,                   				//pop    r13
		0x41, 0x5c,                   				//pop    r12
		0x41, 0x5b,                   				//pop    r11
		0x41, 0x5a,                   				//pop    r10
		0x41, 0x59,                   				//pop    r9
		0x41, 0x58,                   				//pop    r8
		0x5f,                      					//pop    rdi
		0x5e,                      					//pop    rsi
		0x5a,                      					//pop    rdx
		0x59,                      					//pop    rcx
		0x5b,                      					//pop    rbx
		0x58,                      					//pop    rax
		0x5d,                      					//pop    rbp
		0x5c                      					//pop    rsp
	};

	(void)packer;
	(void)args;
	ft_memcpy(dest, payload, SIZE_DEF_END);
}


// UNSAFE DOES NOT CHECK FOR WRITEABILITY OR SIZE
void	inject_call_jmp(t_packer *packer, uint8_t* dest, void *args)
{
    // uint8_t opcode = 0xe9;
	// (void)packer;
    // // if (!is_same_endian(packer))
    // // {
    // //     change_endian(&arg1, sizeof(arg1));
    // // }
    // dest[0] = opcode;
    // ft_memcpy(dest + 1, &arg1, sizeof(arg1));

	uint8_t payload[SIZE_CALL_JMP] = {
		0xe9, 0xfc, 0xff, 0xff, 0xff	//jmp    1 <call_jmp+0x1>
	};
	(void)packer;
	(void)args;
	ft_memcpy(dest, payload, SIZE_CALL_JMP - sizeof(int));
	ft_memcpy(dest + 1, &((t_btc_args*)args)->jump, sizeof(int));
}


// to call this payload : need to mov permutations address in rdi
void	inject_def_init_perm(t_packer *packer, uint8_t *dest, void *args)
{
	uint8_t payload[SIZE_DEF_INIT_PERM] = {
		0x55,						  //push   rbp
		0x48, 0x89, 0xe5,			  //mov    rbp,rsp
		0xb9, 0xff, 0x00, 0x00, 0x00, //mov    ecx,0xff
		//<_fill_permutations>:
		0x88, 0x0c, 0x0f, //mov    BYTE PTR [rdi+rcx*1],cl
		0xe2, 0xfb,		  //loop   9 <_fill_permutations>
		0x88, 0x0f,		  //mov    BYTE PTR [rdi],cl
		0xc9,			  //leave
		0xc3			  //ret
	};

	(void)packer;
	(void)args;
	ft_memcpy(dest, payload, SIZE_DEF_INIT_PERM);
}


// to call this payload : need to mov permutations address in rdi and key address in rsi
void	inject_def_key_sched(t_packer *packer, uint8_t *dest, void *args)
{
	uint8_t payload[SIZE_DEF_KEY_SCHED] = {
		0x5,			  //push   rbp
		0x48, 0x89, 0xe5, //mov    rbp,rsp
		0x48, 0x31, 0xc9, //xor    rcx,rcx
		0x48, 0x31, 0xd2, //xor    rdx,rdx
		//<_do_permutations>:
		0x48, 0x31, 0xc0,						  //xor    rax,rax
		0x88, 0xc8,								  //mov    al,cl
		0x24, 0x07,								  //and    al,0x7
		0x8a, 0x04, 0x06,						  //mov    al,BYTE PTR [rsi+rax*1]
		0x00, 0xc2,								  //add    dl,al
		0x8a, 0x04, 0x0f,						  //mov    al,BYTE PTR [rdi+rcx*1]
		0x00, 0xc2,								  //add    dl,al
		0x8a, 0x04, 0x0f,						  //mov    al,BYTE PTR [rdi+rcx*1]
		0x8a, 0x24, 0x17,						  //mov    ah,BYTE PTR [rdi+rdx*1]
		0x88, 0x24, 0x0f,						  //mov    BYTE PTR [rdi+rcx*1],ah
		0x88, 0x04, 0x17,						  //mov    BYTE PTR [rdi+rdx*1],al
		0x48, 0xff, 0xc1,						  //inc    rcx
		0x48, 0x81, 0xf9, 0xff, 0x00, 0x00, 0x00, //cmp    rcx,0xff
		0x7e, 0xd7,								  //jle    1c <_do_permutations>
		0xc9,									  //leave
		0xc3									  //ret
	};

	(void)packer;
	(void)args;
	ft_memcpy(dest, payload, SIZE_DEF_KEY_SCHED);
}

// to call this payload: need to mov zone addr in rdi, len in rsi and permutations addr in rdx
void	inject_def_cypher(t_packer *packer, uint8_t *dest, void *args)
{
	uint8_t payload[SIZE_DEF_CYPHER] = {
		0x55,			  		//push   rbp
		0x48, 0x89, 0xe5, 		//mov    rbp,rsp
		0x53,			  		//push   rbx
		0x4d, 0x31, 0xc0, 		//xor    r8,r8
		0x48, 0x31, 0xdb, 		//xor    rbx,rbx
		0x48, 0x31, 0xc9, 		//xor    rcx,rcx
		0x48, 0x31, 0xc0, 		//xor    rax,rax
		// 0000000000000058 <_do_cypher>:
		0xfe, 0xc3,				//inc    bl
		0x02, 0x0c, 0x1a,		//add    cl,BYTE PTR [rdx+rbx*1]
		0x8a, 0x04, 0x1a,		//mov    al,BYTE PTR [rdx+rbx*1]
		0x8a, 0x24, 0x0a,		//mov    ah,BYTE PTR [rdx+rcx*1]
		0x88, 0x24, 0x1a,		//mov    BYTE PTR [rdx+rbx*1],ah
		0x88, 0x04, 0x0a,		//mov    BYTE PTR [rdx+rcx*1],al
		0x48, 0x31, 0xc0,		//xor    rax,rax
		0x8a, 0x04, 0x1a,		//mov    al,BYTE PTR [rdx+rbx*1]
		0x02, 0x04, 0x0a,		//add    al,BYTE PTR [rdx+rcx*1]
		0x8a, 0x04, 0x02,		//mov    al,BYTE PTR [rdx+rax*1]
		0x4a, 0x31, 0x04, 0x07, //xor    QWORD PTR [rdi+r8*1],rax
		0x49, 0xff, 0xc0,		//inc    r8
		0x49, 0x39, 0xf0,		//cmp    r8,rsi
		0x75, 0xd7,				//jne    58 <_do_cypher>
		0x5b,					//pop    rbx
		0xc9,					//leave
		0xc3					//ret
	};

	(void)packer;
	(void)args;
	ft_memcpy(dest, payload, SIZE_DEF_CYPHER);
}
