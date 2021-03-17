/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_injecters.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 19:29:02 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/17 19:37:30 by ezalos           ###   ########.fr       */
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



t_btc *create_btc(int type)
{
	t_btc *btc;
	if ((btc = malloc(sizeof(t_btc))))
		return (NULL);
	ft_memcpy(btc, &bytecode_lib[type], sizeof(t_btc));
	if ((btc->args = malloc(sizeof(t_btc_args))))
	{
		free(btc);
		return (NULL);
	}
	return (btc);
}

void		free_btc(t_btc *btc)
{
	free(btc->args);
	free(btc);
}



// void		update_arg_crypt_calls(t_btc *inst, t_zone *write_zone)
// {
// 	(void)inst;
// 	(void)write_zone;
// 	return;
// }

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

void		update_arg_crypt_calls(t_btc *inst, t_zone *zone)
{
	while (inst)
	{
		if (inst->type == BTC_DECRYPT)
		{
			inst->args->crypt_func_addr = (void*)((size_t)zone->phdr->p_vaddr + (size_t)zone->offset);
		}
		inst = inst->next;
	}

}

void	update_args(ssize_t ret,t_btc *inst)
{
	(void)ret;
	(void)inst;
	return;
}


void	write_btc(t_btc *inst, t_zone *zone, t_packer *packer)
{
	inst++;
	zone++;
	packer++;
	return;
}

ssize_t		bytecode_inject(t_packer *packer, t_list *zones,t_zone *zone, t_btc *inst)
{
	ssize_t	ret;

	if (inst->type == BTC_DEF_CRYPT)
	{
		update_arg_crypt_calls(inst, zone);
	}
	update_zone(zone, inst);
	ret = solve_bytecodes(packer, zones, zone, inst->next, inst->type == BTC_JMP);
	if (ret != FAILURE)
	{
		update_args(ret, inst);
		write_btc(inst, zone, packer);
	}
	else
	{
		undo_update_zone(zone, inst);
	}
	return (ret);
}

ssize_t			solve_bytecodes(t_packer *packer, t_list *zones, t_zone *current_zone, t_btc *inst, int headless)
{
	t_list	*zone_list = zones;
	t_zone	*zone;
	ssize_t	ret;
	t_btc	*jmp;


	if (inst == NULL)
		return (current_zone->offset);

	if (headless)
	{
		while (zone_list != NULL)
		{
			zone = zone_list->data;
			if (can_i_write(zone, inst))
				ret = bytecode_inject(packer, zones, zone, inst);
			if (ret != FAILURE)
				return (ret);
			zone_list = zone_list->next;
		}
		return (FAILURE);
	}
	else
	{
		zone = zone_list->data;
		if (can_i_write(zone, inst))
			return (bytecode_inject(packer, zones, zone, inst));
		else
		{
			ret = FAILURE;
			jmp = create_btc(BTC_JMP);
			jmp->next = inst->next;
			if (can_i_write(zone, jmp))
				ret = bytecode_inject(packer, zones, zone, jmp);
			free_btc(jmp);
			return (ret);
		}
	}
}


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


void inject_write(t_packer *packer, uint8_t* dest)
{
    uint8_t payload[SIZE_WRITE] = {
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

    // DO THIS AT SOME POINT
    uint8_t endian_flipped_payload[SIZE_WRITE] = {
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

    if (!is_same_endian(packer))
    {
        ft_memcpy(dest, endian_flipped_payload, SIZE_WRITE);
    }
    else
    {
        ft_memcpy(dest, payload, SIZE_WRITE);
    }
}