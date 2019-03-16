#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare(char* str1, char* str2)
{

	while(str1!=0 && str2!=0)
	{
		str1++;
		str2++;	
	}

	return 1;
}

int main()
{
	char str[] = "Hello World\0";
	printf("Helloooooo\n");
	compare("asd", "asd");

	return 0;
}
