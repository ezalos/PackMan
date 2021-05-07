/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_injecters.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 19:29:02 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/07 13:10:45 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	inject_def_begin(uint8_t *dest, void *args)
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

	(void)args;
	ft_memcpy(dest, payload, SIZE_DEF_BEGIN);
}

void	inject_call_mprotect(uint8_t *dest, void *args)
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

	(void)args;
	ft_memcpy(dest, payload, SIZE_CALL_MPROTECT);
}

void	inject_def_cypher_prepare(uint8_t *dest, void *args)
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
		0x48, 0x8d, 0xb4, 0x24, 0x00, 0x01, 0x00,  	//lea    rsi,[rsp+0x100]
		0x00, 
		0xe8, 0xfc, 0xff, 0xff, 0xff          		//call   33 <btc_def_cypher_prepare+0x33>
	};
	uint8_t	*jmp_init_perm;
	uint8_t	*jmp_key_sched;

	// logging_recursive("struct SCHEDULE: %x \n", ((t_btc_args *)args)->jmp_key_sched);
	// logging_recursive("struct INIT PERM: %x \n", ((t_btc_args *)args)->jmp_init_perm);
	jmp_init_perm = (uint8_t *)(&((t_btc_args *)args)->jmp_init_perm);
	// logging_recursive("jmp_init_perm: %x\n", *(uint32_t*)jmp_init_perm);
	jmp_key_sched = (uint8_t *)&((t_btc_args *)args)->jmp_key_sched;
	// logging_recursive("jmp_key_sched: %x\n", *(uint32_t*)jmp_key_sched);

	// logging_recursive("%s: Begin memcopy\n", __func__);
	// logging("KEY IS: %llx\n", *(uint64_t*)((t_btc_args *)args)->crypt_key);

	ft_memcpy(payload + 0xe, ((t_btc_args *)args)->crypt_key, 0x4);
	ft_memcpy(payload + 0x19, ((t_btc_args *)args)->crypt_key + 0x4, 0x4);
	ft_memcpy(payload + OFFSET_CALL_INIT_PERM, jmp_init_perm, sizeof(uint32_t));
	ft_memcpy(payload + OFFSET_CALL_KEY_SCHED, jmp_key_sched, sizeof(uint32_t));
	ft_memcpy(dest, payload, SIZE_DEF_CYPHER_PREPARE);
}


void	inject_call_cypher(uint8_t *dest, void *args)
{
	uint8_t payload[SIZE_CALL_CYPHER] = {
		0x48, 0x83, 0xec, 0x10,             		//sub    rsp,0x10
		0xc7, 0x04, 0x24, 0xff, 0xff, 0xff, 0xff,   //mov    DWORD PTR [rsp],0xffffffff
		0xc7, 0x44, 0x24, 0x04, 0xff, 0xff, 0xff,   //mov    DWORD PTR [rsp+0x4],0xffffffff
		0xff, 
		0x48, 0x8b, 0x3c, 0x24,             		//mov    rdi,QWORD PTR [rsp]
		0xe8, 0xfc, 0xff, 0xff, 0xff,          		//call   18 <btc_call_cypher+0x18>
		0x48, 0x89, 0xc7,                			//mov    rdi,rax
		0xc7, 0x44, 0x24, 0x08, 0xff, 0xff, 0xff,   //mov    DWORD PTR [rsp+0x8],0xffffffff
		0xff, 
		0xc7, 0x44, 0x24, 0x0c, 0xff, 0xff, 0xff,   //mov    DWORD PTR [rsp+0xc],0xffffffff
		0xff, 
		0x48, 0x8b, 0x74, 0x24, 0x08,          		//mov    rsi,QWORD PTR [rsp+0x8]
		0x48, 0x8d, 0x54, 0x24, 0x10,          		//lea    rdx,[rsp+0x10]
		0xe8, 0xfc, 0xff, 0xff, 0xff,          		//call   32 <btc_call_cypher+0x32>
		0x48, 0x83, 0xc4, 0x10             			//add    rsp,0x10
	};
	uint8_t *addr;
	uint8_t *len;
	uint8_t	*jmp_def_cypher;
	uint8_t	*jmp_find_abs_addr;

	// a priori les informations d'adresse et len seront dans args
	// pour l'adresse (il faut qu'elle soit absolue, moyen de le savoir avant l'execution du woody ?)
	addr = (uint8_t*)(&(((t_btc_args*)args)->crypt_plaintext_vaddr));
	len = (uint8_t *)(&(((t_btc_args *)args)->crypt_plaintext_size));
	jmp_def_cypher = (uint8_t *)(&(((t_btc_args *)args)->jmp_def_cypher));
	jmp_find_abs_addr = (uint8_t *)(&(((t_btc_args *)args)->jmp_find_abs_addr));

	ft_memcpy(payload + 0x7, addr, 0x4); //adresse de la zone a decrypter 1st part
	ft_memcpy(payload + 0xf, addr + 0x4, 0x4); //adresse de la zone a decrypter 2nd part
	ft_memcpy(payload + 0x23, len, 0x4); //longueur a decrypter (8octets) 1/2
	ft_memcpy(payload + 0x2b, len + 0x4, 0x4); // 2/2
	ft_memcpy(payload + OFFSET_CALL_FIND_ABS_ADDR, jmp_find_abs_addr, sizeof(uint32_t));
	ft_memcpy(payload + OFFSET_CALL_CYPHER, jmp_def_cypher, sizeof(uint32_t));
	ft_memcpy(dest, payload, SIZE_CALL_CYPHER);
}

void	inject_def_write(uint8_t* dest, void *args)
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

	(void)args;
	// revoir en faisant des xor rdi, rdi   xor rdx, rdx    xor rax, rax
	ft_memcpy(dest, payload, SIZE_DEF_WRITE);
}

void	inject_def_end(uint8_t *dest, void *args)
{
	uint8_t payload[SIZE_DEF_END] = {
		0x48, 0x81, 0xc4, 0x08, 0x01, 0x00, 0x00,	//add    rsp,0x108
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

	(void)args;
	ft_memcpy(dest, payload, SIZE_DEF_END);
}

// UNSAFE DOES NOT CHECK FOR WRITEABILITY OR SIZE
void	inject_call_jmp(uint8_t* dest, void *args)
{
	uint8_t payload[SIZE_CALL_JMP] = {
		0xe9, 0xfc, 0xff, 0xff, 0xff	//jmp    1 <call_jmp+0x1>
	};
	ft_memcpy(payload + 1, &((t_btc_args *)args)->jump, sizeof(int));
	ft_memcpy(dest, payload, SIZE_CALL_JMP);
}

// to call this payload : need to mov permutations address in rdi
void	inject_def_init_perm(uint8_t *dest, void *args)
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

	(void)args;
	ft_memcpy(dest, payload, SIZE_DEF_INIT_PERM);
}


// to call this payload : need to mov permutations address in rdi and key address in rsi
void	inject_def_key_sched(uint8_t *dest, void *args)
{
	uint8_t payload[SIZE_DEF_KEY_SCHED] = {
		0x55,			  //push   rbp
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

	(void)args;
	ft_memcpy(dest, payload, SIZE_DEF_KEY_SCHED);
}

// to call this payload: need to mov zone addr in rdi, len in rsi and permutations addr in rdx
void	inject_def_cypher(uint8_t *dest, void *args)
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

	(void)args;
	ft_memcpy(dest, payload, SIZE_DEF_CYPHER);
}

void	inject_def_find_abs_vaddr(uint8_t *dest, void *args)
{
	uint8_t payload[SIZE_DEF_FIND_ABS_VADDR] = {
		0x55,                      					//push   rbp
		0x48, 0x89, 0xe5,                			//mov    rbp,rsp
		0x48, 0x8d, 0x0d, 0xf5, 0xff, 0xff, 0xff,  	//lea    rcx,[rip+0xfffffffffffffff5]        # 0 <btc_def_find_abs_vaddr>
		0x48, 0x83, 0xec, 0x08,             		//sub    rsp,0x8
		0xc7, 0x04, 0x24, 0xdd, 0xcc, 0xbb, 0xaa,   // mov    DWORD PTR [rsp],0xaabbccdd
		0xc7, 0x44, 0x24, 0x04, 0xff, 0xee, 0xdd,   // mov    DWORD PTR [rsp+0x4],0xccddeeff
		0xcc, 
		0x48, 0x2b, 0x0c, 0x24,             		//sub    rcx,QWORD PTR [rsp]
		0x48, 0x89, 0xf8,                			//mov    rax,rdi
		0x48, 0x01, 0xc8,                			//add    rax,rcx
		0x48, 0x83, 0xc4, 0x08,             		//add    rsp,0x8
		0xc9,                      					//leave  
		0xc3,                     					//ret   
	};
	uint8_t *addr;

	addr = (uint8_t*)(&(((t_btc_args*)args)->crypt_func_find_abs_vaddr_vaddr));
	ft_memcpy(payload + 0x12, addr, 0x4); //adresse de la zone a decrypter 1st part
	ft_memcpy(payload + 0x1a, addr + 0x4, 0x4); //adresse de la zone a decrypter 2nd part
	ft_memcpy(dest, payload, SIZE_DEF_FIND_ABS_VADDR);
}
