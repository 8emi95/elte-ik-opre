#include "stdio.h"

int strcpy_(char* str1, char* str2)
{
	while (str1 && str2 && *str1 && *str2 && *str1++==*str2++);
	return str1 && str2 && !*str1 && !*str2;
}

int main()
{
	char str1[] = "fst";
	char* str2;
	printf("str1 %s str2\n", strcpy_(str1, str2) ? "=" : "!=");	
}
