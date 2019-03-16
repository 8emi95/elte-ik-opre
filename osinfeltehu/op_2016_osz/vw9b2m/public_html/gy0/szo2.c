#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * length(char * str)
{

 while (*str++==32) str+=1;
 return str;
}

int main(int argc,char ** argv) 

{
	printf("banana: .%s".length(argv[1]));
	return 0;
 //passwd régi jelszo ketszer uj
}