#include <stdio.h>

int equal(char *str1, char *str2);

int main()
{
	char s1[] = "Hajra Fradi!";
	char s2[] = "Hajra Vasas!";
	char s3[] = "Hajra!";
	char s4[] = "Hajra Fradi!";
}

int equal(char *str1,char *str2)
{
	while(*str1 || *str2)
	{
		if(*str1 != *str2)
			return 0;
		str1++;
		str2++;
	}
	return 1;
}
