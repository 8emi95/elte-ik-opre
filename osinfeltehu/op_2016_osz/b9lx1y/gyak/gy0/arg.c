#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 printf("Number of command line numbers: %i\n", argc - 1);
 for (i=1;i<argc;i++){
  int num = atoi(argv[i]);
  printf("%i. argument + 1 is %i\n",i, num + 1);
 }
 return 0;
}
