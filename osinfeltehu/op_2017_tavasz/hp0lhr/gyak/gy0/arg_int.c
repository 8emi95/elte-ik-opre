#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 int akt;
 printf("Number of command line arguments are: %i\n",argc);
 for (i=0;i<argc;i++){
  akt = atoi(argv[i])*2;
  printf("%i. argument is %i\n",i,akt);
 }
 return 0;
}