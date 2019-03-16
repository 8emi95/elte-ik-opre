#include <stdio.h>

int length(char * str)
{
	int l =0;
	while(*str++ != 0) l++;
	return l;
}

int main()
{
	char* str1 = "Hello";
	char* str2 = "World";
	char* str3 = "Hello";

	int i = 0;
	while(i<length(str1) && i<length(str3) && str1[i]==str2[i])
	{
		i++;
	}
	printf("%d\n",i);
	if(i==length(str1))
	{
		 printf("%s es %s egyelno\n",str1,str2);
	}

	return 0;
}
