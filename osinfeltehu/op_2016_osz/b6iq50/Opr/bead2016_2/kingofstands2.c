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

#define CSONEV "egyedicso_b6iq50.ftc"

#define FAJL "kingofstands.data"
#define TMP "kingofsands_temp.data"


/*A "King of Stands" rendezvény szervezõ társaság, 
amint a rendezvény ideje eljön, megbízza egy partnerét(gyermekét) az esemény lebonyolítására. 
A gyermekfolyamat indulása után csõvezetéken megkapja, 
hogy hol kerül a rendezvény lebonyolításra! 
A helyszínre utazik, majd visszajelzi (jelzést küld vissza) a központnak, 
hogy a megadott helyszinen van, várja a résztvevõket,kész a rendezvény lebonyolítására. 
Ezután a rendezvényszervezõ csõvezetéken elküldi a helyszínen tartózkodó partnernek, 
hogy kik a résztvevõk. Ezt a partner képernyõre írja, 
majd várja a vendégeket és levezényli a rendezvényt! 
A rendezvény végén jelentést küld vissza, amiben tájékoztatja a fõszervezõt, 
hogy mennyire sikeres volt a rendezvény (véletlenszám), 
illetve a jelentkezettek közül végül is ki nem érkezett meg! 
(Minden résztvevõ 10% valószínûséggel nem érkezik meg!) 
Miután egy esemény lebonyolításra került, "King of Stands" újra megvizsgálja, 
melyik rendezvény a következõ! 
*/

/*
szülõ - gyerek
szülõ - olvas file-ból
szülõ csövön küldi a gyereknek a helyszínt
szülõ vár
gyerek szól, hogy õ csinálja a dolgát, várja a résztvevõket
szülõ csövön küldi gyereknek a résztvevõk listáját
gyerek olvas csõbõl
gyerek kiírja a résztvevõket (printf)
gyerek randomol a rendezvény sikerére 0-100? %-os
gyerek randomol a résztvevõk megjelenésére 10% hogy valaki nem érkezik meg
gyerek ír csövön szülõnek -> rendezvény sikere, ki nem érkezett meg

gyerek emgszüntetése ciklus végén

ez az egész for ciklusban az összes rendezvénnyel
hányszor fusson le a ciklus?

kell a file-ból: a helyszínek listája
				a jelentkezettek listája
*/

int esemenyek[42];
int esemenyek_ind=0;
int esem_jel_ind=0;

int pipefd[2];

volatile gyerek_olvashat = 0;
volatile szulo_olvashat = 0;
		
void gyerek_indul(){
	gyerek_olvashat=1;
	szulo_olvashat=0;
}

void szulo_indul(){
	szulo_olvashat=1;
	gyerek_olvashat=0;
}

void killgyerek(){
	close(pipefd[0]);
	close(pipefd[1]);
	printf("Gyerek folyamat leallt.\n");
	exit(0);
}

struct kingofstands_tipus{
	char jelentkezo_neve[42];
	char e_mail[42];
	int rendezveny_azonosito;
	int jelentkezo_azonosito;
	time_t jelentkezesi_ido;
};

void listaz();
void esemenyek_listazasa(int esemenyek[]);
int eleme_e(int esemenyek[], int rendezveny_azon);
void aktualis_esemenyre_jelentkezettek(char esemenyre_jelentkezettek[][42], int aktualis_esemeny);

const char * ido_formatum(time_t t);

int main(int argc,char** argv){
	
	//jelentkezettek listjájának kiírása
	printf("Az osszes jelentkezett venged osszes adata:\n");
	listaz();
	printf("____________________\n");
	
	//Csõ létrehozása //hibaüzenet, ha nem sikerült
	if (pipe(pipefd) == -1)
	{
		perror("Hiba a cso nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	signal(SIGUSR1, gyerek_indul);
	signal(SIGUSR2, szulo_indul);
	
	pid_t pid = fork();
	printf("Folyamat elindult.\n");
	
	if(pid == 0){
		signal(SIGKILL, killgyerek);
		printf("Gyerek folyamat elindult.\n");
		printf("___________________\n");
		
		while(gyerek_olvashat==0) {printf("...\n");} //gyerek vár, amíg olvashat
		printf("Gyerek tovabb haladt.\n");
			
		int esemenyek_szama=0;
		read(pipefd[0],&esemenyek_szama,sizeof(&esemenyek_szama));
		srand(time(NULL));
		int i;
		for(i=0; i<esemenyek_szama; i++){
			int aktualis_esemeny;
			printf("Gyerek olvassa a csobol az esemeny azonositojat.\n");
			read(pipefd[0],&aktualis_esemeny,sizeof(&aktualis_esemeny));
			printf("Gyereknel az aktualis esemeny: %i \n", aktualis_esemeny);
			
			printf("Gyerek var a resztvevok listajara.\n");
			gyerek_olvashat=0;
			kill(getppid(), SIGUSR2); //szulo irhat
			while(gyerek_olvashat==0) {}
			
			printf("Gyerek tovabb haladt.\n");
			
			char esemenyre_jelentkezettek2[42][42];
			int esem_jel_ind2=0;
			printf("Gyerek olvassa a csobol a jelentkezettek.\n");
			read(pipefd[0],esemenyre_jelentkezettek2,sizeof(esemenyre_jelentkezettek2));
			read(pipefd[0],&esem_jel_ind2,sizeof(&esem_jel_ind2));
			printf("Gyereknel a jelentkezettek listaja:\n");
			//printf("esem_jel_ind2: %i\n", esem_jel_ind2);
			int j;
			for(j=0; j<esem_jel_ind2; j++){
				printf("%i. vendeg: %s\n", j+1, esemenyre_jelentkezettek2[j]);
			}
				
			printf("Gyerek vizsgalja az esemenyek sikeresseget.\n");
			
			//aktualis_esemeny
			//esemenyre_jelentkezettek2
			
			int esemeny_siker=(rand()%10)+1;
			char ki_jelent_meg[esem_jel_ind2][42]; //megegyezõ indexek alapján
			
			for(j=0; j<esem_jel_ind2; j++){
				int r = rand() % 100; //random int 0 és 99 között
				if(r >89)
					strcpy(ki_jelent_meg[j]," nem jelent meg");
				else strcpy(ki_jelent_meg[j], " megjelent");
			}
			
			printf("Az esemeny sikeressege: %i.\n", esemeny_siker);
			
			for(j=0; j<esem_jel_ind2; j++){
				printf("%i. vendeg; %s %s az esmenyen\n", j+1, esemenyre_jelentkezettek2[j], ki_jelent_meg[j]);
			}
			
			//int esemeny_siker=(rand()%10)+1;
			//char ki_jelent_meg[esem_jel_ind2][42]; //megegyezõ indexek alapján
			
			printf("Gyerek atadja a szulonek a rendezveny sikeresseget es a megerkezettek listajat.\n");
			write(pipefd[1], &esemeny_siker, sizeof(&esemeny_siker));
			write(pipefd[1], ki_jelent_meg, sizeof(ki_jelent_meg));	
			fflush(NULL);
			
			gyerek_olvashat=0;
			kill(getppid(), SIGUSR2); //szulo irhat
			while(gyerek_olvashat==0) {}
			
		}
		printf("Gyerek folyamat leallt.\n");
		
	}else{
		printf("Szulo folyamat elindult.\n");
		printf("___________________\n");
		printf("Szulo inditja az esemenyek kilistazasat.\n");
		esemenyek_listazasa(esemenyek);
		//printf("Esemenyek indexe: %i\n", esemenyek_ind);
		int i;
		for(i=0; i<esemenyek_ind; i++){
			printf("Az %i. esemeny azonositoja: %i\n", i+1, esemenyek[i]);
		}
		printf("Szulo vegzett az esemenyek kilistazasaval.\n");
		printf("___________________\n");
		
		//események kiértékelése
		for(i=0; i<esemenyek_ind; i++){
							
			printf("Szulo atadja a gyereknek az %i(%i.) esemenyt.\n", i+1, esemenyek[i]);
			if(i==0){//ha az elsõ esemény van, akkor elmondjuk a gyereknek, hány eseménye lesz még
				write(pipefd[1], &esemenyek_ind, sizeof(&esemenyek_ind));
			}
			write(pipefd[1], &esemenyek[i], sizeof(&esemenyek[i]));
			fflush(NULL);
			kill(pid, SIGUSR1); //gyerek olvashat
			while(szulo_olvashat==0){} //szülõ vár a válaszára
			printf("Szulo tovabb haladt.\n");
				
			printf("Szulo inditja az esemenyre jelentkezettek kilistazasat.\n");
			char esemenyre_jelentkezettek[42][42]; //valamiért így se akarja kinullázni az elõzõ elemeket...
			esem_jel_ind=0;
			int why;
			int why2;
			for(why=0; why<42; why++){ //...ezért végigfutok rajta
				for(why2=0; why2<42; why2++){
					esemenyre_jelentkezettek[why][why2]=0;
				}
			}
			
			aktualis_esemenyre_jelentkezettek(esemenyre_jelentkezettek, esemenyek[i]);
			
			int j;
			for(j=0; j<esem_jel_ind; j++){
				printf("Az %i. esemenyre jelentkezett %i. vendeg: %s\n", i+1, j+1, esemenyre_jelentkezettek[j]);
			}
			printf("--------\n");
			
			//csövön nevek átküldése a gyereknek
			printf("Szulo atadja a gyereknek a esemenyre jelentkezettek listajat\n");
			write(pipefd[1], esemenyre_jelentkezettek, sizeof(esemenyre_jelentkezettek));
			write(pipefd[1], &esem_jel_ind, sizeof(&esem_jel_ind));
			fflush(NULL);
			szulo_olvashat=0;
			kill(pid, SIGUSR1); //gyerek olvashat
			//!!!!
			while(szulo_olvashat==0){} //szülõ vár a válaszára
			
			//kiolvasom a sikert és a vedégeket
			int esemeny_siker;
			char ki_jelent_meg[esem_jel_ind][42];
			
			read(pipefd[0],&esemeny_siker,sizeof(&esemeny_siker));
			read(pipefd[0],ki_jelent_meg,sizeof(ki_jelent_meg));
			
			printf("A %i esemeny sikeressege: %i.\n", esemenyek[i], esemeny_siker);
			
			for(j=0; j<esem_jel_ind; j++){
				printf("%i. vendeg; %s %s az esmenyen\n", j+1, esemenyre_jelentkezettek[j], ki_jelent_meg[j]);
			}
			szulo_olvashat=0;
			printf("___________________\n");
		}
		
		kill(pid, SIGKILL);
		printf("Folyamatok lealltak.\n");
	}
	
	close(pipefd[0]);
	close(pipefd[1]);
	
	return 0;
}

void listaz(){
	int beFileAllapot = open(FAJL, O_RDONLY);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		struct kingofstands_tipus be_rekord;
		int sor = 0;
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct kingofstands_tipus)) > 0){	
			sor++;
			printf("felvitt adatok: %s|%s|%d|%d|%s\n",&be_rekord.jelentkezo_neve, &be_rekord.e_mail, be_rekord.rendezveny_azonosito, be_rekord.jelentkezo_azonosito, ido_formatum(be_rekord.jelentkezesi_ido));
		}
	}
	close(beFileAllapot);
}

void esemenyek_listazasa(int esemenyek[]){
	
	int beFileAllapot = open(FAJL, O_RDONLY);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
		printf("%i\n", beFileAllapot);
	}
	else{
		
		struct kingofstands_tipus be_rekord;
		int sor = 0;
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct kingofstands_tipus)) > 0){	
			sor++;
			
			//új esemény azonosítójának hozzáadása a listához
			if(eleme_e(esemenyek, be_rekord.rendezveny_azonosito)==0){ //nem találta a tömbben az éppeni azonosítót
				esemenyek[esemenyek_ind]=be_rekord.rendezveny_azonosito;
				esemenyek_ind++;
			}
		}
	}
	close(beFileAllapot);
}

int eleme_e(int esemenyek[], int rendezveny_azon){
	
	//0 ha nem találta, 1 ha megtalálta
	int i;
	for(i=0; i<esemenyek_ind; i++){
		if(rendezveny_azon==esemenyek[i]){
			return 1;
		}
	}
	return 0;
}

void aktualis_esemenyre_jelentkezettek(char esemenyre_jelentkezettek[][42], int aktualis_esemeny){
	int beFileAllapot = open(FAJL, O_RDONLY);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
		printf("%i\n", beFileAllapot);
	}
	else{
		
		struct kingofstands_tipus be_rekord;
		int sor = 0;
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct kingofstands_tipus)) > 0){	
			sor++;
			
			//esemény alapján nevek hozzáadása a listához
			if(be_rekord.rendezveny_azonosito==aktualis_esemeny){
				strcpy(esemenyre_jelentkezettek[esem_jel_ind], be_rekord.jelentkezo_neve);
				esem_jel_ind++;
			}
		}
	}
	close(beFileAllapot);
}

const char * ido_formatum(time_t t){
	static char buff[20];
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));
	return buff;
}