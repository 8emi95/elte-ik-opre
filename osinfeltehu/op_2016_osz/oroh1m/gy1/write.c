#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 printf("Number of command line arguments are: %i\n",argc);
 for (i=0;i<atoi(argv[1]);i++){
  printf("%i. argument is %s\n",i,argv[2]);
 }
 return 0;
}