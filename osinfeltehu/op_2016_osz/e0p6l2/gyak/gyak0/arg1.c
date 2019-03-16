#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 if (argc != 2) return 1;

 int i = atoi(argv[1]) + 1;
 printf("%i\n", i);
 return 0;
}
