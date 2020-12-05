
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
