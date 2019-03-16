#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int length(char* str) 
{
	return strlen(str);
}


int main()
{

	char* tmp = "almafa1";
	int hossz = length(tmp);
	printf("length of %s is: %i /n", tmp, hossz);
	
	return 0;
}

