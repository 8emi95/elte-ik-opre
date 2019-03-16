/*--------------------------------------------------------------------------------------------
Készítsen C programot, amely rendszerhívások alkalmazásával megoldja a következő feladatot:

Egy kiállításokat, rendezvényeket szervező társaság a "King of Stands", elektronikus látogatói nyilvántartást készített. A látogatók a nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, a rendezvény azonosító számát! Az elektronikus rendszer a jelentkezés nyugtázásaként visszaadja, hogy hányadik vendégként történt meg a regisztráció! Az alkalmazás adjon lehetőséget vendég jelentkezéshez, a jelentkezettek listázásához, egy vendég adatainak módosításához, törléséhez. Legyen lehetőségünk egy új rendezvény indításához, egy rendezvény összes adatának törléséhez! A jelentkezőknél a korábbi adatok mellett rögzítsük automatikusan a jelentkezési időt(rendszeridő) is(ezt nem kell bekérni)!

Készítsünk C programot, amellyel a leírt tevékenység elvégezhető. Az adatokat fájlban tároljuk. 
----------------------------------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

struct latogato {
 char nev[128];
 char mail[128];
 int rendezvenyId;
 int sorszam;
 time_t beerkezes;
};

struct rendezveny {
 char nev[128];
 int rendezvenyId;
 //int resztvevok;
 //struct latogatok** latogatoLista;
};

void rendezvenyMenu();
struct rendezveny** rendezvenyLista;
int maxRend = 20;
int rv_meret = 0;
struct rendezveny* ujRendezveny();
struct rendezveny* ujRendezvenyFromParam(char* nev, int rendezvenyId);
void rendBovites();
void rendezvenyKiirasa();
void rendezvenyTorlese(int num);

void latogatoMenu();
struct latogato** latogatoLista;
int maxMeret = 20;
int meret = 0;
int sorszam = 0;
struct latogato* ujLatogato();
struct latogato* ujLatogatoFromParam(char* nev, char* mail, int rendezvenyId);
void listaBovites();
struct latogato* latogatoFelvetele();
void latogatokKiirasa();
void kiirLatogatok(struct latogato* vendeg, int i);
void latogatoModositasa(int num);
void latogatoTorlese(int num);

void listaMentese(char* filename);
void listaOlvasasa(char* filename);
void listaMenteseBin(char* filename);
void listaOlvasasaBin(char* filename);

int main(int argc, char** argv)
{
	struct rendezveny* rLista[maxRend];
	rendezvenyLista = rLista;
	struct latogato* aLista[maxMeret];
	latogatoLista = aLista;

	if( argc > 1 ) {
		char* inFile;
		inFile = (char*)malloc((strlen(argv[1])+1)*sizeof(char));
		strcpy(inFile, argv[1]);
		listaOlvasasaBin(inFile);
		//listaOlvasasa("kezdo.txt");
	}

	int menu = 0;
	//while ( menu < 4 ) 
	while ( 1 ) {
		printf("\n\nNyomd meg a megfelelo szamot az alabbi lehetosegek valasztasahoz:\n");
		printf("\t1: Rendezvenyek\n");
		printf("\t2: Latogatok\n");
		printf("\t3: Kilepes es mentes\n");
		printf("\t4: Kilepes\n");
		
		int beolv = scanf("%d", &menu);
		if( beolv != 1 ) {
			menu = 5;
			printf("Hibas megadas!\n");
			//continue;
		}

		char* filename;
		switch(menu) {
			case 1:
				rendezvenyMenu();
				break;
			case 2:
				latogatoMenu();
				break;
			case 3:
				// Lehet ures file is
				//if( meret != 0 || rv_meret != 0 ) {
					//char* filename;
					if( argc > 2 ) {
						filename = (char*)malloc((strlen(argv[2])+1)*sizeof(char));
						strcpy(filename, argv[2]);
					}
					else {
						//printf("Az adat fajl neve az elso parameterben megadhato!\n");
						filename = (char*)malloc((strlen("king_of_stands.txt")+1)*sizeof(char));
						strcpy(filename, "king_of_stands.txt");
					}

					listaMentese(filename);
					listaMenteseBin("king_of_stands");
					free(filename);
				//}
			default:
				if( meret != 0 ) {
					int i;
					for( i = meret-1; i >= 0; i--) {
						struct latogato* tmp = latogatoLista[i];
						free(tmp);
					}
				}
				return 0;
				break;
		}
	}

	if( rv_meret != 0 ) {
		int l;
		for( l = meret-l; l >= 0; l--) {
			struct rendezveny* tmp = rendezvenyLista[l];
			free(tmp);
		}
	}
	if( meret != 0 ) {
		int i;
		for( i = meret-1; i >= 0; i--) {
			struct latogato* tmp = latogatoLista[i];
			free(tmp);
		}
	}
	return 0;
}

void rendezvenyMenu() {
	int menu = 0;

	printf("\n\nNyomd meg a megfelelo szamot az alabbi lehetosegek valasztasahoz:\n");
	printf("\t1: Uj rendezveny\n");
	printf("\t2: Rendezvenyek listazasa\n"); // rajta leveo latogatok is???
	printf("\t3: Rendezveny torlese\n");
	printf("\t4: Vissza\n");
	
	int beolv = scanf("%d", &menu);
	if( beolv != 1 ) {
		printf("Hibas megadas!\n");
		return;
	}

	switch(menu) {
		case 1:
			ujRendezveny();
			break;
		case 2:
			rendezvenyKiirasa();
			break;
		case 3:
			rendezvenyKiirasa();
			printf("Melyik sorszamu rendezvenyt toroljuk?\n");

			int num;
			if( scanf("%d", &num) != 1 ) {
				return;
			}
			num--;
			if( num < rv_meret)
			rendezvenyTorlese(num);
			break;
		default:
			return;
	}
}

struct rendezveny*  ujRendezveny() {
	if( rv_meret+1 >= maxRend ) {
		rendBovites();
	}

	int c;
	while((c = getchar()) != '\n' && c != EOF);
	printf("A rendezveny neve:\n");
	char nev[128];
	//scanf("%s", nev);
	scanf(" %127[^\n]",nev);

	printf("A rendezveny azonositoja: ");
	int rendezvenyId = -1;
	scanf("%d", &rendezvenyId);

	return ujRendezvenyFromParam(nev, rendezvenyId);
}

struct rendezveny* ujRendezvenyFromParam(char* nev, int rendezvenyId) {
	struct rendezveny* akt;
	akt = NULL;

	int i = 0;
	for( i = 0; i < rv_meret; i++) {
		if( strcmp(rendezvenyLista[i]->nev, nev) == 0 ) {
			akt = rendezvenyLista[i];
		}
	}
	if( akt == NULL ) {
		akt = malloc(sizeof(struct rendezveny));
		rendezvenyLista[rv_meret] = akt;
		rv_meret++;
	}

	strcpy(akt->nev, strdup(nev));
	akt->rendezvenyId = rendezvenyId;

	//akt->resztvevok = 0;
	return akt;
}

void rendBovites() {
	int regMaxMeret = maxRend;
	maxRend *= 2;

	struct rendezveny* bLista[maxRend];
	int i;
	for( i = 0; i < rv_meret+1; i++ ) {
		bLista[i] = rendezvenyLista[i];
	}

	rendezvenyLista = bLista;
}

void rendezvenyKiirasa() {
	if( rv_meret == 0 ) {
		printf("Nincs meg rendezveny!");
		return;
	}

	printf("\n\nRendezvenyek:\n");
	int i;
	for(i = 0; i < rv_meret; i++) {
		struct rendezveny* program = rendezvenyLista[i];
		//printf("\t%d. %s %d %d\n", i+1, program->nev, program->rendezvenyId, program->resztvevok);
		printf("\t%d. %s %d\n", i+1, program->nev, program->rendezvenyId);
	}
}

void rendezvenyTorlese(int num) {
	struct rendezveny* program = rendezvenyLista[num];
	//free(program->nev);
	free(program);

	int i;
	for( i = num; i < rv_meret-1; i++)
		rendezvenyLista[i] = rendezvenyLista[i+1];

	rv_meret--;
	//rendezvenyLista = realloc(rendezvenyLista, rv_meret*sizeof(struct rendezveny*));
}

void latogatoMenu() {
	int menu = 0;

	printf("\n\nNyomd meg a megfelelo szamot az alabbi lehetosegek valasztasahoz:\n");
	printf("\t1: Uj Latogato\n");
	printf("\t2: Latogatok kilistazasa\n");
	printf("\t3: Latogato modositas\n");
	printf("\t4: Latogato torlese\n");
	printf("\t5: Vissza\n");
	
	int beolv = scanf("%d", &menu);
	if( beolv != 1 ) {
		printf("Hibas megadas!\n");
		return;
	}

	switch(menu) {
		case 1:
			latogatoFelvetele();
			break;
		case 2:
				latogatokKiirasa();
			break;
		case 3:
			latogatokKiirasa();
			printf("Melyik sorszamu latogatot modositsuk?\n");

			int num;
			if( scanf("%d", &num) != 1 ) {
				return;
			}
			num--;
			if(num < meret)
				latogatoModositasa(num);
			break;
		case 4:
			latogatokKiirasa();
			printf("Melyik sorszamu latogatot toroljuk?\n");

			int num2;
			if( scanf("%d", &num2) != 1 ) {
				return;
			}
			num2--;
			if(num2 < meret)
				latogatoTorlese(num2);
			break;
		default:
			return;
	}
}

struct latogato* ujLatogatoFromParam(char* nev, char *mail, int rendezvenyId) {
	struct latogato* akt;
	akt = NULL;

	int i = 0;
	for( i = 0; i < meret; i++) {
		if( strcmp(latogatoLista[i]->nev, nev) == 0 ) {
			akt = latogatoLista[i];
		}
	}
	if( akt == NULL ) {
		akt = malloc(sizeof(struct latogato));
		latogatoLista[meret] = akt;
		meret++;
	}

	//akt->nev = strdup(nev);
	strcpy(akt->nev, strdup(nev));
	strcpy(akt->mail, strdup(mail));
	akt->rendezvenyId = rendezvenyId;

	/*time_t current_raw;
	struct tm*	ido = malloc(sizeof(struct tm));
	time(&current_raw);
	localtime_r(&current_raw, ido);
	akt->ido = ido;*/
	time(&akt->beerkezes);

	sorszam++;
	akt->sorszam = sorszam;
	printf("%d. vendegent tortnet a regisztracio!\n", sorszam);
	return akt;
}

struct latogato* ujLatogato() {
	int c;
	while((c = getchar()) != '\n' && c != EOF);
	printf("A latogato neve:\n");
	char nev[128];
	//scanf("%s", nev);
	scanf("%127[^\n]",nev);

	printf("Latogato e-mail cime:\n");
	char mail[128];
	scanf("%s", mail);

	printf("A rendezveny azonositoja: ");
	printf("A letezo rendezvenyek:");
	int i;
	for(i=0; i < rv_meret; i++ )
		printf(" %d;", rendezvenyLista[i]->rendezvenyId);
	printf("\n");
	int rendezvenyId = -1;
	scanf("%d", &rendezvenyId);

	return ujLatogatoFromParam(nev, mail, rendezvenyId);
}

void listaBovites() {
	int regiMaxMeret = maxMeret;
	maxMeret *= 2;

	struct latogato* bLista[maxMeret];
	int i;
	for( i = 0; i < meret+1; i++ ) {
		bLista[i] = latogatoLista[i];
	}

	latogatoLista = bLista;
}

struct latogato* latogatoFelvetele() {
	if( meret+1 >= maxMeret ) {
		listaBovites();
	}
	return ujLatogato();
}

void latogatokKiirasa() {
	if( meret == 0 ) {
		printf("Meg nincs latogatok!");
		return;
	}

	printf("\n\nA teljes lista:\n");
	int i;
	for(i = 0; i < meret; i++) {
		kiirLatogatok( latogatoLista[i], i);
	}
	printf("A latogatok szama: %d\n", meret);
}

void kiirLatogatok(struct latogato* vendeg, int i) {
	struct tm* ido = malloc(sizeof(struct tm));
	localtime_r(&vendeg->beerkezes, ido);
	printf("\t%d. %s %s %d %d %d %d %d:%d\n", i+1, vendeg->nev, vendeg->mail, vendeg->rendezvenyId, vendeg->sorszam, ido->tm_year+1900, ido->tm_mon+1, ido->tm_mday, ido->tm_hour, ido->tm_min);
	free(ido);
}

void latogatoModositasa(int num) {
	struct latogato* vendeg = latogatoLista[num];
	kiirLatogatok(vendeg, num);

	printf("Melyik sorszamu parametert modositsuk?\n1. Nev\t2. E-mail cim\t3. Rendezveny azonosito \n");

	int menu;
	if( scanf("%d", &menu) != 1 ) {
		return;
	}

	switch(menu) {
		case 1:
			printf("Add meg az uj nevet!\n");
			char nev[128];
			scanf(" %127[^\n]",nev);
			strcpy(vendeg->nev, strdup(nev));
			break;
		case 2:
			printf("Add meg az uj e-mail cimet!\n");
			char mail[128];
			scanf("%s", mail);
			strcpy(vendeg->mail, strdup(mail));
			break;
		case 3:
			printf("Add meg az uj rendezveny szamot!\n");
			int rendezvenyId = -1;
			scanf("%d", &rendezvenyId);
			vendeg->rendezvenyId = rendezvenyId;
			break;
	}
}

void latogatoTorlese(int num) {
	struct latogato* vendeg = latogatoLista[num];
	//free(program->nev);
	free(vendeg);

	int i;
	for( i = num; i < meret-1; i++)
		latogatoLista[i] = latogatoLista[i+1];

	meret--;
	//latogatoLista = realloc(latogatoLista, meret*sizeof(struct latogato*));
}

void listaMentese(char* filename) {
	printf("Az adat fajl neve: %s\n", filename);
	FILE* f;
	f = fopen(filename, "w");
	if( f == NULL ) {
		perror("Hiba a fajl megnyitasaban!\n");
		//free(filename);
		return;
	}

	fprintf(f, "Rendezvenyek:\n");
	fprintf(f, "{\n");
	//fprintf(f, "Nev;\tRendezveny azonosito;\n");
	int l;
	for( l = 0; l < rv_meret; l++) {
		fprintf(f, "%s; %d;\n", rendezvenyLista[l]->nev, rendezvenyLista[l]->rendezvenyId);
	}
	fprintf(f, "}\n");

	fprintf(f, "\n");

	fprintf(f, "Latogatok:\n");
	fprintf(f, "{\n");
	//fprintf(f, "Nev;\tE-mail;\tRendezveny azonosito;\tSorszam;\tDatum;\n");
	struct tm* ido = malloc(sizeof(struct tm));
	int i;
	for( i = 0; i < meret; i++) {
		localtime_r(&(latogatoLista[i]->beerkezes), ido);
		//struct tm* timeinfo = akt[i]->ido;
		fprintf(f, "%s; %s; %d; %d; %d; %d; %d; %d:%d;\n", latogatoLista[i]->nev, latogatoLista[i]->mail, latogatoLista[i]->rendezvenyId, latogatoLista[i]->sorszam, ido->tm_year+1900, ido->tm_mon+1, ido->tm_mday, ido->tm_hour, ido->tm_min);
	}
	fprintf(f, "}\n");
	free(ido);

	close(1);
}

void listaOlvasasa(char* filename) {
	FILE* f;
	f = fopen(filename, "r");
	if( f == NULL ) {
		perror("Hiba a fajl megnyitasaban!\n");
		//free(filename);
		return;
	}

	size_t maxLineLength = 1024;
	char* line = (char*)malloc(maxLineLength*sizeof(char));
	// Skip the first two (header) row
	getline(&line, &maxLineLength, f);
	getline(&line, &maxLineLength, f);
	while( getline(&line, &maxLineLength, f) != -1 && line[0] != '}') { // include the newline character
		char nev[128];
		int rendezvenyId;
		int sorszam;
		sscanf(line, "%[^;]; %d;\n", &nev, &rendezvenyId);		
		ujRendezvenyFromParam(nev, rendezvenyId);
	}

	getline(&line, &maxLineLength, f);

	// Skip the first two (header) row
	getline(&line, &maxLineLength, f);
	getline(&line, &maxLineLength, f);
	while( getline(&line, &maxLineLength, f) != -1 && line[0] != '}') { // include the newline character
		char nev[128];
		char mail[128];
		int rendezvenyId;
		int sorszam;
		int ev, honap, nap, ora, perc;
		sscanf(line, "%[^;]; %[^;]; %d; %d; %d; %d; %d; %d:%d;\n", &nev, &mail, &rendezvenyId, &sorszam, &ev, &honap, &nap, &ora, &perc);		

		struct latogato* akt = ujLatogatoFromParam(nev, mail, rendezvenyId);
		//struct latogato* akt = latogatoLista[meret-1];

		// time_t from struct_m
		time_t rawtime;
		time ( &rawtime );
		struct tm * timeinfo;
		timeinfo = localtime ( &rawtime );
		timeinfo->tm_year = ev - 1900;
		timeinfo->tm_mon = honap - 1;
		timeinfo->tm_mday = nap;
		timeinfo->tm_hour = ora;
		timeinfo->tm_min = perc;
		akt->beerkezes = mktime(timeinfo);
	}

	free(line);
}

void listaMenteseBin(char* filename) {
	printf("Az binaris fajl neve: %s\n", filename);
	int f;
	f = open(filename, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	if (f<0){ perror("Error at opening the file\n");exit(1);}

	//const char rend[] = "Rendezvenyek:{";
	//write(f, rend, sizeof(rend));
	write(f, &rv_meret, sizeof(int));
	int l;
	for( l = 0; l < rv_meret; l++) {
		write(f, rendezvenyLista[l], sizeof(*rendezvenyLista[l]));
	}
	//const char end[] = "}\n";
	//write(f, end, sizeof(end));

	//const char lat[] = "Latogatok:{";
	//write(f, lat, sizeof(lat));
	write(f, &meret, sizeof(int));
	int i;
	for( i = 0; i < meret; i++) {
		write(f, latogatoLista[i], sizeof(*latogatoLista[i]));
	}
	//write(f, end, sizeof(end));
	close(f);
}

void listaOlvasasaBin(char* filename) {
	//printf("Az binaris fajl neve: %s\n", filename);
	int f;
	f = open(filename, O_RDONLY);
	if (f<0){ perror("Error at opening the file\n");exit(1);}

	int uj_rv_meret;
	read(f, &uj_rv_meret, sizeof(int));
	int l;
	for(l = 0; l < uj_rv_meret; l++) {
		ujRendezvenyFromParam("",0);
		read(f, rendezvenyLista[l], sizeof(*rendezvenyLista[l]));
	}

	int uj_meret;
	read(f, &uj_meret, sizeof(int));
	int i;
	for(i = 0; i < uj_meret; i++) {
		ujLatogatoFromParam("","",0);
		read(f, latogatoLista[i], sizeof(*latogatoLista[i]));
	}

	close(f);
}
