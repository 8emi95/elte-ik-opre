#include <stdio.h>
#include <stdlib.h>

char* despace(char* str)
{
	while(*str==' ')str++;
	return str;
}


int main(int argc, char** argv)
{
	int i;
	for(i=1; i<argc; i++)
	{
		printf("%i. word: %s\n",i,despace(argv[i]));
	}
	
		char* str = argv[1];
		printf("%s\n",str);
	for(i=0;i<3; i++)
	{
		(*str)++;
		printf("%s\n",str);
	}
	return 0;
}