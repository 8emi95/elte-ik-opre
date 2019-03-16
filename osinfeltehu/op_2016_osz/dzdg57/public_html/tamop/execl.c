#include <stdio.h>

void main() {
	int a,b;
	a = fork();
	if(a) {
		printf("Hello, ez a szulo, ami var most!\n");
		printf("Most varakozik a wait...\n");
		wait(&b);
		printf("Most van vege a szulo varakozasanak...\n");
	} else {
		printf("Ez a gyermek processz! Most indul az execl...\n");
		execl("/usr/bin/ls", "ls", "-al", 0);
		printf("Es ezzel a kiirassal ez le is fut...\n");
		/* Ez az ut�bbi ki�r�s term�szetesen m�r nem jelenik meg
		   a k�perny�n, mivel a gyermek processz lecser�lte mag�t
		   az ls parancsra! */
	}
	
}