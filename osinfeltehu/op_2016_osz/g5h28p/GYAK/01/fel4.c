#include <stdio.h>
#include <stdlib.h>

//4. Irjuk ki egymas utan a program bemeneti parametereit! Ugyanez forditott sorrendben.

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 //printf("Number of command line arguments are: %i\n",argc);
 for (i=1;i<argc;i++){
  printf(argv[i]);
 }
 printf("\n");
 for (i=argc;i>0;i--){
  printf(argv[i]);
 }
 return 0;
}
