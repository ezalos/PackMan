
#include "head.h"

void		unit_test_cypher(char *str, int len, char key)
{
	char	*dup;
	int		i = -1;

	dup = malloc(len);
	memmove(dup, str, len);

	printf("Crypt:   %d %02hhx\n", len, key);
	ft_cypher(dup, len, key);
	while (++i < len)
	{
		if ((char)dup[i] != (char)(str[i] ^ key))
			printf("Error %d: %02hhx insted of %02hhx\n", i, (char)dup[i], (char)(str[i] ^ key));
	}
	printf("DeCrypt: %d %02hhx\n", len, key);
	ft_cypher(dup, len, key);
	i = -1;
	while (++i < len)
	{
		if ((char)dup[i] != (char)(str[i]))
			printf("Error %d: %02hhx insted of %02hhx\n", i, (char)dup[i], (char)(str[i]));
	}

	free(dup);
}

void		test_cypher(void)
{
	unit_test_cypher("Hello World!\n", 13, 0b10101010);
	unit_test_cypher("\0\0\0\0\0\0", 6, 0b10101010);
	unit_test_cypher("", 0, 0b10101010);
}




void	unit_test_alter(uint8_t *content, size_t len)
{
	uint8_t		*dup;
	uint8_t 	permutations[PERM_SIZE];
	uint8_t		key[KEY_SIZE];
	size_t		i;

	dup = malloc(len);
	memcpy(dup, content, len);
	
	init_key(key);
	i = -1;
	printf("Key: ");
	while (++i < KEY_SIZE)
		printf("%02hhx", key[i]);
	printf("\n");
	
	init_permutations(permutations);
	schedule_key(permutations, key);

	crypt_zone(dup, len, permutations);
	printf("  Crypted: ");
	i = -1;
	while (++i < len)
		printf("%02hhx", dup[i]);
	printf("\n");

	init_permutations(permutations);
	schedule_key(permutations, key);

	crypt_zone(dup, len, permutations);
	printf("Decrypted: ");
	i = -1;
	while (++i < len)
		printf("%02hhx", dup[i]);
	printf("\n");

	if (memcmp((char*)content, (char*)dup, len))
		printf("Crypting didn't work\n");
	else
		printf("Crypting worked fine\n");
}

void	test_cypher_alter(void)
{
	unit_test_alter((uint8_t*)"Hello World!\n", 13);
	unit_test_alter((uint8_t*)"\0\0\0\0\0\0", 6);
	unit_test_alter((uint8_t*)"", 0);
}