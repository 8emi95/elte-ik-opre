#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) //1. param az arg dbszama, karaktermutatok tombje a 2. param
//char** means an array of character arrays = array of strings
{
 int i; //kulon kell a ciklusvaltozot deklaralni!
 --int atoi(chartomb) feladat: bemenet egesz szam, hozzaadni 1-et
 printf("Number of command line arguments are: %i\n",argc); //1. param konstans szovegeket + valtozot. %i. 2. parameterben szam
 for (i=0;i<argc;i++){
  printf("%i. argument is %s\n",i,argv[i]); //%i egesz ertek, %s a szoveg
 }
 return 0;
}