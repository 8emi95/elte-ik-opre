#include <stdio.h>
#include <stdlib.h>

int main(int argc,char ** argv) 
{
 printf("Number of command line arguments are: %i\n",argc);
 int i;
 for (i=1;i<argc;i++){
  int value = atoi(argv[i])+1;
  printf("Increased value of %i. argument is %i\n",i,value);
 }
 return 0;
}