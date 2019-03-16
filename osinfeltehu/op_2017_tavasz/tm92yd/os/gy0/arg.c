#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
//argv[0]: futtatás
//argv[1]..[argc-1]: paraméterek
//argv[argc]: NULL
{
 int i;
 printf("Number of command line arguments are: %i\n",argc);
 for (i=0;i<argc;i++){
  printf("%i. argument is %s\n",i,argv[i]);
 }
 return 0;
}