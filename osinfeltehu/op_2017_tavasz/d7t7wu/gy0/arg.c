#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 
 printf("Number of command line arguments are: %i\n",argc);
 printf("%i. argument is %s\n", i, argv[i]);
 for (i=1;i<argc;i++){
  printf("%i. argument is %i\n",i,2*(atoi(argv[i])));
 }
 return 0;

}