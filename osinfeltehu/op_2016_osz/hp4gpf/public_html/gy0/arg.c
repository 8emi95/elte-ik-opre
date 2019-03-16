#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) // paramétereket kap itt meg, utána meg, hogy hány van  
//char** means an array of character arrays = array of strings
{
//c sajátossága, hogy kint külön kell deklarálni a dolgokat 
  int i;
  int a;
  printf("Number of command line arguments are: %i\n",argc);
  for (i=0;i<argc;i++){
    //Végigmegy és kiírja a változókat a karaktersor i-edik tagját
    if(i == 1){
      a = atoi(argv[i]);
      a = a + 1;
      printf("A megnovelt szam: %i\n\n", a);
    }
    //printf("%i. argument is %s\n",i,argv[i]); //%i intként %s zövegként kezelhje kiíráskor
  }
 return 0;
}