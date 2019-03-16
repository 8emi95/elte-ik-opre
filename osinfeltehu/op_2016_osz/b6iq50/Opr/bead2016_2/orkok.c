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

#define FAJL "kingofsands.data"
#define TMP "kingofsands_temp.data"

/*A "King of Stands" rendezvény szervező társaság, 
amint a rendezvény ideje eljön, megbízza egy partnerét(gyermekét) az esemény lebonyolítására. 
A gyermekfolyamat indulása után csővezetéken megkapja, 
hogy hol kerül a rendezvény lebonyolításra! 
A helyszínre utazik, majd visszajelzi (jelzést küld vissza) a központnak, 
hogy a megadott helyszinen van, várja a résztvevőket,kész a rendezvény lebonyolítására. 
Ezután a rendezvényszervező csővezetéken elküldi a helyszínen tartózkodó partnernek, 
hogy kik a résztvevők. Ezt a partner képernyőre írja, 
majd várja a vendégeket és levezényli a rendezvényt! 
A rendezvény végén jelentést küld vissza, amiben tájékoztatja a főszervezőt, 
hogy mennyire sikeres volt a rendezvény (véletlenszám), 
illetve a jelentkezettek közül végül is ki nem érkezett meg! 
(Minden résztvevő 10% valószínűséggel nem érkezik meg!) 
Miután egy esemény lebonyolításra került, "King of Stands" újra megvizsgálja, 
melyik rendezvény a következő! 
*/

/*
szülő - gyerek
szülő - olvas file-ból
szülő csövön küldi a gyereknek a helyszínt
szülő vár
gyerek szól, hogy ő csinálja a dolgát, várja a résztvevőket
szülő csövön küldi gyereknek a résztvevők listáját
gyerek olvas csőből
gyerek kiírja a résztvevőket (printf)
gyerek randomol a rendezvény sikerére 0-100? %-os
gyerek randomol a résztvevők megjeleésére 10% hogy valaki nem érkezik meg
gyerek ír csövön szülőnek -> rendezvény sikere, ki nem érkezett meg

gyerek emgszüntetése ciklus végén

ez az egész for ciklusban az összes rendezvénnyel
hányszor fusson le a ciklus?
*/

int pipefd[2];
//volatile -> nazt definiálja hogy ki mikor mehet tovább,
//volatile esetén nem optimalizálhatja a fordító a programot =>
/*
void update(void)
{
time =time+1;
}

void main()
{
time=0;
while(time<100);
}

az update triggerelése esetén a time folyamatosan nő => nem lesz végtelen ciklus
*/
volatile gyerek_olvashat = 0;
volatile szulo_olvashat = 0;

void gyerek_indul(){
	gyerek_olvashat=1;
}

void szulo_indul(){
	szulo_olvashat=1;
}

struct kingofstands_tipus{
	char jelentkezo_neve[42];
	char e_mail[42];
	int rendezveny_azonosito;
	int jelentkezo_azonosito;
	time_t jelentkezesi_ido;
};

void listaz();
const char * ido_formatum(time_t t);

int main(int argc,char** argv){
	
	if (pipe(pipefd) == -1) //cső létrehozásakor visszaad egy 2 elemű int tömböt -> 0 - olvas, 1 - ír
	{
		perror("Hiba a cso nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	//így minden tragikus forkolás és rossz időbeosztás nélkül össze lesznek kötve
	signal(SIGUSR1, gyerek_indul);
	signal(SIGUSR2, szulo_indul);
	
	pid_t pid = fork();
	printf("Ork vagyok.\n");
	
	if(pid == 0){
		printf("Gyerek ork vagyok, irok a csobe.\n");
		//int azon1[32];
		
		//gyerek ír---------------
		char szoveg[32];
		sprintf(szoveg,"medve");
		//write(pipefd[1], azon1, sizeof(azon1)); => így a tömb referenciáját írja ki
		write(pipefd[1], szoveg, sizeof(szoveg));
		//(mit használsz, mibe olvasol azaz referencia, mekkora byte mennyiséget olvasol)
		//\n kell csőbe íráskor hogy üreítse a buffert
		printf("Gyerek ork vagyok, irtam a csobe: %s \n", szoveg);
		fflush(NULL); 	// flushes all write buffers (not necessary) 
		
		//kettő közti végtelen ciklus-------
		while(gyerek_olvashat==0){} //fut amíg nem a gyerek nem olvashat
		printf("Gyerek ork vagyok, varok.\n");
		printf("%i\n", gyerek_olvashat);
		
		//gyerek olvas--------------
		printf("Gyerek ork vagyok, olvasok.\n");
		read(pipefd[0],szoveg,sizeof(szoveg));
		printf("Az uzenet a szulotol: %s \n", szoveg);
		
	}else{
		//sleep(3); nem muszáj
		
		//szülő olvas---------
		//mindenképp emgvárja az írást
		printf("Szulo ork vagyok, olvasok.\n");
		char szoveg[32];
		//wait(); addig vár míg a gyerek nem végzett, továbbiakban nem kell a signalok miatt
		read(pipefd[0],szoveg,sizeof(szoveg));
		printf("Az uzenet a gyerektol: %s \n", szoveg);
		
		//szülő ír a csőbe a kivárt olvasás után--------
		printf("Szulo ork vagyok, irok a csobe.\n");
		sprintf(szoveg,"roka");
		write(pipefd[1], szoveg, sizeof(szoveg));
		printf("Szulo ork vagyok, irtam a csobe: %s \n", szoveg);
		fflush(NULL);
		
		//szülő signalt küld a gyereknek, hogy olvashat--------
		printf("Indulhat a gyerek\n");
		kill(pid, SIGUSR1);
	}
	
	//		(amit felülírok, ami lekezeli az adott signal-t)
	//signal(SIGTERM,gyerek_handler);
	//signal(SIGUSR1,gyerek_visszahiv);
	//		SIGUSR2
	//		(kinek, milyen típust)
	//kill(pid[i], SIGTERM);
	
	
	
	//cső eltakarítása
	close(pipefd[0]);
	close(pipefd[1]);
	//unlink(CSONEV); névtelen cső esetén nem kell, mert nem létezik
		
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

const char * ido_formatum(time_t t){
	static char buff[20];
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));
	return buff;
}