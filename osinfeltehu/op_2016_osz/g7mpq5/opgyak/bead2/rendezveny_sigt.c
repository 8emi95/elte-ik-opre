/*--------------------------------------------------------------------------------------------
2. beadandó feladat. Beadási határidő: 2016-12-11.

A "King of Stands" rendezvény szervező társaság, amint a rendezvény ideje eljön, megbízza egy partnerét(gyermekét) az esemény lebonyolítására. A gyermekfolyamat indulása után csővezetéken megkapja, hogy hol kerül a rendezvény lebonyolításra! A helyszínre utazik, majd visszajelzi (jelzést küld vissza) a központnak, hogy a megadott helyszinen van, várja a résztvevőket,kész a rendezvény lebonyolítására. Ezután a rendezvényszervező csővezetéken elküldi a helyszínen tartózkodó partnernek, hogy kik a résztvevők. Ezt a partner képernyőre írja, majd várja a vendégeket és levezényli a rendezvényt! A rendezvény végén jelentést küld vissza, amiben tájékoztatja a főszervezőt, hogy mennyire sikeres volt a rendezvény (véletlenszám), illetve a jelentkezettek közül végül is ki nem érkezett meg! (Minden résztvevő 10% valószínűséggel nem érkezik meg!) Miután egy esemény lebonyolításra került, "King of Stands" újra megvizsgálja, melyik rendezvény a következő!
----------------------------------------------------------------------------------------------*/

// TODO meg egy csovet a masik iranyba, s jelzesekkel iranyitani


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#include <signal.h>

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
 char helyszin[128];
 time_t kezdes;
 //int resztvevok;
 //struct latogatok** latogatoLista;
};

void rendezvenyMenu();
struct rendezveny** rendezvenyLista;
int maxRend = 20;
int rv_meret = 0;
struct rendezveny* ujRendezveny();
struct rendezveny* ujRendezvenyFromParam(char* nev, int rendezvenyId, char* helyszin);
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


void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  //signal(SIGTERM, SIG_DFL);
  //raise(SIGTERM); //sajat maganak kuld jelyest???
}


int main(int argc, char** argv)
{
	signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), handler = SIG_DFL - back to default behavior 
	struct rendezveny* rLista[maxRend];
	rendezvenyLista = rLista;
	struct latogato* aLista[maxMeret];
	latogatoLista = aLista;
	int indul = 0;

	if( argc > 1 ) {
		char* inFile;
		inFile = (char*)malloc((strlen(argv[1])+1)*sizeof(char));
		strcpy(inFile, argv[1]);
		listaOlvasasaBin(inFile);
		//listaOlvasasa("kezdo2.txt");
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
		
		time_t now;
		time(&now);
    int akt;
		for(akt = 0; akt < rv_meret; akt++)
			{
				if( rendezvenyLista[akt]->kezdes <= now )
					{
						indul = 1;
						break;
					}
			}
		if( !indul )
		 continue;
		indul = 0;
		 

		//** csovezetek
		int pipefd[2]; // unnamed pipe file descriptor array
		char helyszinMsg[512];  // char array for reading from pipe
		if ( pipe(pipefd) == - 1 ) {
			perror("Hiba a pipe nyitasakor!");
			exit(EXIT_FAILURE);
		}
  
		//** uzenetsor
		/*int uzenetsor, msgStatus; 
		key_t kulcs; 
  
		// msgget needs a key, amelyet az ftok generál 
		//. 
		kulcs = ftok(argv[0],1);  // 2. par hanyadik kulcsot hozzuk letre, ftok egyedi kulcs
		//printf ("A kulcs: %d\n",kulcs);
		uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );  // IPC_PRIVATE
		if ( uzenetsor < 0 ) { 
			perror("msgget"); 
			return 1; 
		} */
  
		//** fork
		////srand(time(NULL));
		int status;
		//printf("Before forking \n");
		pid_t  child=fork(); //forks make a copy of variables
		if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
  
		// King of Stands
		// Partnert létrehoz, s csövön keresztül városba küldi, visszjelzés után csővezetéken küldi a résztvevőket, teljes
		if (child>0) //the parent process, it can see the returning value of fork - the child variable!
		{
			close(pipefd[0]); //Usually we close unused read end
			write(pipefd[1], rendezvenyLista[akt]->helyszin, strlen(rendezvenyLista[akt]->helyszin)+1);
			close(pipefd[1]); // Closing write descriptor
			fflush(NULL); 	// flushes all write buffers (not necessary)

    	pause(); //waits till a signal arrive 

		}
		// Partner
		// Városba utazik, s vissza jelez signallal, kiirja a resztvevok listajat, vhogy vissza kuldi, h vegul ki ment el (10%, h vki nem)
		else //child process
		{
			// fogad
			printf("The child process \n");
			close(pipefd[1]);  //Usually we close the unused write end
			read(pipefd[0],helyszinMsg,sizeof(helyszinMsg)); // reading max 100 chars //a read var addig mig a masik ir bele, ez egy szinkronizacios eszkoz
			printf("Gyerek olvasta uzenet: %s", helyszinMsg);
			printf("\n");
			close(pipefd[0]); // finally we close the used read end

    	kill(getppid(),SIGTERM); 

			rendezvenyTorlese(akt);
			return 0;
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

	printf("A rendezveny helyszine:\n");
	char helyszin[128];
	scanf(" %127[^\n]",helyszin);

	struct rendezveny* akt = ujRendezvenyFromParam(nev, rendezvenyId, helyszin);
	printf("A rendezveny időpontja (formátum: YYYY MM dd hh:mm): ");
	int ev, honap, nap, ora, perc;
	scanf("%d %d %d %d:%d", &ev, &honap, &nap, &ora, &perc);

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
	akt->kezdes = mktime(timeinfo);

	return akt;
}

struct rendezveny* ujRendezvenyFromParam(char* nev, int rendezvenyId, char* helyszin) {
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
	strcpy(akt->helyszin, strdup(helyszin));

	time(&akt->kezdes);

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
		struct tm* ido = malloc(sizeof(struct tm));
		localtime_r(&program->kezdes, ido);
		//printf("\t%d. %s %d %d\n", i+1, program->nev, program->rendezvenyId, program->resztvevok);
		printf("\t%d. %s %d %s %d %d %d %d:%d\n", i+1, program->nev, program->rendezvenyId, program->helyszin, ido->tm_year+1900, ido->tm_mon+1, ido->tm_mday, ido->tm_hour, ido->tm_min);
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
	printf("\t%d. %s %s %d %d %d %d %d %d:%d\n", i+1, vendeg->nev, vendeg->mail, vendeg->rendezvenyId, vendeg->sorszam, ido->tm_year+1900, ido->tm_mon+1, ido->tm_mday, ido->tm_hour, ido->tm_min);
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
	//fprintf(f, "Nev;\tRendezveny azonosito;\tHelyszin;\n");
	struct tm* ido = malloc(sizeof(struct tm));
	int l;
	for( l = 0; l < rv_meret; l++) {
		localtime_r(&(rendezvenyLista[l]->kezdes), ido);
		fprintf(f, "%s; %d; %s; %d; %d; %d; %d:%d;\n", rendezvenyLista[l]->nev, rendezvenyLista[l]->rendezvenyId, rendezvenyLista[l]->helyszin, ido->tm_year+1900, ido->tm_mon+1, ido->tm_mday, ido->tm_hour, ido->tm_min);
	}
	fprintf(f, "}\n");

	fprintf(f, "\n");

	fprintf(f, "Latogatok:\n");
	fprintf(f, "{\n");
	//fprintf(f, "Nev;\tE-mail;\tRendezveny azonosito;\tSorszam;\tDatum;\n");
	//struct tm* ido = malloc(sizeof(struct tm));
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
		char helyszin[128];
		int sorszam;
		int ev, honap, nap, ora, perc;
		sscanf(line, "%[^;]; %d; %[^;]; %d; %d; %d; %d:%d;\n", &nev, &rendezvenyId, &helyszin, &ev, &honap, &nap, &ora, &perc);

		struct rendezveny* akt = ujRendezvenyFromParam(nev, rendezvenyId, helyszin);
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
		akt->kezdes = mktime(timeinfo);
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
		ujRendezvenyFromParam("",0,"");
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
