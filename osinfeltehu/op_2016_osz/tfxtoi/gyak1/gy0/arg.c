#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 printf("Number of command line arguments are: %i\n",argc);
 for (i=1;i<argc;i++){
  int actualNumber = atoi(argv[i]);
  int incrementBy = 1;
  int sum = actualNumber + incrementBy;

  printf("%i. argument is %s. After incremention: %i\n",i,argv[i],sum);
 }
 return 0;
}
