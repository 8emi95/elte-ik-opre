#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>


#define CSONEV "egyedicsor_fz7f3.ftc"

#define FAJL "nyuszi.data"
#define TMP "nyuszi_temp.data"
//
// unnamed pipe example
//
volatile szamlalo = 0;
volatile zarolva = 0;
volatile gyerek_mehet = 0;
int pipefd[2];
char gyerek_azon[32];

struct nyuszi_tipus{
	char megfigyelo_neve[32];
	char telepules_nev[32];
	int nyuszi_db;
	char nyuszi_kor[32];
	time_t megfigyelesi_ido;
};

void handler(int signumber){
  //printf("Signal with number %i has arrived\n",signumber);
  szamlalo++;
  zarolva = 0;
}

void gyerek_handler(int signumber){
	gyerek_mehet = 1;
}

void gyerek_visszahiv(int signumber){
	close(pipefd[0]);
	close(pipefd[1]);
	printf("%s elfaradt, visszahivva, futas megszakitva\n", gyerek_azon);
	exit(0);
}


void gyerek(int idx);
void uj_nyuszi(struct nyuszi_tipus uj);
void fajl_ujrakeszites();

int main(int argc, char *argv[])
{
	
	
	// inic --------------------------------------------------
	int gyerekek_szama = atoi(argv[1]);
	
	int i; // csak ciklus valtozo
	
	unlink(CSONEV);
	
	
	pid_t pid[gyerekek_szama];
	//char sz[100];  // char array for reading from pipe
	
	
	
	signal(SIGTERM,handler);
	

	if (pipe(pipefd) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	
	
	// gyerekek letrehozasa ---------------------------------
	for (i = 0; i < gyerekek_szama; i++){
		
		if (zarolva == 0){
			zarolva = 1;
			pid[i] = fork();	// creating parent-child processes
			if (pid[i] == -1) 
			{
				perror("Fork hiba");
				exit(EXIT_FAILURE);
			} 
			
			if (pid[i] == 0){ // gyerekek
				signal(SIGTERM,gyerek_handler);
				signal(SIGUSR1,gyerek_visszahiv);
				gyerek(i);
				close(pipefd[0]);
				close(pipefd[1]);
				return 0;
			}
		}
		else{
			i--;
		}
		
	}
	
	
	// szulo ---------------------------------
	
	
	printf("Szulo elindultm var a jelzesekre:\n");
	while(szamlalo!=gyerekek_szama);
	printf("jelzes erkezett: %i\n",szamlalo);
	printf("indulhat a munka!\n");
	
	
	// azonositok megkapasa ---------------------------------
	char azon[32];
	zarolva = -1;
	
	for (i = 0; i < gyerekek_szama; i++){
		if (zarolva == 0 || zarolva == -1){
			if (zarolva == 0){
				printf("Szulo megkapta %s-rol, hogy kesz, szulo folytatja...\n", azon);
			}
			
			zarolva = 1;
			
			sprintf(azon,"medve_%d",i);
			printf("A szulo elkuldte az azonositot: %s\n",azon);
			write(pipefd[1], azon, sizeof(azon));
			kill(pid[i], SIGTERM);
		}
		else{
			i--;
		}
	}
	while(zarolva != 0);
	printf("Szulo megkapta %s-rol, hogy kesz\n", azon);
	
	printf("szulo befejezte az azonositok osztasat\n");
	
	// expedíciók elindítása - jelzés, hogy mehetnek
	for (i = 0; i < gyerekek_szama; i++){
		kill(pid[i], SIGTERM);
	}
	
	fajl_ujrakeszites();
	struct nyuszi_tipus adat;
	
	// visszahívások ---------------------------------------
	srand(getpid());
	int r = rand() % (gyerekek_szama / 2);
	for (i = 0; i < r; i++){
		int kit = rand() % gyerekek_szama;
		kill(pid[kit],SIGUSR1);
	}

	
	// megfigyelesek ---------------------------------------
	close(pipefd[1]);
	while (read(pipefd[0],&adat,sizeof(adat))){
		uj_nyuszi(adat);
	}
	
	
	
	// vege ------------------------------------------------
	close(pipefd[0]);
	unlink(CSONEV);
	
	printf("szulo befejezte\n");
	
	exit(EXIT_SUCCESS);	// force exit, not necessary
}

void gyerek(int idx){
	// gyerekek letrehozasa ---------------------------------
	kill(getppid(),SIGTERM);
	printf("%i. Gyerek elindult\n",idx);
	
	while (gyerek_mehet != 1);
	
	// azonositok megkapasa ---------------------------------
	char azon[32];
	
	read(pipefd[0],azon,sizeof(azon));
	strcpy(gyerek_azon, azon);
	printf("Gyerek megkapta az azonositot: %s\n",gyerek_azon);
	
	gyerek_mehet = 0; // várjanak, még a cső használatával, a többiek kapják az azonosítókat
	kill(getppid(), SIGTERM);
	while (gyerek_mehet != 1);
	
	// megfigyelesek ---------------------------------------
	const char *kor[3] = {"fiatal", "felnott", "oreg" };
	
	srand (getpid());
	int ido = rand() % 3 + 2;
	sleep(ido);
	
	int i;
	for (i = 0; i < 3; i++){
		struct nyuszi_tipus adat;
		strcpy(adat.megfigyelo_neve, gyerek_azon);
		strcpy(adat.telepules_nev, "Budapest");
		
		int db = rand() % 100 + 1;
		
		adat.nyuszi_db = db;
		strcpy(adat.nyuszi_kor,kor[i]);
		adat.megfigyelesi_ido = time(NULL);
			
		write(pipefd[1], &adat, sizeof(struct nyuszi_tipus));
	}
	
	printf("%s gyerek befejezte a futast\n",gyerek_azon);
	
}

void uj_nyuszi(struct nyuszi_tipus uj){
	int beFileAllapot = open(FAJL, O_WRONLY|O_APPEND);
	
	if(beFileAllapot < 0){
		//printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		if (write(beFileAllapot, &uj, sizeof(struct nyuszi_tipus)) < 0){
			//printf("Hiba a fajlba iraskor!\n");
		}
		else{
			//printf("felvitt adatok: %s|%s|%i|%s|%s\n",&uj.megfigyelo_neve, &uj.telepules_nev, uj.nyuszi_db, &uj.nyuszi_kor, ido_formatum(uj.megfigyelesi_ido));
		}
	}
	
	close(beFileAllapot);
}

void fajl_ujrakeszites(){
	
	
	if (remove(FAJL) == 0) printf("Fajl torolve\n");
	else printf("Nem talaltam fajlt, ami torolheto!\n");
	
	int beFileAllapot = open(FAJL, O_WRONLY|O_CREAT, S_IRWXU);
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		printf("Fajl letrehozva!\n");
	}
	close(beFileAllapot);
}
