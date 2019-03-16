#include <stdio.h>
#include <stdlib.h>

main() {
	int hibakod;
	
	char *argv[1];
	argv[0] = "/bin/ls";
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	
	// A függvény segítségével egyszerûen indíthatunk programokat.
	// A függvény gyermekfolyamatként futtatja a /bin/sh parancsértelmezõt, végrehajtva a függvény paramétereként jelölt parancsot. A futtatás során a 
	// parancsértelmezõ a PATH környezeti változót használja a parancsban esetleg szereplõ futtatandó programfájl megkeresésére.
	// Ha a parancsot nem lehetett futtatni, a visszatérési érték -1, különben a gyermekfolyamat visszatérési értéke.
	hibakod = system(argv[0]);
	
	if( hibakod == -1 ) {
		printf("A programot nem lehet futtatni.\n");
	} else {
		printf("Hibakod:%d\n", hibakod);
	}
}