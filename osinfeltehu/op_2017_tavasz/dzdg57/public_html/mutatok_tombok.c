#include <stdio.h>

// a mutat� t�pus�b�l tudja a ford�t�, hogy mekkora helyet foglalnak a t�mb elemei, h�ny b�jtnyit kell a mem�ri�ban el�rel�pni, hogy a k�vetkez�
// elemhez jussunk. Az �tadott mutat� t�pusa teh�t fontosabb, mint azt els� pillant�sra gondoltuk volna.

kiir( int *tomb ) {
	// a f�ggv�ny a mutat�t t�mbk�nt kezeli
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
	// a f�ggv�nyt int * t�pus� mutat�val kell megh�vni, mi azonban a t�mb nev�vel h�vtuk meg
	// a C nyelvben a t�mb nev�nek [] jelek n�lk�l val� megh�v�sa egyen�rt�k� a legels� - a 0 index� - elem c�m�vel (&t[0]).
	kiir(t);
	// teh�t ez ugyanaz:
	// kiir(&t[0]);
	
	printf("----------------------------");
	t[2]=12;
	// �ltal�nosan elterjedt fog�sa C nyelv� programoz�sban, hogy a t�mb c�m�vel egy�tt a t�mb m�ret�t is �tadjuk, hiszen m�sk�ppen nem tudhatjuk meg
	// a h�vott f�ggv�nyen bel�l a t�mb m�ret�t.
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