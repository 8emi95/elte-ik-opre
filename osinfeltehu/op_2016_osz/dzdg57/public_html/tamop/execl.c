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
		/* Ez az utóbbi kiírás természetesen már nem jelenik meg
		   a képernyõn, mivel a gyermek processz lecserélte magát
		   az ls parancsra! */
	}
	
}