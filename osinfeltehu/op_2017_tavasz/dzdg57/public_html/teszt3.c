#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

struct myStruct {
	int id;
	char *valami;
	char *valasz1;
	char *valasz2;
	char *valasz3;
	char *valasz4;
};

void kiir_fopen_bin(char* fajlnev, struct myStruct *st) {
	printf("\nKiiras fajlba elkezdodott\n");
	FILE * f;
	// http://stackoverflow.com/questions/2174889/whats-the-differences-between-r-and-rb-in-fopen
	// ezert hasznalunk wb es nem w-t... nem szovegfajllal dolgozunk (nem egy txt)
	f = fopen(fajlnev, "wb");
	printf("........................itt %i\n", sizeof(st)/sizeof(st[0]) );
	fwrite(st, sizeof(st[0]), sizeof(st)/sizeof(st[0]), f);
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

void menu() {
	while(1) {
		printf(" 1. Uj kerdes bevitele\n 2. Listazas\n 3. Jelentkezo adatainak modositasa\n 4. Jelentkezo torlese\n 5. Uj rendezveny inditasa\n 6. Rendezveny adatainak torlese\n 7. Kilepes\n ");
		
		int n;
		scanf("%d", &n);
		switch(n) {
			case 1:
				addNewQuestion();
				break;
			case 2:
				listQuery();
				break;
			case 3:
				//modifyVisitorData();
				break;
			case 4:
				//deleteVisitor();
				break;
			case 5:
				//createNewEvent();
				break;
			case 6:
				//deleteEvent();
				break;
			case 7:
				printf("Kilepes...\n");
				exit(0);
				break;
			default:
				printf("\nA megadott opciok kozul valassz!\n");
		}
				
			
	}

}

void addNewQuestion() {
	//struct queryStruct *st = malloc(sizeof(struct queryStruct));
	struct queryStruct st;
	printf("Adjon meg egy uj kerdest!\n");
	scanf("%s", &st.question);
	int numberOfAnswers = 0;
	do {
		printf("Hany lehetseges valaszt szeretne(2 es 4 kozott valasszon)!\n");
		scanf("%d", &numberOfAnswers);
	}
	while(!(numberOfAnswers <= 2 && numberOfAnswers <=4));
	
	int i;
	for(i=0; i<numberOfAnswers; i++) {
		printf("Az %i. valasz: \n", i);
		scanf("%s", &st.answers[i]);
		
	}
}

int main(int argc, char** argv) {
	// english abc #
	// hany kerdes
	//struct myStruct st[n];
	struct myStruct *st = malloc(sizeof(struct myStruct)*n);

	int i;
	int k;
	char * ch;
	for(i=1; i<n; i++) {
		st[i].num = st[i-1].num + 1;
		st[i].ch = st[i-1].ch + 1;
		st[i].valami = "szoveg";
		st[i].valasz1 = "elso";
		st[i].valasz2 = "masodik";
		st[i].valasz3 = "harmadik";
		st[i].valasz4 = "negyedik";
 		
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
		printf("valasz4: %s ", st2[i].valasz4);

	}
	
	free(st2);
}