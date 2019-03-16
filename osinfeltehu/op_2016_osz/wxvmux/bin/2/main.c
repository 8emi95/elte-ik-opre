/*
Kapu Erik
WXVMUX
1. beadandó feladat. Beadási határidő: 2016-11-07.
Készítsen C programot, amely rendszerhívások alkalmazásával megoldja a következő feladatot:

Egy kiállításokat, rendezvényeket szervező társaság a "King of Stands",
elektronikus látogatói nyilvántartást készített.

 * A látogatók a nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, a rendezvény azonosító számát!

 * Az elektronikus rendszer a jelentkezés nyugtázásaként visszaadja, hogy hányadik vendégként történt meg a regisztráció!
   
   Az alkalmazás adjon lehetőséget vendég * jelentkezéshez,a * jelentkezettek listázásához, * egy vendég adatainak módosításához, * törléséhez. 

   Legyen lehetőségünk egy * új rendezvény indításához, egy *rendezvény összes adatának törléséhez! 
   
 * A jelentkezőknél a korábbi adatok mellett rögzítsük automatikusan a jelentkezési időt(rendszeridő) is(ezt nem kell bekérni)!

Készítsünk C programot, amellyel a leírt tevékenység elvégezhető. Az adatokat fájlban tároljuk.  

2.rész

 * A "King of Stands" rendezvény szervező társaság, amint a rendezvény ideje eljön, megbízza egy partnerét(gyermekét) az esemény lebonyolítására.
 
 * A gyermekfolyamat indulása után csővezetéken megkapja, hogy hol kerül a rendezvény lebonyolításra! 
 
 * A helyszínre utazik, majd visszajelzi (jelzést küld vissza) a központnak, hogy a megadott helyszinen van, várja a résztvevőket,kész a rendezvény lebonyolítására. 
 
* Ezután a rendezvényszervező csővezetéken elküldi a helyszínen tartózkodó partnernek, hogy kik a résztvevők. Ezt a partner képernyőre írja, majd várja a vendégeket és levezényli a rendezvényt! 
 
 A rendezvény végén jelentést küld vissza, amiben tájékoztatja a főszervezőt, hogy mennyire sikeres volt a rendezvény (véletlenszám), illetve a jelentkezettek közül végül is ki nem érkezett meg! (Minden résztvevő 10% valószínűséggel nem érkezik meg!) 
 
 Miután egy esemény lebonyolításra került, "King of Stands" újra megvizsgálja, melyik rendezvény a következő!

*/
	

#include <stdio.h> 
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

typedef struct Latogato {
	char nev[30];
	char email[30];
	char idopont[20];
	struct Latogato *next;
}Latogato;

typedef struct Rendezveny{
	char nev[30];
	int azonosito;
	Latogato *l;
	struct Rendezveny *next;
}Rendezveny;

int rendezvenylistaz();
void jelentkezes();
void ujRendezveny();
int listaz();
void latogatoAdatModosit();
void rendezvenyTorles(int azonosito);
void latogatoTorol();
void rendezvenyLebonyolitasa();
int isEmailValid();
void mentes();
void betoltes();
Rendezveny* isLetezoRendezvenyAzon(int rendezvAzon);

int latogatokSzama = 0;

Rendezveny *ren = NULL;

void jelentkezes(){
	printf("\n---------------\n");
    
	printf("\n");
	if(!rendezvenyListaz()) return;
	printf("\n");
	
	char nev[30];
	char email[30];
	int rendezvAzon;
	char idopont[20];
	
	printf("Kerem adja meg az alabbi adatokat a jelentkezeshez: \n");
	
	Rendezveny *valasztottRendezveny = NULL;
	do{
		char azon[5];
		printf("Rendezveny azonositoszama: ");
		scanf("%s",azon);
		rendezvAzon = atoi(azon);
		valasztottRendezveny = isLetezoRendezvenyAzon(rendezvAzon);
	}while(valasztottRendezveny == NULL);
	
	printf("Nev: ");
	scanf("%s", &nev);
	
	do{
		printf("E-mail: ");
		scanf("%s", &email);
	} while(!isEmailValid(email));
	
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	strftime(idopont, sizeof(idopont)-1, "%d %m %Y %H:%M", t);
	
	Latogato *latogato = (Latogato* )malloc( sizeof(Latogato));	
	strcpy(latogato->nev, nev);
	strcpy(latogato->email, email);
	strcpy(latogato->idopont,idopont);
	latogato->next = NULL;
	
	if(valasztottRendezveny-> l == NULL){
		valasztottRendezveny-> l = latogato;
	} else {
		latogato->next = valasztottRendezveny->l;
		valasztottRendezveny-> l = latogato;
	}
	
	int sorszam = 0;
	Latogato *ptr = valasztottRendezveny->l;
	while(ptr != NULL){        
      sorszam = sorszam + 1;
      ptr = ptr->next;
    }
	
	printf("Sikeres jelentkezes! A jelentkezo sorszama a %i azonositoju rendezvenyen: %i\n", rendezvAzon, sorszam);
	
	latogatokSzama = latogatokSzama + 1;
 
 }

void ujRendezveny(){
	char nev[30];
	int rendezvAzon;
	
	printf("Adja meg a rendezveny nevet: ");
	scanf("%s", &nev);
	
	srand(time(NULL));
	int randnum = rand() % 99999;
    if(randnum < 10000) randnum = 99999 - randnum;	
	
	Rendezveny *rendezveny = (Rendezveny *)malloc(sizeof(Rendezveny));
	
	strcpy(rendezveny->nev,nev);
	rendezveny->azonosito = randnum;
	rendezveny->l = NULL;
	
	rendezveny->next = ren;
	ren = rendezveny;	

	printf("Rendezveny sikeresen felvele");
}
 
int listaz(){
	printf("\n\n---------------\n");
	
	Rendezveny* seged = ren;
	
	int sorszam = 0;
	while(seged != NULL){
		Latogato* latSeged = seged->l;
		while(latSeged != NULL){
			sorszam = sorszam + 1;
			printf("%i. %s - %s - %i \n", sorszam,latSeged->nev, latSeged->email,seged->azonosito);
			latSeged = latSeged->next;
		}
		seged = seged->next;
	}
	if(sorszam == 0){
		latogatokSzama = 0;
		printf("Nincsennek bejegyzett latogatok!");
		return 0;
	} else {
		latogatokSzama = sorszam;
		return 1;
	}
	
} 

int rendezvenyListaz(){
	Rendezveny *seged = ren;
	if (seged != NULL) {		
		printf("Aktualis rendezvenyek: \n");			
		while (seged != NULL){			
			printf("%s: %i\n", seged->nev,seged->azonosito);			
			seged = seged->next;
		}
		return 1;
	}
	else printf("Jelenleg nincs aktualis rendezveny. \n");
	return 0;
}

void rendezvenyTorles(int azonosito){
		
	int found = 0;
	Rendezveny* renSeged = ren;
	Rendezveny* renSegedElozo = NULL;
	
	int azon = azonosito;
	do{	
		renSeged = ren;
		renSegedElozo = NULL;
		if(azon == 0){
			if(!rendezvenyListaz())return;
			printf("\n Adja meg a torolni kivant rendezveny azonsoitoszamat: ");
			char azonString[5];
			scanf("%s",azonString);
			azon = atoi(azonString);
		}
		while(renSeged != NULL && !found){			
			if (azon == renSeged->azonosito){
				found = azon;
				Latogato* latSeged = renSeged->l;
				Latogato* latElozo = latSeged;
				
				while(latSeged != NULL){
					latSeged = latSeged->next;
					free(latElozo);
					latElozo = latSeged;
				}
			} else {
				renSegedElozo = renSeged;
				renSeged = renSeged->next;
			}
		}
		azon = 0; // ha inputrol jott az azonosito, de helytelen volt ez szukseges, hogy ujra kerje 
	} while(!found);	
	
	if(renSeged->next == NULL){
		if(renSegedElozo == NULL){
			free(renSeged);
			ren = NULL;
		} else{
			renSegedElozo->next = renSeged->next;
			free(renSeged);
		}
	} else {
		if(renSegedElozo == NULL){
			ren = renSeged ->next;
			free(renSeged);
		} else{
			renSegedElozo->next = renSeged->next;
			free(renSeged);
		}
	}
	printf("Az %i azonositoju rendezveny sikeresen torolve!", found);
	
}

void latogatoAdatModosit(){ 
	
	char nev[30];
	char email[30];
	int rendezvAzon;
	int melyik;
	
	if(!listaz()) return;
	
	do{
		printf("\n Hanyadik bejegyzest kivanja modositani?");
		scanf("%i", &melyik);		
	}while(melyik <= 0 || melyik > latogatokSzama);
	
	printf("\n Nev: ");
	scanf("%s", &nev);
	
	do{
		printf("E-mail: ");
		scanf("%s", &email);
	} while(!isEmailValid(email));
		
	
	Latogato* latSeged = NULL;
	Latogato* modositando = NULL;
	Rendezveny* renSeged = ren;
	
	int sorszam = 0;
	
	while(renSeged != NULL){
		
		latSeged = renSeged->l;
		
		while(latSeged != NULL && sorszam != melyik){
			sorszam = sorszam + 1;
			if(sorszam == melyik) modositando = latSeged; 
			latSeged = latSeged->next;
		}
		renSeged = renSeged->next;	
	}
	
	strcpy(modositando->nev, nev);
	strcpy(modositando->email, email);
	printf("\nA bejegyzes modositva!\n");

}

void latogatoTorol(){
	if(!listaz()) return;
	
	int melyik = 0;
	
	
	do{
		printf("\n Hanyadik bejegyzest kivanja torolni?");
		scanf("%i", &melyik);		
	}
	while(melyik <= 0 || melyik > latogatokSzama);	
	
	Rendezveny* renSeged = ren;	
	
	int sorszam = 0;
	while((renSeged != NULL) && (sorszam != melyik)){
		Latogato* latSeged = renSeged -> l;
	    Latogato* latSegedElozo = NULL;		
		while((latSeged != NULL) && (sorszam != melyik)){
			sorszam = sorszam + 1;
			if(sorszam == melyik){
				if(latSeged->next == NULL){
					if(latSegedElozo != NULL){
						latSegedElozo->next = NULL;
						free(latSeged);
					} else {
						renSeged->l = NULL;
						free(latSeged);						
					}
					
				} else 
				if(latSegedElozo != NULL){
					latSegedElozo->next = latSeged->next;
					free(latSeged);
				} else {
					renSeged->l = latSeged->next;
					free(latSeged);	
				}
								
			} else {
				latSegedElozo = latSeged;
				latSeged = latSeged->next;
			}
			
		}
		renSeged = renSeged->next;
	}
	latogatokSzama = latogatokSzama - 1;
	printf("Sikeres torles");
}

void handler(int signumber){
 // printf("A %i szamu szignal megerkezett(handler)\n",signumber);
}

void rendezvenyLebonyolitasa(){
	if(!rendezvenyListaz())return;
	int azon = 0;
	int found = 0;
	Rendezveny* renSeged = ren;
	Rendezveny* renSegedElozo = NULL;
	
	
	signal(SIGUSR1 ,handler);
	int status;
	
	int pipefdParentToChild[2];
	int pipefdChildToParent[2];	
	
	char pipeAdat[100];
	int pipeSzam;
	Latogato pipeLatogato;
	
	if (pipe(pipefdParentToChild) == -1){
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
	}
	
	if (pipe(pipefdChildToParent) == -1){
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
	}
	
	pid_t child = fork();
		
	if (child<0){perror("A fork hivas sikertelen volt\n"); exit(1);} 

	if (child>0){		
		do{	
			renSeged = ren;
			renSegedElozo = NULL;
			printf("\n Adja meg az inditando rendezveny azonsoitoszamat: ");			
			char azonString[5];
			scanf("%s",azonString);
			azon = atoi(azonString);

			while(renSeged != NULL && !found){			
				if (azon == renSeged->azonosito){
					if(renSeged->l == NULL){
						printf("Erre a rendezvenyre egyetlen latogato sincs feljelentkezve!");						
					} else {
						found = 1;			
					}
				} else {
					renSegedElozo = renSeged;
					renSeged = renSeged->next;
				}
			}
		} while(!found);	
		
		char helyszin[30];
		printf("Kerem adja meg a rendezveny helyszinet: ");
		scanf("%s", helyszin);
						
		close(pipefdParentToChild[0]);
		close(pipefdChildToParent[1]);
		
		write(pipefdParentToChild[1], helyszin, sizeof(helyszin));
		
		printf("Helyszin elkuldve, a King of Stands visszajelzesre var \n");
		
		pause();
		printf("A visszajelzes megerkezett, a partner a rendezveny helyszinere ert.\n",SIGUSR1 );
		
		Latogato* latSeged = renSeged->l;
		while(latSeged != NULL){						
			write(pipefdParentToChild[1], latSeged ,sizeof(Latogato));				
			latSeged = latSeged->next;			
			pause();
		}
		
		close(pipefdParentToChild[1]);					
		read(pipefdChildToParent[0], &pipeSzam, sizeof(pipeSzam));		
				
		printf("\n A %s rendezveny ertekelese: %i\n",renSeged->nev, pipeSzam);
		printf("A kovetkezo jelentkezettek nem jelentek meg: \n");
		
		while(read(pipefdChildToParent[0], pipeAdat, sizeof(pipeAdat))){
			printf("%s \n", pipeAdat);
		}
		close(pipefdChildToParent[0]);
		waitpid(child,&status,0);
		
		rendezvenyTorles(azon);
	} else {				
		
		close(pipefdParentToChild[1]);
		close(pipefdChildToParent[0]);
		
		Latogato* latogatok = NULL;
				
		read(pipefdParentToChild[0], pipeAdat, sizeof(pipeAdat));
		printf("Helyszin atveve: %s \n",pipeAdat);	
					
		kill(getppid(),SIGUSR1); 
				
		printf("A resztvevok nevei:\n");		 
		while(read(pipefdParentToChild[0], &pipeLatogato, sizeof(Latogato))){									
			printf("%s, ", pipeLatogato.nev);
			//latogato felvetele a gyerek latogatoinak listajara
			Latogato *latogato = (Latogato* )malloc( sizeof(Latogato));	
			strcpy(latogato->nev, pipeLatogato.nev);
			strcpy(latogato->email, pipeLatogato.email);
			strcpy(latogato->idopont,pipeLatogato.idopont);
			latogato->next = NULL;
			
			if(latogatok == NULL){
				latogatok = latogato;
			} else {
				latogato->next = latogatok;
				latogatok = latogato;
			}
			kill(getppid(), SIGUSR1);
		} 
		
		//rendezveny sikeressege
		srand(getpid());
		int randNum = rand()%10 + 1;
		
		write(pipefdChildToParent[1], &randNum, sizeof(int));
		
		//Akik nem jelentek meg
		Latogato* latSeged = latogatok;
		while(latSeged != NULL){
			randNum = rand()%10 + 1;
			if(randNum == 5){
				write(pipefdChildToParent[1], latSeged->nev, sizeof(latSeged->nev));
			}
			latSeged = latSeged->next;
		}
		//destukt
		latSeged = latogatok;
		Latogato* latElozoSeged = latSeged;
		while(latSeged != NULL){
			latSeged = latSeged->next;
			free(latElozoSeged);
			latElozoSeged = latSeged;
		}
		
		close(pipefdChildToParent[1]);
		close(pipefdParentToChild[0]);
		
		exit(1);
	}		
}
	
int isEmailValid(const char *address) {
	int count = 0;
	const char *c, *domain;
	static char *rfc822_specials = "()<>@,;:\\\"[]";

	//nev validacio
	for (c = address;  *c;  c++) {
		if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) == '\"')) {
			while (*++c) {
				if (*c == '\"') break;
				if (*c == '\\' && (*++c == ' ')) continue;
				if (*c <= ' ' || *c >= 127) return 0;
		    
			}
			if (!*c++) return 0;
			if (*c == '@') break;
			if (*c != '.') return 0;
			continue;
		}
		if (*c == '@') break;
		if (*c <= ' ' || *c >= 127) return 0;
		if (strchr(rfc822_specials, *c)) return 0;
    }
	if (c == address || *(c - 1) == '.') return 0;

  //domain validacio
	if (!*(domain = ++c)) return 0;
    do {
		if (*c == '.') {
			if (c == domain || *(c - 1) == '.') return 0;
			count++;
		}
		if (*c <= ' ' || *c >= 127) return 0;
		if (strchr(rfc822_specials, *c)) return 0;
    } while (*++c);

  return (count >= 1);
}

Rendezveny* isLetezoRendezvenyAzon(int rendezvAzon){
	Rendezveny *seged = ren;	
	while(seged != NULL){
		if (rendezvAzon == seged->azonosito)return seged;
		seged = seged->next;
	}	
	return NULL;
}

void mentes(){
	Rendezveny* renSeged = ren;
	FILE *renFile;
	renFile = fopen("events.bin", "w");
	if (renFile != NULL){	
		while(renSeged != NULL){
			fwrite(renSeged, sizeof(*renSeged), 1, renFile);
			
			Latogato* latSeged = renSeged->l;
			FILE *latfile;
			char filename[12];
			sprintf(filename, "lat%i.bin", renSeged->azonosito);
			latfile = fopen(filename, "w");
			if (latfile != NULL){							
				while(latSeged != NULL){
					fwrite(latSeged, sizeof(*latSeged), 1, latfile);//ha van path megadva, nem mukodik. Miert?
					latSeged = latSeged->next;			
				}
				fclose(latfile);
			} else {
				printf("Hiba a fajl nyitasakor!(latogato)");
			}
			renSeged = renSeged->next;		
		}
		fclose(renFile);
	} else {
		printf("Hiba a fajl nyitasakor!(rendezveny)");
	}
	printf("Sikeres mentes!");
}

void betoltes(){
	
	ren = NULL;
	
	if( access("events.bin", 0 ) != -1 ) {
		FILE *renfile;
		renfile = fopen("events.bin", "r");
		if (renfile != NULL) {
			Rendezveny renSeged;
			while (fread(&renSeged, sizeof(renSeged), 1, renfile)) {
				
				Rendezveny *rendezveny = (Rendezveny *)malloc(sizeof(Rendezveny));
				
				strcpy(rendezveny->nev,renSeged.nev);
				rendezveny->azonosito = renSeged.azonosito;
				rendezveny->l = NULL;
				
				char filename[12];
				sprintf(filename, "lat%i.bin", renSeged.azonosito);
				
				FILE *latfile;
				latfile = fopen(filename, "r");
				Latogato latSeged;
				
				while(fread(&latSeged, sizeof(latSeged), 1, latfile)){
					
					Latogato* latogato = (Latogato*)malloc(sizeof(Latogato));
					strcpy(latogato->nev, latSeged.nev);
					strcpy(latogato->email, latSeged.email);
					strcpy(latogato->idopont,latSeged.idopont);
                    																		
					latogato->next = rendezveny->l;
					rendezveny->l = latogato;									
					
				}			
				
				fclose(latfile);
				
				rendezveny->next = ren;
				ren = rendezveny;	
			}
		}
		fclose(renfile);
	}
	printf("Az adatok sikeresen betoltve! \n");
}

int main( int argc, char * argv[] ) {
	
	printf("\n\nKing of Stands\n");
	
	int valasz;
	do{
		printf("\n\n---------------\n");
		printf("Kerem valasszon az alabbi lehetosegek kozul: \n\n");
		printf("Jelentkezes(1)\n");
		printf("Latogatok listazasa(2)\n");
		printf("Rendezvenyek listazasa(3)\n");
		printf("Uj rendezveny bejegyzese(4)\n");
		printf("Latogato adatainak modositasa(5)\n");
		printf("Latogato torlese(6)\n");
		printf("Rendezveny torlese(7)\n");
		printf("Rendezveny lebonyolitasa(8)\n");
		printf("Adatok mentese(9)\n");
		printf("Adatok betoltese(10)\n");
		printf("Kilepes(11)\n");
		printf("---------------\n");
		printf("Valasz: ");
		scanf(" %i", &valasz);
		getchar();
		switch(valasz) {
			case 1:
				jelentkezes();
				break;
			case 2:
				listaz();
				break;
			case 3:
				rendezvenyListaz();
				break;
			case 4:
				ujRendezveny();
				break;
			case 5:
				latogatoAdatModosit();
				break;
			case 6: 
				latogatoTorol();
				break;
			case 7: 
				rendezvenyTorles(0);
				break;
			case 8: 
				rendezvenyLebonyolitasa();
				break;
			case 9: 
				mentes();
				break;
			case 10: 
				betoltes();
				break;
			case 11:
				printf("A program terminal..");
				break;
			default:
				printf("\n\nNem letezo menupont!\n");
				break;
		}
	} while(valasz != 11);	 
	
	return 0;
}
