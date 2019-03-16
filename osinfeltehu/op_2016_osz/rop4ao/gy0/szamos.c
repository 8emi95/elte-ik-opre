#include <stdio.h>
#include <stdlib.h>
int main (int argc, char ** argv)
{
 int i;
 for (i=1;i<argc;i++){
  int num = atoi(argv[i])+1;
  printf("%i. number +1 is %i\n",i,num);
 }
 return 0;
}