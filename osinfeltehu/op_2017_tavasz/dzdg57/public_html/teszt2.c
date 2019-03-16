#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

struct myStruct {
	int num;
	char ch;
	char *valami;
	char *valasz1;
	char *valasz2;
	char *valasz3;
	char *valasz4;
};

void kiir_fopen_bin(char* fajlnev, struct myStruct *str) {
	printf("\nKiiras fajlba elkezdodott\n");
	FILE * f;
	// http://stackoverflow.com/questions/2174889/whats-the-differences-between-r-and-rb-in-fopen
	// ezert hasznalunk wb es nem w-t... nem szovegfajllal dolgozunk (nem egy txt)
	f = fopen(fajlnev, "wb");
	int temp = sizeof(str);
	printf("sizeof(int): %i, sizeof(ch): %i, sizeof(char*): %i\n", sizeof(int), sizeof(char), sizeof(char*));
	printf("sizeof(str): %i, sizeof(str[0]): %i \n", sizeof(str), sizeof(str[0]) );
	fwrite(str, sizeof(str[0]), sizeof(str)/sizeof(str[0]), f);
	fclose(f);
	printf("Kiiras fajlba befejezodott\n");
}

struct myStruct * beolvas_fopen_bin(char* fajlnev) {
	printf("\nBeolvasas a fajlbol megkezdodott\n");
	FILE * f;
	int n=26;
	printf("beolvasasnal sizeof(struct myStruct): %i\n", sizeof(struct myStruct));
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
	int n = 4;
	//struct myStruct st[n];
	struct myStruct *st = malloc(sizeof(struct myStruct)*n);
	
	int i;
	int k;
	char * ch;
	for(i=0; i<n; i++) {
		st[i].num = i + 1;
		st[i].ch = 'a';
		char tempTomb[50] = "valami";
		st[i].valami = tempTomb;
		// ilyen nincs:!!!!!!!!!
		//tempTomb = "elso";
		strcpy(*st[i].valasz1, "elso");
		strcpy(tempTomb, "masodik");
		st[i].valasz2 = tempTomb;
		strcpy(tempTomb, "harmadik");
		st[i].valasz3 = tempTomb;
		strcpy(tempTomb, "negyedik");
		st[i].valasz4 = tempTomb;
 		
		for(k=0; k<2; k++) {
			
			//sprintf(ch, "A %i. tombelem", k);
			//st[i].tomb[k] = malloc(sizeof(ch) * 50);
			//st[i].tomb[k] = "bla";
			//printf(ch);
		}
	}
	
	//tomb kiirasa fajlba
	char * fajlnev = "tomb.dat";
	//if(!access(fajlnev, W_OK) == 0) {perror("Nincs jogod irni a fajlt\n"); exit(1);}
	kiir_fopen_bin(fajlnev, st);	
	// felszabadítom
	free(st);
	
	// visszaolvasom egy új tombbe
	//struct myStruct *st2 = malloc(sizeof(struct myStruct)*n);
	struct myStruct *st2 = beolvas_fopen_bin(fajlnev);
	
	// kiiratom a fajlbol visszaolvasott strukturatombot
	printf("\n A fajlbol visszaolvasott adatok kiiratasa: \n");
	for(i=0; i<n; i++) {
		printf("az %i. elem sorszama: %i, karakter: %c\n",i, st2[i].num, st2[i].ch);
		printf("ide a szoveg %s ", st2[i].valami);
		int k;
		printf("\n");
		printf("valasz1: %s ", st2[i].valasz1);
		printf("valasz2: %s ", st2[i].valasz2);
		printf("valasz3: %s ", st2[i].valasz3);
		printf("valasz4: %s\n", st2[i].valasz4);

	}
	
	free(st2);
}