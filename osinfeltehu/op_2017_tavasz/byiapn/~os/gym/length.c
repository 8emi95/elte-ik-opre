#include <stdio.h>
#include <stdlib.h>

int length(char *str)
{
	int c = 0;
	char* p = str;
	while(*p!=0)
	{
		c++;
		p++;
	}
	return c;
}

int main()
{
	char str[] = "ot";
	printf("%d\n", length(str));
}
