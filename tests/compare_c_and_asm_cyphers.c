#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

extern void	init_permutations_asm(uint8_t *permutations);
extern void	schedule_key_asm(uint8_t *permutations, uint8_t *key);
extern void crypt_zone_asm(uint8_t *zone, size_t len, uint8_t *permutations);

#define KEY_SIZE	8
#define	FALSE		0
#define TRUE		1
#define SEQ_LEN		0x200

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

	srand(time(NULL));
	i = -1;
	while (++i < KEY_SIZE)
		key[i] = rand(); // & 0x100;  //voir si modulo est obligatoire
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

void	print_tab(uint8_t *tab)
{
	int		i;

	i =-1;
	while (++i < 256)
		printf("%hhu\n", tab[i]);

}


void	generate_random_bytes(uint8_t *s, size_t len)
{
	size_t	i;

	i = -1;
	while (++i < len)
		s[i] = rand() % 256;
}

void	print_bytes(uint8_t *s, size_t len)
{
	size_t	i;

	i = -1;
	while (++i < len)
		printf("%02hhx", s[i]);
	printf("\n");
}


int		main(void)
{
	uint8_t tab1[256] = {18};
	uint8_t	tab2[256] = {18};
	uint8_t	*s1;
	uint8_t	*s2;
	uint8_t key[KEY_SIZE];

	init_key(key);

	init_permutations_asm(tab1);
	init_permutations(tab2);

	schedule_key_asm(tab1, key);
	schedule_key(tab2, key);

	if (!memcmp(tab1, tab2, 256))
		printf("Tabs are the same :)\n");
	else
		printf("Tabs are different :(\n");
	
	s1 = malloc(SEQ_LEN);
	s2 = malloc(SEQ_LEN);

	generate_random_bytes(s1, SEQ_LEN);
	memcpy(s2, s1, SEQ_LEN);

	print_bytes(s1, SEQ_LEN);
	printf("\n");
	print_bytes(s2, SEQ_LEN);

	printf("\n");
	printf("\n");

	crypt_zone_asm(s1, SEQ_LEN, tab1);
	crypt_zone(s2, SEQ_LEN, tab2);

	print_bytes(s1, SEQ_LEN);
	printf("\n");
	print_bytes(s2, SEQ_LEN);

	printf("\n");
	printf("\n");

	if (!memcmp(tab1, tab2, 256))
		printf("Crypts are the same :)\n");
	else
		printf("Crypts are different :(\n");

	return (0);
}
