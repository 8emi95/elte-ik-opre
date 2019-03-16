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
	// & jelenti, hogy az adat címét kívánjuk használni, nem pedig az értékét
	mutato = &adat;
	
	printf("adat: %i, mutato: %i\n", adat, *mutato);
	
	// mutató elõtt a * karakter itt jelzi, hogy nem a mutatóról, hanem az általa jelölt memóriaterületrõl van szó. (pointer dereference) 
	//(nem tévesztendõ össze a mutató létrehozásával, hiába használunk mindkét esetben *-ot a mutató neve elõtt)
	*mutato = 35;
	printf("uj ertek: \t adat: %i, mutato: %i\n", adat, *mutato);
	
	
	char chAdat;
	chAdat = 'f';
	
	// így megszabadulhatunk a warning-tól... nyilván ez nem túl elegáns... pointerek mérete ugyanolyan, de azért int-hez int*-ot char-hoz char*-ot használjunk...
	mutato = (int *) &chAdat;

	printf("chAdat: %c, mutato: %c\n", chAdat, *mutato);
	
	
	printf("-------------------------------\n");
	// PARAMÉTER ÁTADÁS MUTATÓKKAL
	
	int elso = 2;
	int masodik = 5;
	novel(&elso, &masodik);
	printf("elso: %d, masodik: %d\n", elso, masodik);
	
}

novel(int * a, int * b) {
	*a += 1;
	// ha így csináljuk, akkor viszont kell a zárójel, mert a * erõsebb
	(*b)++;
}