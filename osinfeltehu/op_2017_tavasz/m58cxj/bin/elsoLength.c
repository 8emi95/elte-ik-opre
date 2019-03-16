#include <stdio.h>


int length2(const char *c)
{
	int i = 0;
	while (c+i != '\0')
	{
		++i;
	}
	return i;
}

int main()
{
	const char *str = "Hello!";
	int size = length2(str);
	printf("%i\n",size);
	return 0;
} 	