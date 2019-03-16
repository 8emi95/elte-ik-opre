#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 int ertek;
 
 printf("Number of command line arguments are: %i\n",argc);
 for (i=1;i<argc;i++){
	 
  ertek = atoi(argv[i])+1;
  printf("%i. argument is %i\n",i,ertek)+1;
 }
 return 0;
}

//c nem tartalmaz ossztályokat, string típus sincs
//string típus helyett adattömb van, amelyet a memória címe azonosít char** = charakter tömb
//charakter mutató tömb: mutatók mutatója
//1 paraméter: hány bemenőparaméter lesz
//külön kell deklarálni a ciklusváltozót
//