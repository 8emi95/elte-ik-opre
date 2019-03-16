#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
//argc a paraméterszám a char** karaktermutatókra mutató tömb
{
 /*int i; //külön kell deklarálni a ciklusváltozót
 printf("Number of command line arguments are: %i\n",argc);
 for (i=0;i<argc;i++){
  printf("%i. argument is %s\n",i,argv[i]); //%i a szám %s pedig a string azaz karaktertömb
 }
 return 0;
 */
  int kiszam = atoi(argv[1])+1; //számmá alakítjuk a karaktertömböt
  printf("%i",kiszam);
}
 