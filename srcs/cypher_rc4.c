/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rc4.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 17:04:49 by rkirszba          #+#    #+#             */
/*   Updated: 2021/03/25 19:50:55 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void	init_permutations(uint8_t *permutations)
{
	uint8_t	i;

	i = 0;
	while (TRUE)
	{
		permutations[i] = i;
		if (i == 0xFF)
			break ;
		i++;
	}
}

void	init_key(uint8_t *key)
{
	size_t	i;

	srand(clock());
	i = -1;
	while (++i < KEY_SIZE)
		key[i] = rand();
}

void	schedule_key(uint8_t *permutations, uint8_t *key)
{
	uint8_t	i;
	uint8_t j;
	uint8_t	tmp;

	i = 0;
	j = 0;
	while (TRUE)
	{
		j = (j + permutations[i] + key[i & (KEY_SIZE - 1)]); // mettre modulo PERM_SIZE si on change le type
		tmp = permutations[i];
		permutations[i] = permutations[j];
		permutations[j] = tmp;
		if (i == 0xFF)
			break ;
		i++;
	}
}

void	crypt_zone(uint8_t *zone, size_t len, uint8_t *permutations)
{
	size_t	index;
	uint8_t	i;
	uint8_t	j;
	uint8_t	k;
	uint8_t	tmp;

	index = -1;
	i = 0;
	j = 0;
	while (++index < len)
	{
		i = (i + 1); // mettre modulo PERM_SIZE si on change le type
		j = j + permutations[i]; // mettre modulo PERM_SIZE si on change le type
		tmp = permutations[i];
		permutations[i] = permutations[j];
		permutations[j] = tmp;
		k = permutations[(uint8_t)(permutations[i] + permutations[j]) /*mettre modulo PERM_SIZE si on change le type*/]; // mettre modulo 0x100 si on change le type
		zone[index] ^= k;
	}
}

void	crypt_zones(t_packer *packer)
{
	uint8_t 	permutations[PERM_SIZE];
	uint8_t		*content;
	t_list		*to_crypt;
	t_zone		*zone;

	init_permutations(permutations);//asm
	init_key(packer->key);
	schedule_key(permutations, packer->key);//asm
	content = (uint8_t*)(packer->content);
	to_crypt = packer->to_crypt;
	while (to_crypt)
	{
		zone = (t_zone*)(to_crypt->data);
		crypt_zone(content + zone->offset, zone->size, permutations);//asm
		to_crypt = to_crypt->next;
	}
}