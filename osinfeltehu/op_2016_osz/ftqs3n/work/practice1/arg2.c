#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
  if ( argc != 2 )
  {
    printf("Number of command line arguments must be 2 !\n");
    exit(1);
  } 
  char* in_par1 = argv[1];

  int num;
  num = atoi(in_par1);
  num++;

  printf("%i\n", num);
  
  return 0;
}
