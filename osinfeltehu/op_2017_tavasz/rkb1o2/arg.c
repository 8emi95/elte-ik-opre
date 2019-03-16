#include <stdio.h>
#include <stdlib.h> 

//call it wit some parameters from the commend line

int main (int argc,char ** argv) 
// char** means an array of character arrays = array of strings

{
 int i;
 printf ("Number of commend line arguments are %i\n", argc);
 for (i=0; i<argc; i++) {
  printf("%i %i  \n",i,atoi(argv[i])+1);  
  
 }
 return 0; 

} 

