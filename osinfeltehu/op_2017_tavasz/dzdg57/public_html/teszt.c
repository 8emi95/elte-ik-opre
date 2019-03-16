#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

struct myStruct {
	int num;
	char ch;
	char * valami;
	char * tomb[50];
};

void kiir_fopen_bin(char* fajlnev, struct myStruct *str) {
	printf("\nKiiras fajlba elkezdodott\n");
	FILE * f;
	// http://stackoverflow.com/questions/2174889/whats-the-differences-between-r-and-rb-in-fopen
	// ezert hasznalunk wb es nem w-t... nem szovegfajllal dolgozunk (nem egy txt)
	f = fopen(fajlnev, "wb");
	fwrite(str, sizeof(str[0]), sizeof(str)/sizeof(str[0]), f);
	fclose(f);
	printf("Kiiras fajlba befejezodott\n");
}

struct myStruct * beolvas_fopen_bin(char* fajlnev) {
	printf("\nBeolvasas a fajlbol megkezdodott\n");
	FILE * f;
	int n=26;
	struct myStruct *str = malloc(sizeof(struct myStruct)*n);
	// http://stackoverflow.com/questions/2174889/whats-the-differences-between-r-and-rb-in-fopen
	// ezert hasznalunk wb es nem w-t... nem szovegfajllal dolgozunk (nem egy txt)
	f = fopen(fajlnev, "rb");
	fread(str, sizeof(str[0]), sizeof(str)/sizeof(str[0]), f);
	fclose(f);
	printf("Beolvasas a fajlbol befejezodott\n");
	return str;
}

int main(int argc, char** argv) {
	// english abc #
	int n = 26;
	//struct myStruct st[n];
	struct myStruct *st = malloc(sizeof(struct myStruct)*n);
	st[0].num = 0;
	st[0].ch = 'a';
	int i;
	int k;
	char * ch;
	for(i=1; i<n; i++) {
		st[i].num = st[i-1].num + 1;
		st[i].ch = st[i-1].ch + 1;
		st[i].valami = "szoveg";
		st[i].tomb[0] = "valami";
		st[i].tomb[0] = "mas";
		for(k=0; k<2; k++) {
			
			//sprintf(ch, "A %i. tombelem", k);
			//st[i].tomb[k] = "valami";
			//printf(ch);
		}
	}
	
	//tomb kiirasa fajlba
	char * fajlnev = "tomb.dat";
	//if(!access(fajlnev, W_OK) == 0) {perror("Nincs jogod irni a fajlt\n"); exit(1);}
	kiir_fopen_bin(fajlnev, st);	
	// felszabad�tom
	free(st);
	
	// visszaolvasom egy �j tombbe
	//struct myStruct *st2 = malloc(sizeof(struct myStruct)*n);
	struct myStruct *st2 = beolvas_fopen_bin(fajlnev);
	
	// kiiratom a fajlbol visszaolvasott strukturatombot
	printf("\n A fajlbol visszaolvasott adatok kiiratasa: \n");
	for(i=0; i<n; i++) {
		printf("az %i. elem sorszama: %i, karakter: %c\n",i, st2[i].num, st2[i].ch);
		printf("ide a szoveg %s\n", st2[i].valami);
		int k;
		printf("ide tomb %s\n", st2[i].tomb[0] );
		for(k=0; k<2; k++) {
			//printf("ide meg a tomb elemei %", st2[i].tomb[k]);
		}

	}
	
	free(st2);
}
