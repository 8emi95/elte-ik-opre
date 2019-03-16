#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i, szam, osszeg;
 printf("Number of command line arguments are: %i\n",argc);
 for (i=0;i<argc;i++){
  printf("%i. argument is %s\n",i,argv[i]);
 }
     
 if (argc > 1) {
  szam = atoi(argv[1]);
  osszeg = szam + 1;
  printf("Az uj osszeg: %i\n", osszeg);
 }
 
 return 0;
}
