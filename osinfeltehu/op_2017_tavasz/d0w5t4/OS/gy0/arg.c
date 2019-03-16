#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 printf("Number of command line arguments are: %i\n",argc);
 printf("1. argument is %s\n", argv[i]);
 for (i=1;i<argc;i++){
  printf("%i. argument is %i\n",i, atoi(argv[i]) * 2);
 }
 return 0;
}