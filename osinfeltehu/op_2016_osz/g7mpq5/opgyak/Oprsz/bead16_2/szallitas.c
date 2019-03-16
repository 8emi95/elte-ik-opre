/*
2. beadandó feladat. Beadási határidő: 2015 december 1. Bemutatási határidő: 2015 december 8.

Lassan itt a december, nem elég gyűjteni az ajándékokat, ki is kell szállítani!

A manók felkészítik a rénszarvas szánokat (gyerek folyamat) az ajándékok kiszállítására.
Télapu (szülő folyamat) az ajándékokat gyűjti, majd kívánság szerint kiszállít egy-egy adagot!
Télapu kétféle csoportosítással tud szállítani, vagy az ajándék gyűjtő helyről egy-egy városba indít egy rénszarvas szállítmányt, vagy az ajándék üzemekből indítja azokat!

Előbbi esetben egy rénszarvasszán egy városba viszi az összes ajándékot, második esetben a rénszarvasszán egyféle ajándékot visz a megadott címekre!
A Télapu csövön keresztül tudatja a szánnal, hogy melyik városba kell az összes ajándékot vinni vagy melyik ajándékot kell szállítani!

A manó a szállítás elvégzéséről üzenetsorban tájékoztatja télaput,aki bejegyzi, hogy melyik városba vagy melyik lett az ajándék kiszállítva!
Város szerinti vagy ajándék szerinti szállítás esetén tudjuk kiválasztani a várost, vagy az ajándékot!
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork, for pipe()
#include <sys/wait.h> //waitpid
#include <errno.h> 
#include <sys/time.h>
//ipcs parancs
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <string.h> 
#include <wait.h> 

#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#include <signal.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  //signal(signumber,SIG_DFL);
  //raise(signumber);
  //signal(signumber,SIG_IGN);
}

//** ajandek lista
const char* ajandek[] = {"baba", "auto", "labda", "kirako", "szinezo", "konyv"};

struct level {
 char nev[128];
 int ajandekId;
 char varos[128];
 char cim[512];
 time_t beerkezes;
};

struct level** kivansagLista;
int maxMeret = 100;
int meret = 0;

///void listaBovites();
void listaMentese(char* filename);
void listaBeolvasas(char* filename);

///void levelIras();
void listaKiiras();
void kiir(struct level* kivansag, int i);

///void ujlevel();
void ujlevelFromParam(char* nev, int ajandekId, char* varos, char* cim);

void feljegyez(int type, char *nev);
void kiszallit(int type, char *nev);
void levelTorol(int idx);
void feltolt();
//**


// Telapo
void egyVarosba(int pipefd[2]);
void egyAjandekTipust(int pipefd[2]);


//** uzenetsor
struct manoUzenet { 
	long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
	char mtext [ 1024 ]; 
};

// sendig a message
int manoKuld( int uzenetsor, char kiszallitva[] ) 
{ 
		//const struct manoUzenet uz = { 5, "Hajra Fradi!" }; // 1. uzenet tipusa
		struct manoUzenet uz;
		//uz.mtype = 5;
		//strcpy(uz.mtext, kiszallitva);
		//sscanf(kiszallitva,"%s %d", uz.mtext, &uz.mtype);
		long tmpType;
		sscanf(kiszallitva,"%s %ld", uz.mtext, &tmpType);
		uz.mtype = tmpType;
		printf("pelda %ld %ld %s\n\n", tmpType, uz.mtype, uz.mtext);

		int msgStatus; 
		msgStatus = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); // 1. azon, 2. uzenet cime, 3.. uzenet hossza ,1 nulla karakter is kell, 4.
		// a 3. param ilyen is lehet: sizeof(uz.mtext)
		// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
		if ( msgStatus < 0 ) 
			perror("msgsnd"); 
		return 0; 
} 
     
// receiving a message. 
int manoFogad( int uzenetsor ) 
{
		struct manoUzenet uz; 
		int msgStatus; 
		// az utolso parameter(0) az uzenet azonositoszama
		// ha az 0, akkor a sor elso uzenetet vesszuk ki
		// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
		// vesszuk ki a sorbol 
		//msgStatus = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); // ha 5 helyet 0, mindenfele uzenetet beolvas
		msgStatus = msgrcv(uzenetsor, &uz, 1024, 0, 0 ); // ha 5 helyet 0, mindenfele uzenetet beolvas

		
		//printf("fogad\n");
		//printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
		if ( msgStatus < 0 ) 
			perror("msgsnd"); 
		else
			//printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
			if( uz.mtype == 5 ) {
				printf( "Az ajandekok %s varosaba lettek kiszallitva!\n", uz.mtext ); 
			}
			else if( uz.mtype == 6 ) {
				printf( "A(z) %s ajandekok lettek kiszallitva!\n", uz.mtext ); 
			}
			else {
				printf( "Ismeretlen uzenet kod: %ld\n", uz.mtype ); 
			}
		kiszallit(uz.mtype, uz.mtext);

		///kill(chPid, SIGTERM);
		listaKiiras();
		return 0; 
} 


int main(int argc, char* argv[])
{
	signal(SIGTERM, handler);

	// ajandek lista feltoltese
	struct level* aLista[maxMeret];
	kivansagLista = aLista;
	feltolt();
	listaKiiras();

	int menu = 0;
	while ( menu < 3 ) { 
	//while ( 1 )
			printf("\n\nNyomd meg a megfelelo szamot az alabbi lehetosegek valasztasahoz:\n");
			//printf("\t1: az ajándék gyűjtő helyről egy-egy városba indít egy rénszarvas szállítmányt\n"); // egy rénszarvasszán egy városba viszi az összes ajándékot
			// melyik városba kell az összes ajándékot vinni
			printf("\t1: Melyik varosba szallitsunk?\n");
			//printf("\t2: az ajándék üzemekből indítja azokat\n"); // második esetben a rénszarvasszán egyféle ajándékot visz a megadott címekre
			// melyik ajándékot kell szállítani
			printf("\t2: Melyik ajandekot szallitsuk?\n"); // második esetben a rénszarvasszán egyféle ajándékot visz a megadott címekre
			printf("\t3: Kilepes\n");
			
			int beolv = scanf("%d", &menu);
			if( beolv != 1 || menu > 2) {
				menu = 3;
				//printf("Hibas megadas!\n");
				printf("Kilep\n");
				continue;
			}

		//** csovezetek
		int pipefd[2]; // unnamed pipe file descriptor array
		char telapoMsg[512];  // char array for reading from pipe
		if ( pipe(pipefd) == - 1 ) {
			perror("Hiba a pipe nyitasakor!");
			exit(EXIT_FAILURE);
		}
  
		//** uzenetsor
		int uzenetsor, msgStatus; 
		key_t kulcs; 
  
		// msgget needs a key, amelyet az ftok generál 
		//. 
		kulcs = ftok(argv[0],1);  // 2. par hanyadik kulcsot hozzuk letre, ftok egyedi kulcs
		//printf ("A kulcs: %d\n",kulcs);
		uzenetsor = msgget( kulcs, 0600 | IPC_CREAT );  // IPC_PRIVATE
		if ( uzenetsor < 0 ) { 
			perror("msgget"); 
			return 1; 
		} 
  
		//** fork
		////srand(time(NULL));
		int status;
		//printf("Before forking \n");
		pid_t  child=fork(); //forks make a copy of variables
		if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
  
		// Város szerinti vagy ajándék szerinti szállítás esetén tudjuk kiválasztani a várost, vagy az ajándékot!
		// telapu
		// A Télapu csövön keresztül tudatja a szánnal
		if (child>0) //the parent process, it can see the returning value of fork - the child variable!
		{
			
			close(pipefd[0]); //Usually we close unused read end
			switch(menu) {
				case 1:
					egyVarosba(pipefd);
					break;
				case 2:
					egyAjandekTipust(pipefd);
					break;
				default:
					printf("\tKilep\n");
					menu = 3;
					close(pipefd[1]); // Closing write descriptor
					fflush(NULL); 	// flushes all write buffers (not necessary)
					waitpid(child,&status,0); 
					continue;
					//return 0;
					break;
			}
			close(pipefd[1]); // Closing write descriptor
			fflush(NULL); 	// flushes all write buffers (not necessary)
  
			// fogad
			pause();
			//printf("megjott\n");
			manoFogad( uzenetsor);
			//listaKiiras(); //itt vmiert nem jo???
  
			waitpid(child,&status,0); 

			//wait( NULL );
			//pause();
			//printf("megjott\n");
			// After terminating child process, the message queue is deleted. 
			msgStatus = msgctl( uzenetsor, IPC_RMID, NULL ); // olvasas iras utan takaritani kell
			if ( msgStatus < 0 ) 
				perror("msgctl");

			//0 means, it really waits for the end of child process - the same as wait(&status)
			//printf("The parent process (remain the original) \n");
		}
  
		// renszarvas szan
		// A manó a szállítás elvégzéséről üzenetsorban tájékoztatja télaput,aki bejegyzi, hogy melyik városba vagy melyik lett az ajándék kiszállítva!
		else //child process
		{
			// fogad
			//printf("The child process \n");
			close(pipefd[1]);  //Usually we close the unused write end
			read(pipefd[0],telapoMsg,sizeof(telapoMsg)); // reading max 100 chars //a read var addig mig a masik ir bele, ez egy szinkronizacios eszkoz
			//printf("Gyerek olvasta uzenet: %s",telapoMsg);
			//printf("\n");
			close(pipefd[0]); // finally we close the used read end
  
			// kuld
			manoKuld( uzenetsor, telapoMsg );  // Child sends a message. 
			kill(getppid(), SIGTERM);

			/*wait( NULL );
			//pause();
			//printf("megjott\n");

			// After terminating child process, the message queue is deleted. 
			msgStatus = msgctl( uzenetsor, IPC_RMID, NULL ); // olvasas iras utan takaritani kell
			if ( msgStatus < 0 ) 
				perror("msgctl"); */
			return 0;
		}
	}

	return 0;
	//exit(EXIT_SUCCESS);	// force exit, not necessary
}


void egyVarosba(int pipefd[2]) {
	printf("Add meg a varost ahova szallitunk:\n");
	char tel_felt[128];
	scanf("%s", tel_felt);
	strcat(tel_felt, " 5"); // ezt lehetne elegansabban is!!
	/*for( i = 0; i < meret ; i++) {
		if( strcmp(kivansagLista[i]->varos, tel_felt) == 0 ) {
			kiir(kivansagLista[i], i);
		}
	}*/

	//write(pipefd[1], "Hajra Fradi!",13);
	write(pipefd[1], tel_felt, strlen(tel_felt)+1);
}


void egyAjandekTipust(int pipefd[2]) {
	printf("Add meg mely ajandekot legyen kiszallitva:\n");
	printf("Ajandekok: baba, auto, labda, kirako, szinezo, konyv\n");
	char ajand[128];
	scanf("%s", ajand);
	strcat(ajand, " 6");

	write(pipefd[1], ajand, strlen(ajand)+1);
}

//** ajandek lista feltoltese
struct level* ujLevelFromParam(char* nev, int ajandekId, char* varos, char* cim) {

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
	return(akt);
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
	printf("\n");
	//printf("Az osszes keres: %d\n", meret);

	/*int menu = 0;
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
		}*/
}

void kiir(struct level* kivansag, int i) {
	struct tm* ido = malloc(sizeof(struct tm));
	localtime_r(&kivansag->beerkezes, ido);
	//printf("\t%d. %s %s %s %s %d %d %d %d:%d\n", i+1, kivansag->nev, ajandek[kivansag->ajandekId-1], kivansag->varos, kivansag->cim, ido->tm_year+1900, ido->tm_mon+1, ido->tm_mday, ido->tm_hour, ido->tm_min);
	printf("\t%s %s %s %s %d %d %d %d:%d\n", kivansag->nev, ajandek[kivansag->ajandekId-1], kivansag->varos, kivansag->cim, ido->tm_year+1900, ido->tm_mon+1, ido->tm_mday, ido->tm_hour, ido->tm_min);
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
		fprintf(f, "%s;\t%d;\t%s;\t%s;\t%d;\t%d;\t;%d;\t%d;\t%d;\n", kivansagLista[i]->nev, kivansagLista[i]->ajandekId, kivansagLista[i]->varos, kivansagLista[i]->cim, ido->tm_year+1900, ido->tm_mon+1, ido->tm_mday, ido->tm_hour, ido->tm_min);
	}
	free(ido);

	close(1);
}

void listaBeolvasas(char* filename) {
	//printf("BEOLVAS\n");
	FILE* f;
	f = fopen(filename, "r");
	if( f == NULL ) {
		perror("Hiba a fajl megnyitasaban!\n");
		//free(filename);
		return;
	}

	size_t maxLineLength = 1024;
	char* line = (char*)malloc(maxLineLength*sizeof(char));
	// Skip the first (header) row
	getline(&line, &maxLineLength, f);
	while( getline(&line, &maxLineLength, f) != -1 ) { // include the newline character
		char nev[128];
		int ajandekId;
		char varos[128];
		char cim[512];
		int ev, honap, nap, ora, perc;
		sscanf(line, "%[^;];\t%d;\t%[^;];\t%[^;];\t%d;\t%d;\t;%d;\t%d;\t%d;\n", &nev, &ajandekId, varos, cim, &ev, &honap, &nap, &ora, &perc);		

		//printf("%s", line);
		//printf("%s;\t%d;\t%s;\t%s;\t%d;\t%d;\t;%d;\t%d;\t%d;\n", nev, ajandekId, varos, cim, ev, honap, nap, ora, perc);
		struct level* akt = ujLevelFromParam(nev, ajandekId, varos, cim);

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

void feltolt() {
	//struct level* aLista[maxMeret];
	//kivansagLista = aLista;
	char* filename;
	filename = (char*)malloc((strlen("ajandek_lista.txt")+1)*sizeof(char));
	strcpy(filename, "ajandek_lista.txt");
	listaBeolvasas(filename);
	free(filename);
}

void feljegyez(int type, char *name) {
}

void kiszallit(int type, char *name) {
	int origMeret = meret;
	int i;
	if( type == 5) {
		for( i = origMeret-1; i >= 0; i--) {
			if( strcmp(kivansagLista[i]->varos, name) == 0 ) {
				levelTorol(i);
			}
		}
	}
	else if( type == 6) {
		for( i = origMeret-1; i >= 0; i--) {
			//printf("\nell: %s %s\n", ajandek[kivansagLista[i]->ajandekId-1], name);
			if( strcmp(ajandek[kivansagLista[i]->ajandekId-1], name) == 0 ) {
				levelTorol(i);
			}
		}
	}
	/*int n;
	printf("\nell:\n");
	for( n = 0; n < meret; n++) {
		kiir(kivansagLista[n], n);
	}*/

	feljegyez(type, name);
}

void levelTorol(int idx) {
	struct level* ujLista[maxMeret];
	int i;
	int l = 0;
	for( i = 0; i < idx; i++) {
		ujLista[l] = kivansagLista[i];
		l++;
	}
	for( i = idx+1; i < meret; i++) {
		ujLista[l] = kivansagLista[i];
		l++;
	}

	struct level* akt = kivansagLista[idx];
	meret--;
	free(akt);
	//kivansagLista = ujLista;

	for( i = 0; i < meret; i++) {
		kivansagLista[i] = ujLista[i];
	}

	/*int n;
	printf("\nell:\n");
	for( n = 0; n < meret; n++) {
		kiir(kivansagLista[n], n);
	}*/
}

