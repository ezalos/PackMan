/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_solver.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 11:11:34 by ezalos            #+#    #+#             */
/*   Updated: 2021/03/25 02:24:50 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"


void		print_btc_name(t_btc *inst)
{
	if (inst->type == BTC_CALL_JMP)
		printf("BTC_CALL_JMP\n");
	else if (inst->type == BTC_CALL_MPROTECT)
		printf("BTC_CALL_MPROTECT\n");
	else if (inst->type == BTC_CALL_CYPHER)
		printf("BTC_CALL_CYPHER\n");
	else if (inst->type == BTC_DEF_WRITE)
		printf("BTC_DEF_WRITE\n");
	else if (inst->type == BTC_DEF_CYPHER)
		printf("BTC_DEF_CYPHER\n");
	else if (inst->type == BTC_DEF_INIT_PERM)
		printf("BTC_DEF_INIT_PERM\n");
	else if (inst->type == BTC_DEF_KEY_SCHED)
		printf("BTC_DEF_KEY_SCHED\n");
	else if (inst->type == BTC_DEF_BEGIN)
		printf("BTC_DEF_BEGIN\n");
	else if (inst->type == BTC_DEF_CYPHER_PREPARE)
		printf("BTC_DEF_CYPHER_PREPARE\n");
	else if (inst->type == BTC_DEF_END)
		printf("BTC_DEF_END\n");
	else
		dprintf(2, "Error: Unknown btc type\n");
}

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
	printf("%s: %zu <= %zu ?\n", __func__, inst->size, zone->size);

	return (inst->size <= zone->size);
}

void		write_btc(t_btc *inst, t_zone *zone, t_packer *packer)
{
	//TODO : todo
	uint8_t		*dest;

	dest = zone->offset + packer->content;
	printf("%s\n", __func__);
	print_btc_name(inst);
	printf("Writing at offset: %zu or known under the cute name 0x%lx\n", zone->offset, zone->offset);
	printf("Size is: %zu\n", inst->size);
	printf("It's in phdr nb %d\n", get_program_header_index(packer, zone->phdr));
	inst->func_ptr(packer, dest, inst->args);
	zone->phdr->p_filesz += inst->size;
	return;
}


ssize_t		bytecode_inject(t_packer *packer, t_list *zones, t_zone *zone, t_dlist *inst)
{
	uint8_t	headless;
	ssize_t ret;

	// if (inst == NULL)
	// {
	// 	printf("Inst is NULL -> WE FOUND THE SOLUTION!!\n");
	// 	return (((Elf64_Ehdr *)packer->content)->e_entry);
	// }
	printf("%s: inst nb %d\n", __func__, ((t_btc *)inst->data)->type);
	print_btc_name(inst->data);
	if (((t_btc*)inst->data)->type == BTC_DEF_CYPHER)
	{
		update_arg_crypt_calls(inst, zone);
	}
	update_zone(zone, inst->data);
	//TODO Headless resolution
	headless = ((t_btc *)inst->data)->type == BTC_CALL_JMP;
	ret = solve_bytecodes(packer, zones, zone, inst->next, headless);
	undo_update_zone(zone, ((t_btc *)inst->data));
	if (ret != FAILURE)
	{
		update_args(((t_btc *)inst->data), zone, ret);
		write_btc(inst->data, zone, packer);
		// if (!inst->prev)
		// 	ret = zone->offset + zone->vaddr;
		// else
		ret = zone->offset;
	}
	return (ret);
}

ssize_t		solve_bytecodes(t_packer *packer, t_list *zones, t_zone *current_zone, t_dlist *inst, int headless)
{
	t_list	*zone_list = zones;
	t_zone	*zone;
	ssize_t ret;
	t_dlist	*jmp;

	(void)current_zone;
	if (inst == NULL)
	{
		printf("Inst is NULL -> WE FOUND THE SOLUTION!!\n");
		return (((Elf64_Ehdr *)packer->content)->e_entry);
	}
		// return (current_zone->offset);
	printf("%s: inst nb %d\n", __func__, ((t_btc *)inst->data)->type);
	print_btc_name(inst->data);
	if (headless == TRUE)
	{
		printf("Headless TRUE\n");
		while (zone_list != NULL)
		{
			printf("Zone list\n");
			zone = zone_list->data;
			if (can_i_write(zone, inst->data))
			{
				printf("Zone selected in while\n");
				ret = bytecode_inject(packer, zones, zone, inst);
			}
			if (ret != FAILURE)
				return (ret);
			zone_list = zone_list->next;
		}
		return (FAILURE);
	}
	else
	{
		printf("Headless FALSE\n");
		zone = zone_list->data;
		if (can_i_write(zone, inst->data))
		{
			printf("We can coontinue to write in same zone!!!\n");
			return (bytecode_inject(packer, zones, zone, inst));
		}
		else
		{
			ret = FAILURE;
			jmp = ft_dlist_new(create_btc(BTC_CALL_JMP));
			//TODO Change func for dlist_insert_next()
			ft_dlist_insert_next_wesh(inst, jmp);
			// jmp->next = inst->next;
			if (can_i_write(zone, jmp->data))
			{
				printf("Jump resolution launched\n");
				ret = bytecode_inject(packer, zones, zone, jmp);
			}
			//TODO Change 1st arg
			jmp = ft_dlist_cut(&jmp, jmp);
			free_btc(jmp->data);
			ft_dlist_free(jmp, NULL);
			return (ret);
		}
	}
}