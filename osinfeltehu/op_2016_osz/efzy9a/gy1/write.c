#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv, char* argw, char* argg) 
//char** means an array of character arrays = array of strings
{
 int i;
 printf("Number of command line arguments are: %i\n",argc);
 
 for(i = 0; i < atoi(argw); ++i){
	 printf("%i %s", i, argv);	 
 }
 return 0;
}