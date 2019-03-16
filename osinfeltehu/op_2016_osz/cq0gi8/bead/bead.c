//jelentkezés: név, e-mail, rendezvényazonosító, rendszeridő  - sorszám
//jelentkezettek listázása
//adatmódosítás
//adattörlés
//új rendezvény
//rendezvény törlése

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define VENDEGEK "vendegek.data"
#define RENDEZVENYEK "rendezvenyek.data"
#define TMP "vendeg_temp.data"
#define RENDTMP "rendezvenyektemp.data"

struct vendeg{
	char nev[32];
	char email[32];
	int rendID;
	time_t ido;
	
	int sorszam;
	
};

struct rendezveny{
	char nev[32];
	int rendID;
};

void menu();
void fajl_ujrakeszites();
void jelentkezes(struct vendeg v);
int utolsosorszam();
void listaz();
void torol(int sor);
void vendeg_modositas(struct vendeg uj, int sor);
void ujrendezveny(struct rendezveny r);
void rendezvenyeketlistaz();
void rendezvenyttorol(int ID);
void vendegekettorol(int ID);
const char * ido_formatum(time_t t);

int main(int argc,char** argv){
	
	
	//-----Ha nem talalja meg elso futtataskor a ket datafajlt, akkor
	//-----ezzel a fuggvennyel elkeszithetjuk oket uresen:
	//fajl_ujrakeszites();
	
	int vendegfajl = open(VENDEGEK, O_WRONLY|O_APPEND);
	int rendezvenyfajl = open(RENDEZVENYEK, O_WRONLY|O_APPEND);
	if(vendegfajl < 0 || rendezvenyfajl <0){
		fajl_ujrakeszites();
	}
	close(vendegfajl);
	close(rendezvenyfajl);
	
	int sorszam = utolsosorszam();
	int valasz=-1;
	menu();
	do{
		valasz = -1;
		printf("\nNyomjon 0-at a menu megjelenitesehez!\n");
	
		scanf("%d", &valasz);
		if(valasz == 0) { //MENU-------------------------------
			menu();
		}
		else if(valasz == 1) { //JELENTKEZES-------------------
			struct vendeg uj;
			
			printf("Nev:\n");
			scanf("%s", &uj.nev);
			
			printf("E-mail cim:\n");
			scanf("%s", &uj.email);
			
			printf("Rendezveny azonosito:\n");
			scanf("%i", &uj.rendID);
			
			uj.ido= time(NULL);
			
			sorszam= sorszam+1;
			uj.sorszam= sorszam;
			jelentkezes(uj);
			
			printf("A vendeg sorszama: ");
			printf("%i",sorszam);
			
		}
		else if(valasz == 2) { //LISTAZAS----------------------
			listaz();
		}
		else if(valasz == 3) { //MODOSITAS---------------------
			listaz();
		
			int sor;
			printf("Melyik sort kivanja modositani?\n");
			scanf("%d",&sor);
			
			struct vendeg uj;
			
			printf("Nev:\n");
			scanf("%s", &uj.nev);
			
			printf("E-mail cim:\n");
			scanf("%s", &uj.email);
			
			printf("Rendezveny azonosito:\n");
			scanf("%i", &uj.rendID);
			
			uj.ido= time(NULL);
			
			vendeg_modositas(uj, sor);
		}
		else if(valasz == 4) { //TORLES -----------------------
			listaz();
			
			int sor;
			printf("Melyik vendeget kivanja torolni?\n");
			scanf("%d",&sor);
			torol(sor);
		}
		else if(valasz == 5) { //UJ RENDEZVENY-----------------
			struct rendezveny uj;
		
			printf("Rendezveny neve:\n");
			scanf("%s", &uj.nev);
			
			printf("Rendezveny azonositoszama:\n");
			scanf("%i", &uj.rendID);
			
			ujrendezveny(uj);
		}
		else if(valasz == 6) { //RENDEZVENYEK LISTAZASA--------
			rendezvenyeketlistaz();
		}
		else if(valasz == 7) { //RENDEZVENY TORLESE------------
			rendezvenyeketlistaz();
			
			
			int ID;
			printf("\nAdja meg a torlendo rendezveny azonosito szamat\n");
			scanf("%i",&ID);
			rendezvenyttorol(ID);
			vendegekettorol(ID);
		}
		

		
	}while(valasz != 8);

	return 0;
}


void menu() {
		printf("\nKing of Stands nyilvantarto program");
		printf("\n\nKerem valasszon egy opciot az alabbiak kozul:");
		printf("\n-----------------------------------");
		printf("\n1: Jelentkezes");
		printf("\n2: Vendegek listazasa");
		printf("\n3: Vendeg adatainak modositasa");
		printf("\n4: Vendeg torlese");
		printf("\n5: Uj rendezveny");
		printf("\n6: Rendezveny listazasa");
		printf("\n7: Rendezveny torlese");
		printf("\n8: Kilepes\n");
}

void jelentkezes(struct vendeg v) {
	int beFileAllapot = open(VENDEGEK, O_WRONLY|O_APPEND);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		if (write(beFileAllapot, &v, sizeof(struct vendeg)) < 0){
			printf("Hiba a fajlba iraskor!\n");
		}
		else{
			printf("felvitt adatok: %s|%s|%i|\n%s|%i\n",&v.nev, &v.email, v.rendID, ido_formatum(v.ido), v.sorszam);
		}
	}
	
	
	close(beFileAllapot);
}

void ujrendezveny(struct rendezveny r) {
	int beFileAllapot = open(RENDEZVENYEK, O_WRONLY|O_APPEND);
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		if (write(beFileAllapot, &r, sizeof(struct rendezveny)) < 0){
			printf("Hiba a fajlba iraskor!\n");
		}
		else{
			printf("felvitt adatok: %s|%i\n",&r.nev, r.rendID);
		}
	}
	
	close(beFileAllapot);
}

void listaz(){
	int beFileAllapot = open(VENDEGEK, O_RDONLY);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		struct vendeg be_rekord;
		int sor = 0;
		printf("##########################################\n");
		while (read(beFileAllapot, &be_rekord, sizeof(struct vendeg)) > 0){	
			sor++;
			printf("%i:%s\t%s\t%i\t%s\t%i\n",sor, &be_rekord.nev, &be_rekord.email, be_rekord.rendID, ido_formatum(be_rekord.ido), be_rekord.sorszam);
		}
		printf("\n##########################################\n");
	}
	close(beFileAllapot);
}

void vendeg_modositas(struct vendeg uj, int sor){
	
	int tmpFileAllapot = open(TMP, O_WRONLY|O_CREAT, S_IRWXU);
	int beFileAllapot = open(VENDEGEK, O_RDONLY);
	
	int hiba = 0;
	
	if(tmpFileAllapot < 0 || beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		struct vendeg be_rekord;
		int itt_tart_sor = 0;
		
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct vendeg)) > 0){	
		
			itt_tart_sor++;
			
			if (itt_tart_sor != sor){
				if(write(tmpFileAllapot, &be_rekord, sizeof(struct vendeg)) < 0){
					printf("Hiba tortent a %i. sor kiirasnal\n",itt_tart_sor);
					hiba = 1;
				}
				else{
					printf("Rekord mentve: %i\n",itt_tart_sor);
				}
			}
			else{
				uj.ido = be_rekord.ido;
				
				if (write(tmpFileAllapot, &uj, sizeof(struct vendeg)) < 0){
					printf("Hiba tortent a %i. sor kiirasnal (a modositott erteknek)\n",itt_tart_sor);
					hiba = 1;
				}
				else{
					printf("Rekord mentve: %i\n",itt_tart_sor);
				}
			}
			
		}
		
		printf("Modositas lefutott!");
		
	}
	close(tmpFileAllapot);
	close(beFileAllapot);
		
	if (hiba == 0){
		if (remove(VENDEGEK)!=0){
			printf("Hiba tortent a mentes soran! #1");
			// printf( "ERROR: %s\n", strerror(errno) );
		}
		else{
			if (rename(TMP, VENDEGEK)!=0){
				printf("Hiba tortent a mentes soran! #2");
			}
			else{
				printf("Sikeres mentes!");
			}
		}
	}
}

void torol(int sor){
	
	int tmpFileAllapot = open(TMP, O_WRONLY|O_CREAT, S_IRWXU);
	int beFileAllapot = open(VENDEGEK, O_RDONLY);
	
	int hiba = 0;
	
	if(tmpFileAllapot < 0 || beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		struct vendeg be_rekord;
		int itt_tart_sor = 0;
		
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct vendeg)) > 0){	
		
			itt_tart_sor++;
			
			if (itt_tart_sor == sor){
				// hagyja ki
				printf("Rekord megtalalva, torolve\n");
			}
			else{
				if (write(tmpFileAllapot, &be_rekord, sizeof(struct vendeg)) < 0){
					printf("Hiba tortent a %i. sor kiirasnal\n",itt_tart_sor);
					hiba = 1;
				}
			}
			
		}
		
		printf("Modositas lefutott!");
		
	}
	close(tmpFileAllapot);
	close(beFileAllapot);
		
	if (hiba == 0){
		if (remove(VENDEGEK)!=0){
			printf("Hiba tortent a mentes soran! #1");
			// printf( "ERROR: %s\n", strerror(errno) );
		}
		else{
			if (rename(TMP, VENDEGEK)!=0){
				printf("Hiba tortent a mentes soran! #2");
			}
			else{
				printf("Sikeres mentes!");
			}
		}
	}
}

int utolsosorszam() {
	int beFileAllapot = open(VENDEGEK, O_RDONLY);
	int utso = 0;
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		struct vendeg be_rekord;
		int sor = 0;
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct vendeg)) > 0){	
			sor++;
			utso = be_rekord.sorszam;
		}
		
	}
	
	close(beFileAllapot);
	
	return utso;
}

void fajl_ujrakeszites(){
	if (remove(VENDEGEK) == 0) printf("Fajl torolve\n");
	
	int beFileAllapot = open(VENDEGEK, O_WRONLY|O_CREAT, S_IRWXU);
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		printf("Fajl letrehozva!\n");
	}
	close(beFileAllapot);
	
	if (remove(RENDEZVENYEK) == 0) printf("Fajl torolve\n");
	
	beFileAllapot = open(RENDEZVENYEK, O_WRONLY|O_CREAT, S_IRWXU);
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		printf("Fajl letrehozva!\n");
	}
	close(beFileAllapot);
}

const char * ido_formatum(time_t t){
	static char buff[20];
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));
	return buff;
}

void rendezvenyttorol(int ID){
	int tmpFileAllapot = open(RENDTMP, O_WRONLY|O_CREAT, S_IRWXU);
	int beFileAllapot = open(RENDEZVENYEK, O_RDONLY);
	
	int hiba = 0;
	
	if(tmpFileAllapot < 0 || beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		printf("Fajlok elokeszitve! (1/3)\n");
		
		struct rendezveny be_rekord;
		
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct rendezveny)) > 0){	
			
			if ( be_rekord.rendID == ID ){
				// hagyja ki
				printf("Rekord megtalalva, torolve\n");
			}
			else{
				if (write(tmpFileAllapot, &be_rekord, sizeof(struct rendezveny)) < 0){
					printf("Hiba tortent a kiirasnal\n");
					hiba = 1;
				}
			}
			
		}
		
		printf("Modositas lefutott! (2/3)\n");
		
	}
	close(tmpFileAllapot);
	close(beFileAllapot);
		
	if (hiba == 0){
		if (remove(RENDEZVENYEK)!=0){
			printf("Hiba tortent a mentes soran! #1");
			// printf( "ERROR: %s\n", strerror(errno) );
		}
		else{
			if (rename(RENDTMP, RENDEZVENYEK)!=0){
				printf("Hiba tortent a mentes soran! #2");
			}
			else{
				printf("Sikeres mentes! (3/3)\n");
			}
		}
	}
}

void vendegekettorol(int ID) {
	int tmpFileAllapot = open(TMP, O_WRONLY|O_CREAT, S_IRWXU);
	int beFileAllapot = open(VENDEGEK, O_RDONLY);
	
	int hiba = 0;
	
	if(tmpFileAllapot < 0 || beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		printf("Fajlok elokeszitve! (1/3)\n");
		
		struct vendeg be_rekord;
		
		
		int toroltsorok=0;
		while (read(beFileAllapot, &be_rekord, sizeof(struct vendeg)) > 0){	
			if ( be_rekord.rendID == ID ){
				//hagyja ki
				toroltsorok=toroltsorok+1;
			}
			else {
				if (write(tmpFileAllapot, &be_rekord, sizeof(struct vendeg)) < 0){
					printf("Hiba tortent a kiirasnal\n");
					hiba = 1;
				}
			}
		}
		printf("%i darab vendeg torlesre kerult.", toroltsorok);
		printf("Modositas lefutott! (2/3)\n");
		
	}
	close(tmpFileAllapot);
	close(beFileAllapot);
		
	if (hiba == 0){
		if (remove(VENDEGEK)!=0){
			printf("Hiba tortent a mentes soran! #1");
			// printf( "ERROR: %s\n", strerror(errno) );
		}
		else{
			if (rename(TMP, VENDEGEK)!=0){
				printf("Hiba tortent a mentes soran! #2");
			}
			else{
				printf("Sikeres mentes! (3/3)\n");
			}
		}
	}
	
}

void rendezvenyeketlistaz() {
	int beFileAllapot = open(RENDEZVENYEK, O_RDONLY);
			
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		struct rendezveny be_rekord;
		int sor = 0;
		printf("##########################################\n");
		while (read(beFileAllapot, &be_rekord, sizeof(struct rendezveny)) > 0){	
			sor++;
			printf("%s\t%i\n",&be_rekord.nev, be_rekord.rendID);
		}
		printf("\n##########################################");
	}
	close(beFileAllapot);
	
}


