#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 printf("Number of command line arguments are: %i\n",argc);
 for (i=0;i<argc;i++){
  printf("%i. argument is %s and the result is:%i\n",i,argv[i], (atoi(argv[i]) + 1));
 }
 return 0;
}