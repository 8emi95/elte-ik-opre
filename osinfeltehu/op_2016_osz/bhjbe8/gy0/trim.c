#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* triml(char * str)
{
	while (*str++ == ' '){}
	return --str;
}

//HF ahol alma -> korte : malloc/realloc (nagyobb helyet kell foglalni)


int main(int argc,char ** argv) 
{
	printf("Szokozok nelkul: %s\n", triml(argv[1]));

	return 0;
}