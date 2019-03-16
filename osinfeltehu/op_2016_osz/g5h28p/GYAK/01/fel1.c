#include <stdio.h>
#include <stdlib.h>

//1. Definialjunk egy stringet, majd irjuk ki a hosszat!
int length(char * str)
{
	int l=0;
	while (*str++!=0)l++; //*str points to the next character
	return l;
}

int main(int argc,char ** argv) 
{
	char string[]="Hello world";
	printf("The length of variable str1 \'%s\'\n  %i (with length()), %i (with strlen())\n",string,length(string),strlen(string));

	return 0;
}