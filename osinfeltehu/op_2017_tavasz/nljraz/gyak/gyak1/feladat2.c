#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define mallocate(type, count) ((type*)malloc(count * sizeof(type)))

int main(int argc, char **argv) 
{
	char str1[] = "Hello world!";
	char *str2 = mallocate(char, 80);
	strcpy(str2, "Hello world!");
	printf("A ket string %s\n", (strcmp(str1, str2) == 0 ? "megegyezik!" : "nem egyezik meg!"));
	return 0;
}
