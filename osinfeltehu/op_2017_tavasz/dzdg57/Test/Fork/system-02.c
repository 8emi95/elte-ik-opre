#include <stdio.h>
#include <stdlib.h>

main() {
	int hibakod;
	
	char *argv[1];
	argv[0] = "/bin/ls";
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	
	// A f�ggv�ny seg�ts�g�vel egyszer�en ind�thatunk programokat.
	// A f�ggv�ny gyermekfolyamatk�nt futtatja a /bin/sh parancs�rtelmez�t, v�grehajtva a f�ggv�ny param�terek�nt jel�lt parancsot. A futtat�s sor�n a 
	// parancs�rtelmez� a PATH k�rnyezeti v�ltoz�t haszn�lja a parancsban esetleg szerepl� futtatand� programf�jl megkeres�s�re.
	// Ha a parancsot nem lehetett futtatni, a visszat�r�si �rt�k -1, k�l�nben a gyermekfolyamat visszat�r�si �rt�ke.
	hibakod = system(argv[0]);
	
	if( hibakod == -1 ) {
		printf("A programot nem lehet futtatni.\n");
	} else {
		printf("Hibakod:%d\n", hibakod);
	}
}