#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 char string[30];
 char string2[30];
 
 //fgets - entert is olvas, scanf 
    
	printf("Szoveg: ");
	scanf("%s",string);
	
	printf("Szoveg: ");
	fgets(string2,30,stdin);
 
	printf("%s \n", string);
	printf("%s \n", string2); 

	return 0;
}