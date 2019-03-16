#include <stdlib.h>
#include <stdio.h>


main() {
	int tomb[5];
	int n;
	
	for(n=0; n<5; n=n+1) {
		tomb[n] = n*n+1;
		printf("tomb[%d]=%d\n", n, tomb[n] );
	}
	
	int *mutato;
	int adat;
	
	adat = 13;
	// & jelenti, hogy az adat c�m�t k�v�njuk haszn�lni, nem pedig az �rt�k�t
	mutato = &adat;
	
	printf("adat: %i, mutato: %i\n", adat, *mutato);
	
	// mutat� el�tt a * karakter itt jelzi, hogy nem a mutat�r�l, hanem az �ltala jel�lt mem�riater�letr�l van sz�. (pointer dereference) 
	//(nem t�vesztend� �ssze a mutat� l�trehoz�s�val, hi�ba haszn�lunk mindk�t esetben *-ot a mutat� neve el�tt)
	*mutato = 35;
	printf("uj ertek: \t adat: %i, mutato: %i\n", adat, *mutato);
	
	
	char chAdat;
	chAdat = 'f';
	
	// �gy megszabadulhatunk a warning-t�l... nyilv�n ez nem t�l eleg�ns... pointerek m�rete ugyanolyan, de az�rt int-hez int*-ot char-hoz char*-ot haszn�ljunk...
	mutato = (int *) &chAdat;

	printf("chAdat: %c, mutato: %c\n", chAdat, *mutato);
	
	
	printf("-------------------------------\n");
	// PARAM�TER �TAD�S MUTAT�KKAL
	
	int elso = 2;
	int masodik = 5;
	novel(&elso, &masodik);
	printf("elso: %d, masodik: %d\n", elso, masodik);
	
}

novel(int * a, int * b) {
	*a += 1;
	// ha �gy csin�ljuk, akkor viszont kell a z�r�jel, mert a * er�sebb
	(*b)++;
}