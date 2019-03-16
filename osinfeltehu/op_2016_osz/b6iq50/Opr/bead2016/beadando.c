//Read and print out the file given in the parameter 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define FAJL "kingofstands.data"
#define TMP "kingofstands_temp.data"

struct kingofstands_tipus{
	char jelentkezo_neve[42];
	char e_mail[42];
	int rendezveny_azonosito;
	int jelentkezo_azonosito;
	time_t jelentkezesi_ido;
};

void uj_jelentkezes(struct kingofstands_tipus uj);
void jelentkezo_modositas(struct kingofstands_tipus uj, int jelentkezo_azon);
void torol(int jelentkezo_azon);
void listaz();
void torol_rendezveny(int rendezveny_azon);
void fajl_ujrakeszites();
int uj_jelentkezo_azonosito();

const char * ido_formatum(time_t t);


int main(int argc,char** argv){
	
	char valasz[1];
	
	do{
	
		printf("\n_________________________________\n");
		printf("Jelentkezes rendezvenyre: 1\n");
		printf("Jeletkezes modositasa: 2\n");
		printf("Jeletkezes torlese: 3\n");
		printf("Jeletkezettek listaja: 4\n");
		printf("Rendezveny adatainak torlese: 5\n");
		printf("Fajl ujrairasa uresre: 6\n");
		printf("Feltoltes regi adatokkal: 7\n");
		printf("Kilepes: X");
		
		printf("\n_________________________________\n");
		
		scanf("%s", valasz);
		
		if (strcmp(valasz, "1")==0){ // ----------------------------------------------------------
			struct kingofstands_tipus uj;
			
			printf("jelentkezo_neve:\n");
			scanf("%s",&uj.jelentkezo_neve);
			
			printf("e_mail:\n");
			scanf("%s",&uj.e_mail);
			
			printf("rendezveny_azonosito:\n");
			scanf("%d",&uj.rendezveny_azonosito);
			
			//jelentkezo_azonosito rendszeren belül
			
			uj.jelentkezesi_ido = time(NULL);
			
			uj_jelentkezes(uj);
			
		}else if (strcmp(valasz, "2")==0){ // ----------------------------------------------------------
			int jelentkezo_azon;
			printf("Melyik azonositoju jelentkezot modositsa:\n");
			scanf("%d",&jelentkezo_azon);
			
			struct kingofstands_tipus uj;
			
			printf("jelentkezo_neve:\n");
			scanf("%s",&uj.jelentkezo_neve);
			
			printf("e_mail:\n");
			scanf("%s",&uj.e_mail);
					
			printf("rendezveny_azonosito:\n");
			scanf("%d",&uj.rendezveny_azonosito);
			
			uj.jelentkezesi_ido = time(NULL);
			
			jelentkezo_modositas(uj, jelentkezo_azon);
			
		}else if (strcmp(valasz, "3")==0){ // ----------------------------------------------------------
			int jelenkezo_azon;
			printf("Jelentkezo azonositoja torleshez:\n");
			scanf("%d",&jelenkezo_azon);
			torol(jelenkezo_azon);
		}else if (strcmp(valasz, "4")==0){ // ----------------------------------------------------------
			listaz();
		}else if (strcmp(valasz, "5")==0){ // ----------------------------------------------------------
			int rendezveny_azon;
			printf("Rendezveny azonositoja torleshez:\n");
			scanf("%d",&rendezveny_azon);
			torol_rendezveny(rendezveny_azon);
		}else if (strcmp(valasz, "6")==0){ // ----------------------------------------------------------
			fajl_ujrakeszites();
		}else if (strcmp(valasz, "7")==0){ // ----------------------------------------------------------
			struct kingofstands_tipus uj;
			
			strcpy(uj.jelentkezo_neve,"Julis");
			strcpy(uj.e_mail,"julismaris@gmail.com");
			uj.rendezveny_azonosito=42;
			uj.jelentkezesi_ido = time(NULL);
			uj_jelentkezes(uj);

			strcpy(uj.jelentkezo_neve,"Jancsi");
			strcpy(uj.e_mail,"jancsika@gmail.com");
			uj.rendezveny_azonosito=42;
			uj.jelentkezesi_ido = time(NULL);
			uj_jelentkezes(uj);
			
			strcpy(uj.jelentkezo_neve,"Zolee");
			strcpy(uj.e_mail,"zolee@gmail.com");
			uj.rendezveny_azonosito=412;
			uj.jelentkezesi_ido = time(NULL);
			uj_jelentkezes(uj);
			
			strcpy(uj.jelentkezo_neve,"Chibe");
			strcpy(uj.e_mail,"chibe@gmail.com");
			uj.rendezveny_azonosito=412;
			uj.jelentkezesi_ido = time(NULL);
			uj_jelentkezes(uj);
			
			strcpy(uj.jelentkezo_neve,"Lacee");
			strcpy(uj.e_mail,"lacee@gmail.com");
			uj.rendezveny_azonosito=412;
			uj.jelentkezesi_ido = time(NULL);
			uj_jelentkezes(uj);
			
			strcpy(uj.jelentkezo_neve,"Oliveeer");
			strcpy(uj.e_mail,"oliveeer@gmail.com");
			uj.rendezveny_azonosito=18;
			uj.jelentkezesi_ido = time(NULL);
			uj_jelentkezes(uj);
			
			strcpy(uj.jelentkezo_neve,"Gubor");
			strcpy(uj.e_mail,"gubor@gmail.com");
			uj.rendezveny_azonosito=18;
			uj.jelentkezesi_ido = time(NULL);
			uj_jelentkezes(uj);
		}
	
	}while(strcmp(valasz, "X")!=0);
	
	return 0;
}

void uj_jelentkezes(struct kingofstands_tipus uj){
	
	int jel_azonosito = uj_jelentkezo_azonosito();
	uj.jelentkezo_azonosito=jel_azonosito;
	
	int beFileAllapot = open(FAJL, O_WRONLY|O_APPEND);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{	
		if (write(beFileAllapot, &uj, sizeof(struct kingofstands_tipus)) < 0){
			printf("Hiba a fajlba iraskor!\n");
		}
		else{
			printf("felvitt adatok: %s|%s|%d|%d|%s\n",&uj.jelentkezo_neve, &uj.e_mail, uj.rendezveny_azonosito, uj.jelentkezo_azonosito, ido_formatum(uj.jelentkezesi_ido));
		}
	}
	close(beFileAllapot);
}

void jelentkezo_modositas(struct kingofstands_tipus uj, int jelentkezo_azon){
	
	int tmpFileAllapot = open(TMP, O_WRONLY|O_CREAT, S_IRWXU);
	int beFileAllapot = open(FAJL, O_RDONLY);
	
	int hiba = 0;
	
	if(tmpFileAllapot < 0 || beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		printf("Fajlok elokeszitve! (1/3)\n");
		
		struct kingofstands_tipus be_rekord;
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct kingofstands_tipus)) > 0){	
			
			if (jelentkezo_azon != be_rekord.jelentkezo_azonosito){
				if(write(tmpFileAllapot, &be_rekord, sizeof(struct kingofstands_tipus)) < 0){
					printf("Hiba tortent a %i. jelentkezo kiirasnal\n",jelentkezo_azon);
					hiba = 1;
				}
			}
			else{
				uj.jelentkezesi_ido = be_rekord.jelentkezesi_ido;
				
				if (write(tmpFileAllapot, &uj, sizeof(struct kingofstands_tipus)) < 0){
					printf("Hiba tortent a %i. jelentkezo kiirasnal (a modositott erteknek)\n",jelentkezo_azon);
					hiba = 1;
				}
				else{
					printf("Rekord mentve: %i\n",jelentkezo_azon);
				}
			}
			
		}
		
		printf("Modositas lefutott! (2/3)\n");
		
	}
	close(tmpFileAllapot);
	close(beFileAllapot);
		
	if (hiba == 0){
		if (remove(FAJL)!=0){
			printf("Hiba tortent a mentes soran! #1");
			// printf( "ERROR: %s\n", strerror(errno) );
		}
		else{
			if (rename(TMP, FAJL)!=0){
				printf("Hiba tortent a mentes soran! #2");
			}
			else{
				printf("Sikeres mentes! (3/3)\n");
			}
		}
	}
}

void torol(int jelenkezo_azon){
	
	int tmpFileAllapot = open(TMP, O_WRONLY|O_CREAT, S_IRWXU);
	int beFileAllapot = open(FAJL, O_RDONLY);
	
	int hiba = 0;
	
	if(tmpFileAllapot < 0 || beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		printf("Fajlok elokeszitve! (1/3)\n");
		
		struct kingofstands_tipus be_rekord;
		
		while (read(beFileAllapot, &be_rekord, sizeof(struct kingofstands_tipus)) > 0){	
		
			if (be_rekord.jelentkezo_azonosito == jelenkezo_azon){
				// hagyja ki
				printf("Rekord megtalalva, torolve\n");
			}
			else{
				if (write(tmpFileAllapot, &be_rekord, sizeof(struct kingofstands_tipus)) < 0){
					printf("Hiba tortent a %i. jelenkezo_azon kiirasnal\n",be_rekord.jelentkezo_azonosito);
					hiba = 1;
				}
			}
			
		}
		
		printf("Modositas lefutott! (2/3)\n");
		
	}
	close(tmpFileAllapot);
	close(beFileAllapot);
		
	if (hiba == 0){
		if (remove(FAJL)!=0){
			printf("Hiba tortent a mentes soran! #1");
			// printf( "ERROR: %s\n", strerror(errno) );
		}
		else{
			if (rename(TMP, FAJL)!=0){
				printf("Hiba tortent a mentes soran! #2");
			}
			else{
				printf("Sikeres mentes! (3/3)\n");
			}
		}
	}
}

void torol_rendezveny(int rendezveny_azon){
	int tmpFileAllapot = open(TMP, O_WRONLY|O_CREAT, S_IRWXU);
	int beFileAllapot = open(FAJL, O_RDONLY);
	
	int hiba = 0;
	
	if(tmpFileAllapot < 0 || beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		printf("Fajlok elokeszitve! (1/3)\n");
		
		struct kingofstands_tipus be_rekord;
		
		while (read(beFileAllapot, &be_rekord, sizeof(struct kingofstands_tipus)) > 0){	
		
			if (be_rekord.rendezveny_azonosito == rendezveny_azon){
				// hagyja ki
				printf("Rekord megtalalva, torolve\n");
			}
			else{
				if (write(tmpFileAllapot, &be_rekord, sizeof(struct kingofstands_tipus)) < 0){
					printf("Hiba tortent a %i. rendezveny kiirasnal\n",be_rekord.rendezveny_azonosito);
					hiba = 1;
				}
			}
			
		}
		
		printf("Modositas lefutott! (2/3)\n");
		
	}
	close(tmpFileAllapot);
	close(beFileAllapot);
		
	if (hiba == 0){
		if (remove(FAJL)!=0){
			printf("Hiba tortent a mentes soran! #1");
			// printf( "ERROR: %s\n", strerror(errno) );
		}
		else{
			if (rename(TMP, FAJL)!=0){
				printf("Hiba tortent a mentes soran! #2");
			}
			else{
				printf("Sikeres mentes! (3/3)\n");
			}
		}
	}
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

int uj_jelentkezo_azonosito(){
	int beFileAllapot = open(FAJL, O_RDONLY);
	int azonosito=0;
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{		
		struct kingofstands_tipus be_rekord;
		while (read(beFileAllapot, &be_rekord, sizeof(struct kingofstands_tipus)) > 0){	
			azonosito = be_rekord.jelentkezo_azonosito;
		}
	}
	close(beFileAllapot);
	return azonosito+1;
}

const char * ido_formatum(time_t t){
	static char buff[20];
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));
	return buff;
}