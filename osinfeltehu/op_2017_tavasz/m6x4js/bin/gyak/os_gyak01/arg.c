#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 printf("Number of command line arguments are: %i\n",argc);
 for (i=0;i<argc;i++){
	int x = atoi(argv[i]);
	x *= 2; 
 printf("%i. argument is %i\n",i,x);
 }
 return 0;
}
