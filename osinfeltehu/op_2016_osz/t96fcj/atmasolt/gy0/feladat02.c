#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Param�ter: sz�, adjuk vissza a vezet� sz�k�z�k n�lk�l 
char* NoSpaces(char* str){
	while(*str!= '\0')
	{
			if(*str ==' ')
				printf("tal�ltam sz�k�zt");; 

	}
	return str;
}

int main(int argc,char ** argv) 
{
 printf("Argument without spaces: \'%s\'\n",NoSpaces(argv[1])); 
 return 0;
}