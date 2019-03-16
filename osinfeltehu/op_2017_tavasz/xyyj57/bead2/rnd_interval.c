#include<stdlib.h>
#include <stdio.h>

unsigned int rand_interval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}

int main() {
	int rnd_valaszok_szama;
	int i;
	/*
	 * szülőtől kapjuk meg hogy az általa választott kérdéshez hány db válasz tartozik
	 * kell majd mind a 3 kérdéshez tudni
	 * a ciklusban 3 for lesz ?
	 */
	int kerdesekhez_tartozo_valaszok_szama = 3;	// 2-4 szám
	int rnd_kerdesszama;
	
	int ludas_matyi = 0;
	while (ludas_matyi != 3) {
		rnd_valaszok_szama = rand_interval(10,20);
		for (i = 0; i != rnd_valaszok_szama; ++i) {
		rnd_kerdesszama = rand_interval(1,kerdesekhez_tartozo_valaszok_szama);
		printf("Kerdes szama: %d\n",rnd_kerdesszama);
		}
		++ludas_matyi;
		printf("\n");
	}
	
	return 0;
}

/*
 * Szét kell bontani szülő és gyerek folyamatra
 * 
 */