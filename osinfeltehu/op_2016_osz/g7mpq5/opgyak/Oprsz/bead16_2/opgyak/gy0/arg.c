#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i; // C <F4>ben kulon kell dekralalni a ciklus valtozot!!!
 printf("Number of command line arguments are: %i\n",argc);
 // a `. parameter a futtathato fajl neve
 for (i=0;i<argc;i++){
  printf("%i. argument is %s\n",i,argv[i]);
 }

 for (i=1;i<argc;i++){
  printf("%i. argument is %i\n",i,atoi(argv[i])+1);
 }

 // scanf
 int num = atoi(argv[1]) + 1; // szoveget szamma alakit
 printf("A szam %d\n", num);

 return 0;
}
