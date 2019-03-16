#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* func(const char *str)
{
	if (str == NULL)
		return NULL;
	while (*str == ' ')
		str++;
	return str;
}

int main()
{
	char str[] = "  Hello world!";
	printf("%s\n", str);
	printf("%s\n", func(str));
	return 0;
}
