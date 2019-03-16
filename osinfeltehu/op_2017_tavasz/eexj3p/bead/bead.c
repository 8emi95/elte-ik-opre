#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
	char nev[30];
	int ev;
	int honap;
	int nap;
} Jarat;

typedef struct {
	char nev[40];
	int ev;
	int honap;
	int nap;
	int telefon;
	Jarat* jarat;
} Utas;

int jaratokSzama;
Jarat* jaratok;
int utasokSzama;
Utas* utasok;

void fajlokBeolvasasa(FILE* fajl);
void kiirasFajlba(FILE* fajl);
void menu();
int sorokSzama(FILE* fajl);
void jaratokBeolvasasa(FILE* f);
void utasokBeolvasasa(FILE* f);
void jaratokMentese(FILE* f);
void utasokMentese(FILE* f);
int jaratKeresese(char* nev);
int utasokKeresese(char* nev);
void jaratokListazasa();
void utasokListazasa();
void ujJarat();
void ujUtas();
void jaratTorlese();
void utasTorlese();
void utasModositasa();
void jaratInfo();


int main() {
	FILE* fajl;
	fajlokBeolvasasa(fajl);
	
	menu();
	
	kiirasFajlba(fajl);
}

void fajlokBeolvasasa(FILE* fajl) {
	
	if ((fajl = fopen("jaratok.txt", "r")) == NULL) {
		perror("Hiba az jaratok.txt fajl megnyitasakor!\n");
		exit(1);
	}
	jaratokBeolvasasa(fajl);
	fclose(fajl);
	if ((fajl = fopen("utasok.txt", "r")) == NULL) {
		perror("Hiba az utasok.txt fajl megnyitasakor!\n");
		exit(1);
	}
	utasokBeolvasasa(fajl);
	fclose(fajl);
}

void kiirasFajlba(FILE* fajl) {
	if ((fajl = fopen("jaratok.txt", "w")) == NULL) {
		perror("Hiba az jaratok.txt fajl megnyitasakor!\n");
		exit(1);
	}
	jaratokMentese(fajl);
	fclose(fajl);
	
	if ((fajl = fopen("utasok.txt", "w")) == NULL) {
		perror("Hiba az utasok.txt fajl megnyitasakor!\n");
		exit(1);
	}
	utasokMentese(fajl);
	fclose(fajl);
}

void menu() {
	int menuSzama = -1;
	printf("Udvozlom a Best of Uber alkalmazasban.\n");
	while(menuSzama != 0) {
		printf("\n");
		printf("Irja be a kivan menu szamat: \n");
		printf("1) Jaratok listazasa\n");
		printf("2) Utasok listazasa\n");
		printf("3) Uj jarat\n");
		printf("4) Uj utas\n");
		printf("5) Jarat torlese\n");
		printf("6) Utas torlese\n");
		printf("7) Utas modositasa\n");
		printf("8) Jarat informacio\n");
		scanf("%i",&menuSzama);
		switch(menuSzama) {
			case 0:;
				break;
			case 1:
				jaratokListazasa();
				break;
			case 2:
				utasokListazasa();
				break;
			case 3:
				ujJarat();
				break;
			case 4:
				ujUtas();
				break;
			case 5:
				jaratTorlese();
				break;
			case 6:
				utasTorlese();
				break;
			case 7:
				utasModositasa();
				break;
			case 8:
				jaratInfo();
				break;
			default:
				printf("Nem lehetseges menu!!\n");
		}
	}
}

int sorokSzama(FILE* fajl) {
	int i = 0;
	int ch;
	do {
		ch = fgetc(fajl);
		if (ch == '\n') ++i;	
	} while(ch != EOF);
		rewind(fajl);
		if(i < 1) ++i;	
	return i;
}


void jaratokBeolvasasa(FILE* f) {
	jaratokSzama = sorokSzama(f);
	jaratok = (Jarat*) malloc(sizeof(Jarat) * jaratokSzama);
	int i;
	for(i = 0; i < jaratokSzama; ++i)
		fscanf(f, "%[^;];%i.%i.%i.\n",
		jaratok[i].nev,
		&jaratok[i].ev,
		&jaratok[i].honap,
		&jaratok[i].nap);
}

void utasokBeolvasasa(FILE* f) {
  utasokSzama = sorokSzama(f);
  utasok = (Utas*) malloc(sizeof(Utas) * utasokSzama);
  int i;
  for(i = 0; i < utasokSzama; ++i) {
    char nev[30];
    int ev, honap, nap;
    fscanf(f, "%[^;];%i.%i.%i.;%[^;];%[0-9]\n",
           utasok[i].nev,
           &utasok[i].ev,
           &utasok[i].honap,
           &utasok[i].nap,
           nev,
           utasok[i].telefon);
    int j = jaratKeresese(nev);
    utasok[i].jarat = &jaratok[j];
  }
}

void jaratokMentese(FILE* f) {
	int i = 0;
	for(; i < jaratokSzama; ++i) {
		fprintf(f, "%s;%04i.%02i.%02i.\n",
		jaratok[i].nev,
		jaratok[i].ev,
		jaratok[i].honap,
		jaratok[i].nap);
  }
}

void utasokMentese(FILE* f) {
	int i = 0;
	for(; i < utasokSzama; ++i) {
		fprintf(f, "%s;%04i.%02i.%02i.;%s;%s\n",
		utasok[i].nev,
		utasok[i].ev,
		utasok[i].honap,
		utasok[i].nap,
		utasok[i].jarat->nev,
		utasok[i].telefon);
  }
}

int jaratKeresese(char* nev) {
	fscanf(stdin, "\n%[^;\n]", nev);
	int i = 0;
	for(; i < jaratokSzama; ++i)
		if(0 == strcmp(jaratok[i].nev, nev))
			break;
	return i;
}

int utasKeresese(char* nev) {
	fscanf(stdin, "\n%[^;\n]", nev);
	int i = 0;
	for(; i < utasokSzama; ++i)
		if(0 == strcmp(utasok[i].nev, nev))
			break;
	return i;
}


void jaratokListazasa() {
  int i = 0;
  for(; i < jaratokSzama; ++i)
    printf("    %s;%i.%i.%i.\n",
		jaratok[i].nev,
		jaratok[i].ev,
		jaratok[i].honap,
		jaratok[i].nap);
}

void ujJarat() {
	Jarat* jar = (Jarat*) malloc(sizeof(Jarat) * (jaratokSzama + 1));
	int i = 0;
	for(; i < jaratokSzama; ++i) {
		strcpy(jar[i].nev,jaratok[i].nev);
		jar[i].ev = jaratok[i].ev;
		jar[i].honap = jaratok[i].honap;
		jar[i].nap = jaratok[i].nap;
	}
	free(jaratok);
	jaratok = jar;
	printf("\nAdja meg az új úticél adatait: ");
		fscanf(stdin, "\n%[^;];%i.%i.%i.",
		jaratok[i].nev,
		&jaratok[i].ev,
		&jaratok[i].honap,
		&jaratok[i].nap);
	++jaratokSzama;
}

void jaratTorlese() {
	printf("\nAdja meg a törlendő elem nevét: ");
	char nev[30];
	fscanf(stdin, "\n%[^;\n]", nev);
	int i = jaratKeresese(nev);
	
	if(i == jaratokSzama) {
		printf("  Nem volt ilyen nevű járat!\n");
		return;
	}
	
	Jarat* jar = (Jarat*)malloc(sizeof(Jarat) * (jaratokSzama - 1));
	int j = 0;
	
	for(; j < jaratokSzama; ++j) {
		if(i == j) {
			continue;
		} else if(j < i) {
			strcpy(jar[j].nev, jaratok[j].nev);
			jar[j].ev = jaratok[j].ev;
			jar[j].honap = jaratok[j].honap;
			jar[j].nap = jaratok[j].nap;
		} else {
			strcpy(jar[j-1].nev, jaratok[j].nev);
			jar[j-1].ev = jaratok[j].ev;
			jar[j-1].honap = jaratok[j].honap;
			jar[j-1].nap = jaratok[j].nap;
		}
	}
	
	free(jaratok);
	jaratok = jar;
	--jaratokSzama;
}

void utasokListazasa() {
	int i = 0;
	for(; i < utasokSzama; ++i)
		printf("'%s';'%i.%i.%i.';'%s';'%s'\n",
		utasok[i].nev,
		utasok[i].ev,
		utasok[i].honap,
		utasok[i].nap,
		utasok[i].jarat->nev,
		utasok[i].telefon);
}

void ujUtas() {
	time_t curr;
	time(&curr);
	struct tm local = *(localtime(&curr));
	int ev = local.tm_year + 1900;
	int honap = local.tm_mon + 1;
	int nap = local.tm_mday;
	Utas* tmp = (Utas*) malloc(sizeof(Utas) * (utasokSzama + 1));
	int i = 0;
	
	for(; i < utasokSzama; ++i) {
		tmp[i].ev = utasok[i].ev;
		tmp[i].honap = utasok[i].honap;
		tmp[i].nap = utasok[i].nap;
		tmp[i].jarat = utasok[i].jarat;
		tmp[i].nev, utasok[i].nev;
		tmp[i].telefon, utasok[i].telefon;
	}
	
	free(utasok);
	utasok = tmp;
	char nev[30];
	utasok[utasokSzama].ev = ev;
	utasok[utasokSzama].honap = honap;
	utasok[utasokSzama].nap = nap;
	printf("\nAdja meg az új utas adatait: ");
	printf("\n  Név: ");
	fscanf(stdin, "\n%[^;\n]", utasok[i].nev);
	printf("\n  Úticél: ");
	fscanf(stdin, "\n%[^;\n]", nev);
	printf("\n  Telefon: ");
	fscanf(stdin, "\n%[0-9]", utasok[i].telefon);
	int j = jaratKeresese(nev);
	if(j != jaratokSzama)
		utasok[utasokSzama].jarat = &jaratok[j];
	else
		utasok[utasokSzama].jarat = NULL;
	++utasokSzama;
}

void utasModositasa() {
	printf("\nAdja meg a módosítandó utas nevét: ");
	char nev[40];
	fscanf(stdin, "\n%[^;\n]", nev);
	int i = utasKeresese(nev);
	if(i == utasokSzama) {
	printf("  Nem volt ilyen nevű utas!\n");
	return;
	}
	printf("Mit kíván módosítani?\n"
		"  1. Név\n"
		"  2. Úticél\n"
		"  3. Telefonszám\n"
		">>> ");
	int input;
	fscanf(stdin, "%i", &input);
	switch(input) {
		case 1:
		printf("  Adja meg az új nevet: ");
		fscanf(stdin, "\n%[^;\n]", nev);
		strcpy(utasok[i].nev, nev);
		break;
		case 2:
		printf("  Adja meg az új úticélt: ");
		fscanf(stdin, "\n%[^;\n]", nev);
		break;
		case 3:
		printf("  Adja meg az új telefonszámot: ");
		fscanf(stdin, "\n%[0-9]", utasok[i].telefon);
		break;
		default:
		printf("  Nem történt változtatás!\n");
		return;
	}
	
	if(input != 3)
	return;
	int j = jaratKeresese(nev);
	if(j >= jaratokSzama) {
	printf("  Nem létező állomás!\n");
	return;
	}
	utasok[i].jarat = &jaratok[j];
	}

void utasTorlese() {
	printf("\nAdja meg a törlendő utas nevét: ");
	char name[80];
	fscanf(stdin, "\n%[^;\n]", name);
	int i = utasKeresese(name);
	
	if(i == utasokSzama) {
		printf("  Nem volt ilyen nevű utas!\n");
		return;
	}
	
	Utas* tmp = (Utas*)malloc(sizeof(Utas) * (utasokSzama - 1));
	int j = 0;
	for(; j < utasokSzama; ++j) {
		if(i == j) {
			continue;
		} else if(j < i) {
			strcpy(tmp[j].nev, utasok[j].nev);
			tmp[j].telefon, utasok[j].telefon;
			tmp[j].ev = utasok[j].ev;
			tmp[j].honap = utasok[j].honap;
			tmp[j].nap = utasok[j].nap;
			tmp[j].jarat = utasok[j].jarat;
		} else {
			strcpy(tmp[j-1].nev, utasok[j].nev);
			tmp[j-1].telefon, utasok[j].telefon;
			tmp[j-1].ev = utasok[j].ev;
			tmp[j-1].honap = utasok[j].honap;
			tmp[j-1].nap = utasok[j].nap;
			tmp[j-1].jarat = utasok[j].jarat;
		}
	}
	free(utasok);
	utasok = tmp;
	--utasokSzama;
}

void jaratInfo() {
	printf("\nAdja meg a kívánt elem nevét: ");
	char nev[40];
	fscanf(stdin, "\n%[^;\n]", nev);
	int i = 0;
	for(; i < jaratokSzama; ++i)
		if(0 == strcmp(jaratok[i].nev, nev))
			break;
	if(i == jaratokSzama) {
		printf("  Nem volt ilyen nevű járat!\n");
		return;
	}
	printf("Indulás: %04i.%02i.%02i.:\n"
		, jaratok[i].ev
		, jaratok[i].honap
		, jaratok[i].nap);
	int j = 0;
	for(; j < utasokSzama; ++j) {
		if(jaratok+i == utasok[j].jarat) {
			printf(" + > Név:\t\t %s\n", utasok[j].nev);
			printf("   > Jelentkezés:\t %04i.%02i.%02i.\n",
				utasok[i].ev,
				utasok[i].honap,
				utasok[i].nap);
			printf("   > Telefonszám:\t %s\n", utasok[i].telefon);
		}
	}
}
