/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytecode_blueprint.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 20:12:38 by ezalos            #+#    #+#             */
/*   Updated: 2021/05/07 12:18:26 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

t_btc	*create_btc(int type)
{
	t_btc *btc;
	if (!(btc = malloc(sizeof(t_btc))))
		return (NULL);
	ft_memset(btc, 0, sizeof(t_btc));
	ft_memcpy(btc, &bytecode_lib[type], sizeof(t_btc));
	if (!(btc->args = malloc(sizeof(t_btc_args))))
	{
		free(btc);
		return (NULL);
	}
	ft_memset(btc->args, 0, sizeof(t_btc_args));
	return (btc);
}

t_dlist		*blueprint_add(t_dlist **blueprint, int btc_type)
{
	t_dlist *inst;
	t_btc	*btc;

	if (NULL == (btc = create_btc(btc_type)))
		return (NULL);
	if (NULL == (inst = ft_dlist_new(btc)))
		return (NULL);
	if (NULL == *blueprint)
		(*blueprint) = inst;
	else
		ft_dlist_append_end(blueprint, inst);
	logging_recursive("*** %s: Adding to blueprint %s\n", __func__, btc_to_str(btc));
	return (inst);
}

void		set_call_cypher_args(t_btc *inst, t_zone *zone)
{
	inst->args->crypt_plaintext_vaddr =  zone->vaddr;
	inst->args->crypt_plaintext_size = zone->size;
}

t_dlist		*blueprint_creation(t_packer *packer)
{
	t_dlist *blueprint;
	t_dlist	*inst;
	t_list	*to_crypt;

	to_crypt = packer->to_crypt;
	blueprint = NULL;
	if (NULL == blueprint_add(&blueprint, BTC_DEF_FIND_ABS_VADDR))
		exit(FAILURE == print_error(packer->self_path, MALLOC_ERROR) ? EXIT_FAILURE : 0);
	if (NULL == blueprint_add(&blueprint, BTC_DEF_INIT_PERM))
		exit(FAILURE == print_error(packer->self_path, MALLOC_ERROR) ? EXIT_FAILURE : 0);
	if (NULL == blueprint_add(&blueprint, BTC_DEF_KEY_SCHED))
		exit(FAILURE == print_error(packer->self_path, MALLOC_ERROR) ? EXIT_FAILURE : 0);
	if (NULL == blueprint_add(&blueprint, BTC_DEF_CYPHER))
		exit(FAILURE == print_error(packer->self_path, MALLOC_ERROR) ? EXIT_FAILURE : 0);
	if (NULL == blueprint_add(&blueprint, BTC_DEF_BEGIN))
		exit(FAILURE == print_error(packer->self_path, MALLOC_ERROR) ? EXIT_FAILURE : 0);
	if (NULL == blueprint_add(&blueprint, BTC_DEF_CYPHER_PREPARE))
		exit(FAILURE == print_error(packer->self_path, MALLOC_ERROR) ? EXIT_FAILURE : 0);
	while (to_crypt != NULL)
	{
		if (NULL == (inst = blueprint_add(&blueprint, BTC_CALL_CYPHER)))
			exit(FAILURE == print_error(packer->self_path, MALLOC_ERROR) ? EXIT_FAILURE : 0);
		set_call_cypher_args(((t_btc *)inst->data), (t_zone *)to_crypt->data);
		to_crypt = to_crypt->next;
		to_crypt = NULL;
	}
	if (NULL == blueprint_add(&blueprint, BTC_DEF_WRITE))
		exit(FAILURE == print_error(packer->self_path, MALLOC_ERROR) ? EXIT_FAILURE : 0);
	if (NULL == blueprint_add(&blueprint, BTC_DEF_END))
		exit(FAILURE == print_error(packer->self_path, MALLOC_ERROR) ? EXIT_FAILURE : 0);
	if (NULL == blueprint_add(&blueprint, BTC_CALL_JMP))
		exit(FAILURE == print_error(packer->self_path, MALLOC_ERROR) ? EXIT_FAILURE : 0);
	return (blueprint);
}

size_t		get_blueprint_inject_size(t_dlist *blueprint)
{
	size_t	size;

	size = 0;
	while (blueprint)
	{
		size += ((t_btc *)blueprint->data)->size;
		blueprint = blueprint->next;
	}
	return (size);
}
