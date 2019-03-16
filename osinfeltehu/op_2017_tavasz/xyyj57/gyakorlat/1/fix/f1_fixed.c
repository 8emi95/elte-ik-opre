#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int length(char* str) 
{
	int counter = 0;
	
	while(*str != 0)
	{
		counter = counter + 1;
		str = str+1;
	}
	
	return counter;
}


int main()
{

	char* tmp = "almafa1";
	int hossz = length(tmp);
	printf("length of %s is: %i", tmp, hossz);
	
	return 0;
}

