/*1. beadandó feladat. Beadási határidő: 2015 november 2. Bemutatási határidő: 2015 november 7.
Készítsen C programot, amely rendszerhívások alkalmazásával megoldja a következő feladatot:

Télapu lassan beletörődik, hogy mostantól tényleg ő a Mikulás, így a manókkal és a rénszarvasokkal együtt készül a Karácsonyra. Sorban érkeznek hozzá a gyerekek ajándékkérő levelei. Mindenki csak egyet kívánhat! A manók a levelek alapján a kéréseket egy fájlban tárolják, amely tartalmazza a beérkezés dátumát, a gyerek nevét, a várost, azon belüli pontos címet és a kért ajándékot. A manók idén csak babát, autót, labdát, kirakót, színezőt és könyvet visznek ajándékba!

Készítsünk C programot, amellyel lehessen levelet írni Télapunak (billentyűzeten keresztül adjuk meg az adatokat), majd az adatok tároljuk fájlba. Ne felejtsük el, hogy mindenki csak egyet kérhet! Az utolsó kérés maradjon érvényes! Lehessen listázni a manók fájlját, szűrhessük városra, ajándékra. Adjuk meg a listázások végén az összesítéseket is, azt hogy hány kérés érkezett.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
//#define MAX 50

const char* ajandek[] = {"baba", "auto", "labda", "kirako", "szinezo", "konyv"};

struct level {
 char nev[128];
 int ajandekId;
 char varos[128];
 char cim[512];
 time_t beerkezes;
};

//struct level* kivansagLista[MAX];
struct level** kivansagLista;
int maxMeret = 20;
int meret = 0;

void listaBovites();
void listaMentese(char* filename);

void levelIras();
void listaKiiras();
void kiir(struct level* kivansag, int i);

void ujlevel();
void ujlevelFromParam(char* nev, int ajandekId, char* varos, char* cim);

int main(int argc, char** argv)
{
	struct level* aLista[maxMeret];
	kivansagLista = aLista;

	int menu = 0;
	//while ( menu < 4 ) 
	while ( 1 ) {
		printf("\n\nNyomd meg a megfelelo szamot az alabbi lehetosegek valasztasahoz:\n");
		printf("\t1: Level irasa\n");
		printf("\t2: Lista kiirasa\n");
		printf("\t3: Kilepes es mentes\n");
		printf("\t4: Kilepes\n");
		
		int beolv = scanf("%d", &menu);
		if( beolv != 1 ) {
			menu = 5;
			printf("Hibas megadas!\n");
			continue;
		}

		switch(menu) {
			case 1:
				levelIras();
				break;
			case 2:
				listaKiiras();
				break;
			case 3:
				if( meret != 0 ) {
					char* filename;
					if( argc > 1 ) {
						filename = (char*)malloc((strlen(argv[1])+1)*sizeof(char));
						strcpy(filename, argv[1]);
					}
					else {
						//printf("Az adat fajl neve az elso parameterben megadhato!\n");
						filename = (char*)malloc((strlen("ajandek_lista.txt")+1)*sizeof(char));
						strcpy(filename, "ajandek_lista.txt");
					}

					listaMentese(filename);
					free(filename);
				}
			default:
				if( meret != 0 ) {
					int i;
					for( i = meret-1; i >= 0; i--) {
						struct level* tmp = kivansagLista[i];
						free(tmp);
					}
				}
				return 0;
				break;
		}
	}

	/*if( meret != 0 ) {
		int i;
		for( i = meret-1; i >= 0; i--) {
			struct level* tmp = kivansagLista[i];
			free(tmp);
		}
	}*/
	return 0;
}

void ujLevelFromParam(char* nev, int ajandekId, char* varos, char* cim) {

	//size_t obsSize= 2*128*sizeof(char)+2*sizeof(int)+sizeof(time_t);
	//struct level* akt = malloc(sizeof(struct level));
	struct level* akt;
	akt = NULL;

	int i = 0;
	for( i = 0; i < meret; i++) {
		if( strcmp(kivansagLista[i]->nev, nev) == 0 ) {
			akt = kivansagLista[i];
		}
	}
	if( akt == NULL ) {
		akt = malloc(sizeof(struct level));
		kivansagLista[meret] = akt;
		meret++;
	}

	//akt->nev = strdup(nev);
	strcpy(akt->nev, strdup(nev));
	akt->ajandekId = ajandekId;
	strcpy(akt->varos, strdup(varos));
	strcpy(akt->cim, strdup(cim));

	/*time_t current_raw;
	struct tm*	ido = malloc(sizeof(struct tm));
	time(&current_raw);
	localtime_r(&current_raw, ido);
	akt->ido = ido;*/
	time(&akt->beerkezes);
}

void ujLevel() {
	int c;
	while((c = getchar()) != '\n' && c != EOF);
	printf("Az ajandek kero nevet:\n");
	char nev[128];
	//scanf("%s", nev);
	scanf("%127[^\n]",nev);

	printf("A kivant ajandek szama: \n\t1 - baba\n\t2 - auto\n\t3 - labda\n\t4 - kirako\n\t5 - szinezo\n\t6 - konyv\n");
	int ajandekId = -1;
	scanf("%d", &ajandekId);

	printf("Add meg a varos nevet:\n");
	char varos[128];
	scanf("%s", varos);

	//int c;
	while((c = getchar()) != '\n' && c != EOF);

	printf("Add meg a cimet (utca, hazszam):\n");
	char cim[512];
	//char *cim = malloc (512);
	//scanf("%s", cim); //nem erzekeli a szokozoket
	scanf("%511[^\n]",cim);
	/*fgets (cim, 512, stdin);
	if ((strlen(cim)>0) && (cim[strlen (cim) - 1] == '\n'))
		cim[strlen (cim) - 1] = '\0';*/

	ujLevelFromParam(nev, ajandekId, varos, cim);
}

void listaBovites() {
	int regiMaxMeret = maxMeret;
	maxMeret *= 2;

	struct level* bLista[maxMeret];
	int i;
	for( i = 0; i < meret+1; i++ ) {
		bLista[i] = kivansagLista[i];
	}

	kivansagLista = bLista;
}

void levelIras() {
	if( meret+1 >= maxMeret ) {
		listaBovites();
	}
	ujLevel();
}

void listaKiiras() {
	if( meret == 0 ) {
		printf("Meg nem erkezett kivansag!");
		return;
	}

	printf("\n\nA teljes lista:\n");
	int i;
	for(i = 0; i < meret; i++) {
		kiir( kivansagLista[i], i);
	}
	printf("Az osszes keres: %d\n", meret);

	int menu = 0;
	//while ( menu != 4 ) {
		printf("\n\nNyomd meg a megfelelo szamot az alabbi lehetosegek valasztasahoz:\n");
		printf("\t1: Szures telepules alapjan\n");
		printf("\t2: Szures ajendek alapjan\n");
		printf("\t3: Megse\n");
		
		int beolv = scanf("%d", &menu);
		if( beolv != 1 ) {
			//menu = 4;
			printf("Hibas megadas!\n");
			//continue;
			return;
		}

		switch(menu) {
			case 1:
				printf("Add meg a varost amire szurjunk:\n");
				char tel_felt[128];
				int cnt = 0;
				scanf("%s", tel_felt);
				for( i = 0; i < meret ; i++) {
					if( strcmp(kivansagLista[i]->varos, tel_felt) == 0 ) {
						kiir(kivansagLista[i], i);
						cnt++;
					}
				}
				printf("Az osszes keres %s-rol: %d\n", tel_felt, cnt);
				break;
			case 2:
				printf("Add meg az ajandek szamat amire szurjunk:\n\t1 - baba\n\t2 - auto\n\t3 - labda\n\t4 - kirako\n\t5 - szinezo\n\t6 - konyv\n");
				int id;
				cnt = 0;
				scanf("%d",&id);
				for( i = 0; i < meret; i++) {
					if( kivansagLista[i]->ajandekId == id ) {
						kiir(kivansagLista[i], i);
						cnt++;
					}
				}
				printf("Az osszes keres a(z) %s tipusu ajandekbol: %d\n", ajandek[id], cnt);
				break;
			default:
				break;
		}

}

void kiir(struct level* kivansag, int i) {
	struct tm* ido = malloc(sizeof(struct tm));
	localtime_r(&kivansag->beerkezes, ido);
	printf("\t%d. %s %s %s %s %d %d %d %d:%d\n", i+1, kivansag->nev, ajandek[kivansag->ajandekId-1], kivansag->varos, kivansag->cim, ido->tm_year+1900, ido->tm_mon+1, ido->tm_mday, ido->tm_hour, ido->tm_min);
	free(ido);
}

void listaMentese(char* filename) {
	printf("Az adat fajl neve: %s\n", filename);
	/*int f;
	f = open(filename, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);*/
	FILE* f;
	f = fopen(filename, "w");
	if( f == NULL ) {
		perror("Hiba a fajl megnyitasaban!\n");
		//free(filename);
		return;
	}

	fprintf(f, "Nev;\tAjandek;\tVaros;\tCim;\tDatum;\n");
	struct tm* ido = malloc(sizeof(struct tm));
	int i;
	for( i = 0; i < meret; i++) {
		localtime_r(&(kivansagLista[i]->beerkezes), ido);
		//struct tm* timeinfo = akt[i]->ido;
		fprintf(f, "%s;\t%d;\t%s;\t%s;\t%d;\t%d;\t%d\t%d\t%d\n", kivansagLista[i]->nev, kivansagLista[i]->ajandekId, kivansagLista[i]->varos, kivansagLista[i]->cim, ido->tm_year+1900, ido->tm_mon+1, ido->tm_mday, ido->tm_hour, ido->tm_min);
	}
	free(ido);

	close(1);
}

