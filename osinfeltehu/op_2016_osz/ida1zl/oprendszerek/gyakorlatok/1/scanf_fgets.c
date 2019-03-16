#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//call it with some parameters from the command line 

int main() 
{
	char c1[10];
	char c2[10];
	fgets(c1, 10, stdin);	//beolvassa az entert is, 1-el hosszabb lesz 
	scanf("%s", c2);	//nem olvassa be az entert
	//fgets(c1, 10, stdin);
	
	printf("%s%i\n", c1, strlen(c1));
	printf("%s%i\n", c2, strlen(c2));
	return 0;
}