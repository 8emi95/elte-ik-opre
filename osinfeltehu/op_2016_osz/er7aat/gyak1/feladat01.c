#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char str1[] = "Hello world";
char str2[] = "Hello";

char * delete_space(char * str)
{
	while(*str == ' ')
	{
		str++;
	}
	return str;
}

struct adat {
     char nev[25];
     int jegy;

}

omt main(int argc, char ** argv)
{
   if(argc != 3)
   {
	
