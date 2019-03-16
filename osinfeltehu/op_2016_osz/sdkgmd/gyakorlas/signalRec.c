//-------------------- 
// SigKap.c 
//-------------------- 
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <signal.h>       // Például a kill függvény. 
#include <sys/types.h>    // Például a pid_t típus. 

void Kezelo(int sig) 
{ 
	// A callback függvény, paramétere a szignál számértéke. 
	printf("\n%i –s szamu  jelzest kaptam \n", sig); 
} 

int main(int argc, char * argv[]) { 
	signal(SIGTERM, Kezelo);  // Beállítja a SIGTERM szignál kezelőjének 
                              // a Kezelo függvényt. 
	pause();  // A jelzés érkezéséig vár. 
	printf("\nItt a program vege\n"); 
	fflush(stdout); 
	return 0; 
} 
//--------------------
