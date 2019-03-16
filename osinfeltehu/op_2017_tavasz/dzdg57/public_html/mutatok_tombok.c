#include <stdio.h>

// a mutató típusából tudja a fordító, hogy mekkora helyet foglalnak a tömb elemei, hány bájtnyit kell a memóriában elõrelépni, hogy a következõ
// elemhez jussunk. Az átadott mutató típusa tehát fontosabb, mint azt elsõ pillantásra gondoltuk volna.

kiir( int *tomb ) {
	// a függvény a mutatót tömbként kezeli
	printf("tomb[0]: %d\n", tomb[0]);
	printf("tomb[1]: %d\n", tomb[1]);
}

int legnagyobb (int *tomb, int meret) {
	int max = tomb[0];
	int i;
	for(i=1; i<meret; i++) {
		if(tomb[i]>max) {
			max = tomb[i];
		}
	}
	
	return max;
}

struct vegyes {
	int szam;
	char tomb[3][50];
};

main() {
	int t[5];
	t[0] = 42;
	t[1] = 43;
	// a függvényt int * típusú mutatóval kell meghívni, mi azonban a tömb nevével hívtuk meg
	// a C nyelvben a tömb nevének [] jelek nélkül való meghívása egyenértékû a legelsõ - a 0 indexû - elem címével (&t[0]).
	kiir(t);
	// tehát ez ugyanaz:
	// kiir(&t[0]);
	
	printf("----------------------------");
	t[2]=12;
	// általánosan elterjedt fogása C nyelvû programozásban, hogy a tömb címével együtt a tömb méretét is átadjuk, hiszen másképpen nem tudhatjuk meg
	// a hívott függvényen belül a tömb méretét.
	printf("Legnagyobb=%d\n", legnagyobb(t, 3) );
	
	
	int intTomb[2][3];
	int i, k;
	for(i=0; i<2; ++i) {
		for(k=0; k<3; k++) {
			intTomb[i][k] = i*k;
		}
	}
	
	for(i=0; i<2; ++i) {
		for(k=0; k<3; k++) {
			printf("%i ", intTomb[i][k]);
		}
		printf("\n");
	}
	
	struct vegyes st1;
	st1.szam = 2;
	char ab[50] = "valami 1";
	st1.tomb[0] = ab;
	st1.tomb[1] = ab
	st1.tomb[2] = ab;
	
	printf("st1.int: %i\n", st1.szam);
	//for(i=0; )
}