#include <stdio.h>
#include <stdlib.h>
//call it with some parameters from the command line 

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
 int i;
 printf("Number of command line arguments are: %i\n",argc);
 for (i=1;i<argc;i++){
  printf("%i. argument is %i\n",i,(atoi(argv[i]) + 1));
 }
 return 0;
}

/*
main fuggveny parametere
A main fuggveny a program belepesi pontja:
parametere a program meghivasakor adodik at.

int argc : mennyi argumentum van megadva
char ** argv : stringek tombje

printf  : tartalmazhat konstans szovegeket
		: valtozokat, melyek ertekei kiirasra kerulnek
		:  valtozo tipusanak megadasa: %i : integer tipusu valtozo
		:  							   %s : string tipusu valtozo
c - ben az ciklus inicializacios reszet
	a ciklus blokkon kivul kell megirni/ deklaralni
	
Egy kod forditasa:
gcc arg.c

Az eredmeny fajl mindig x joggal rendelkezik.
Fajl futtatasa ./a.out

Egy argumentumnak szamit a parancsnak a neve- a futtatando allomanynak a neve

atoi() string to integer conversion function

*/