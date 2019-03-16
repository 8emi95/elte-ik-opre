#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 int num=0;
 printf("Number of command line arguments are: %i\n",argc);
 for (i=1;i<argc;i++){
	 num = atoi(argv[i]);
  //printf("%i. argument is %s\n",i,argv[i]);
  printf("%i/n",num);
 }
 return 0;
}