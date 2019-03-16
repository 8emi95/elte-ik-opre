#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Paraméter: szó, adjuk vissza a vezetõ szóközök nélkül 
char* NoSpaces(char* str){
	while(*str==' ')str++;
	return str;
}

int main(int argc,char ** argv) 
{
 printf("Argument without spaces: \'%s\'\n",NoSpaces(argv[1])); 
 return 0;
}