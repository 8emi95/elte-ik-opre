#include <stdio.h>
#include <stdlib.h>

int main(int argc,char ** argv) 
{
 if (argc>1)
 {
  printf("%i\n",atoi(argv[1])+1);
 return 0;
 }
 else {
 printf("Missing argument!\n");
 return 1;
 }
}
