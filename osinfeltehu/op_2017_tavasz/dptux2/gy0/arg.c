#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 //printf("Number of command line arguments are: %i\n",argc);
 for (i=1;i<argc;i++){
  printf("%i\n",atoi(argv[i])*2);
 // printf("%i. argument is %s\n",i,argv[i]);
 }
 return 0;
}
