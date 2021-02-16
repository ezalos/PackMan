#include <stdio.h>

int a = 8;
int b;

int		main(void)
{
	const char	str[12] = "Hello world\n";
	static int	g;
	static int	p = 5;

	printf(str);
	return (0);
}