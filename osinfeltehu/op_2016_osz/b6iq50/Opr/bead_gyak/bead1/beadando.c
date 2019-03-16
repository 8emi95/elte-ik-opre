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

#define FAJL "nyuszi.data"
#define TMP "nyuszi_temp.data"

struct nyuszi_tipus{
	char megfigyelo_neve[32];
	char telepules_nev[32];
	int nyuszi_db;
	char nyuszi_kor[32];
	time_t megfigyelesi_ido;
};

void fajl_ujrakeszites();
void listaz();
void listaz_nev_alapjan(char nev[32]);
void listaz_telep_alapjan(char telep[32]);
void uj_nyuszi(struct nyuszi_tipus uj);
void nyuszi_modositas(struct nyuszi_tipus uj, int sor);
void torol(int sor);
void statisztika(char telep[32]);
const char * ido_formatum(time_t t);

int main(int argc,char** argv){

	char valasz[1];
	
	do{
		
		
		
		
		
		printf("\n_________________________________\n");
		printf("Nyuszi adatok rogzitese: 1\n");
		printf("Nyuszi modositasa: 2\n");
		printf("Nyuszi torlese: 3\n");
		printf("Nyuszik listaja: 4\n");
		printf("Kereses nev alapjan: 5\n");
		printf("Kereses telepules alapjan: 6\n");
		printf("statisztika: 7\n");
		printf("Fajl ujrairasa uresre.: 8\n");
		printf("Feltoltes regi adatokkal.: 9\n");
		
		
		printf("\n_________________________________\n");
		
		scanf("%s", valasz);
		
		
		
		
		
		
		if (strcmp(valasz, "1")==0){ // ----------------------------------------------------------
			struct nyuszi_tipus uj;
			
			printf("megfigyelo_neve:\n");
			scanf("%s",&uj.megfigyelo_neve);
			
			printf("telepules_nev:\n");
			scanf("%s",&uj.telepules_nev);
			
			printf("nyuszi_db:\n");
			scanf("%d",&uj.nyuszi_db);
			
			printf("nyuszi_kor:\n");
			scanf("%s",&uj.nyuszi_kor);
			
			//printf("atadott ertekek: %s|%s|%i|%s|%s\n",&uj.megfigyelo_neve, &uj.telepules_nev, uj.nyuszi_db, &uj.nyuszi_kor, ido_formatum(uj.megfigyelesi_ido));
		
			
			uj.megfigyelesi_ido = time(NULL);
			
			uj_nyuszi(uj);
		}
		else if (strcmp(valasz, "2")==0){ // ----------------------------------------------------------
			int sor;
			printf("hanyadik adatot modositsa:\n");
			scanf("%d",&sor);
			
			struct nyuszi_tipus uj;
			
			printf("megfigyelo_neve:\n");
			scanf("%s",&uj.megfigyelo_neve);
			
			printf("telepules_nev:\n");
			scanf("%s",&uj.telepules_nev);
			
			printf("nyuszi_db:\n");
			scanf("%d",&uj.nyuszi_db);
			
			printf("nyuszi_kor:\n");
			scanf("%s",&uj.nyuszi_kor);
			
			uj.megfigyelesi_ido = time(NULL);
			
			nyuszi_modositas(uj, sor);
		}
		else if (strcmp(valasz, "3")==0){ // ----------------------------------------------------------
			int sor;
			printf("hanyadik adatot torolje:\n");
			scanf("%d",&sor);
			torol(sor);
		}
		else if (strcmp(valasz, "4")==0){ // ----------------------------------------------------------
			listaz();
		}
		else if (strcmp(valasz, "5")==0){ // ----------------------------------------------------------
			char nev[32];
			printf("nev:\n");
			scanf("%s",&nev);
			listaz_nev_alapjan(nev);
		}
		else if (strcmp(valasz, "6")==0){ // ----------------------------------------------------------
			char nev[32];
			printf("telep nev:\n");
			scanf("%s",&nev);
			listaz_telep_alapjan(nev);
		}
		else if (strcmp(valasz, "7")==0){ // ----------------------------------------------------------
			char nev[32];
			printf("telep nev:\n");
			scanf("%s",&nev);
			statisztika(nev);
		}
		else if (strcmp(valasz, "8")==0){ // ----------------------------------------------------------
			fajl_ujrakeszites();
		}
		else if (strcmp(valasz, "9")==0){ // ----------------------------------------------------------
			struct nyuszi_tipus uj;
			
			// --
			strcpy(uj.megfigyelo_neve,"Julis");
			strcpy(uj.telepules_nev,"NyusziFalva");
			uj.nyuszi_db = 30;
			strcpy(uj.nyuszi_kor,"fiatal");
			uj.megfigyelesi_ido = time(NULL);
			
			struct tm * timeinfo;
			time ( &uj.megfigyelesi_ido );
			timeinfo = localtime ( &uj.megfigyelesi_ido );
			timeinfo->tm_year -= 3;
			timeinfo->tm_mon -= 1;
			
			uj.megfigyelesi_ido = mktime ( timeinfo );
			
			uj_nyuszi(uj);
			// --
			strcpy(uj.megfigyelo_neve,"Maris");
			strcpy(uj.telepules_nev,"NyusziFalva");
			uj.nyuszi_db = 20;
			strcpy(uj.nyuszi_kor,"oreg");
			uj.megfigyelesi_ido = time(NULL);
			
			time ( &uj.megfigyelesi_ido );
			timeinfo = localtime ( &uj.megfigyelesi_ido );
			timeinfo->tm_year -= 3;
			timeinfo->tm_mon -= 1;
			
			uj.megfigyelesi_ido = mktime ( timeinfo );
			
			uj_nyuszi(uj);
			// --
			strcpy(uj.megfigyelo_neve,"Maris");
			strcpy(uj.telepules_nev,"NyusziFalva");
			uj.nyuszi_db = 20;
			strcpy(uj.nyuszi_kor,"felnott");
			uj.megfigyelesi_ido = time(NULL);
			
			time ( &uj.megfigyelesi_ido );
			timeinfo = localtime ( &uj.megfigyelesi_ido );
			timeinfo->tm_year -= 3;
			timeinfo->tm_mon += 1;
			
			uj.megfigyelesi_ido = mktime ( timeinfo );
			
			uj_nyuszi(uj);
			// --
			strcpy(uj.megfigyelo_neve,"Sanyi");
			strcpy(uj.telepules_nev,"NyusziFalva");
			uj.nyuszi_db = 15;
			strcpy(uj.nyuszi_kor,"oreg");
			uj.megfigyelesi_ido = time(NULL);
			
			time ( &uj.megfigyelesi_ido );
			timeinfo = localtime ( &uj.megfigyelesi_ido );
			timeinfo->tm_year -= 3;
			timeinfo->tm_mon += 1;
			
			uj.megfigyelesi_ido = mktime ( timeinfo );
			
			uj_nyuszi(uj);
			// --
			strcpy(uj.megfigyelo_neve,"Sanyi");
			strcpy(uj.telepules_nev,"NyusziFalva");
			uj.nyuszi_db = 20;
			strcpy(uj.nyuszi_kor,"fiatal");
			uj.megfigyelesi_ido = time(NULL);

			
			uj_nyuszi(uj);
		}
	
	
	
	
	}while(strcmp(valasz, "X")!=0);

	
	
	
	return 0;
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



void listaz(){
	int beFileAllapot = open(FAJL, O_RDONLY);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		struct nyuszi_tipus be_rekord;
		int sor = 0;
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct nyuszi_tipus)) > 0){	
			sor++;
			printf("%i:%s\t%s\t%i\t%s\t%s\n",sor, &be_rekord.megfigyelo_neve, &be_rekord.telepules_nev, be_rekord.nyuszi_db, &be_rekord.nyuszi_kor, ido_formatum(be_rekord.megfigyelesi_ido));
		}
	}
	close(beFileAllapot);
}


void listaz_nev_alapjan(char* nev){
	int beFileAllapot = open(FAJL, O_RDONLY);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		struct nyuszi_tipus be_rekord;
		int sor = 0;
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct nyuszi_tipus)) > 0){	
			if ( strcmp(be_rekord.megfigyelo_neve, nev) == 0 ){
				sor++;
				printf("%i:%s\t%s\t%i\t%s\t%s\n",sor, &be_rekord.megfigyelo_neve, &be_rekord.telepules_nev, be_rekord.nyuszi_db, &be_rekord.nyuszi_kor, ido_formatum(be_rekord.megfigyelesi_ido));
			}
		}
	}
	close(beFileAllapot);
}



void listaz_telep_alapjan(char* telep){
	int beFileAllapot = open(FAJL, O_RDONLY);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		struct nyuszi_tipus be_rekord;
		int sor = 0;
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct nyuszi_tipus)) > 0){	
			if ( strcmp(be_rekord.telepules_nev, telep) == 0 ){
				sor++;
				printf("%i:%s\t%s\t%i\t%s\t%s\n",sor, &be_rekord.megfigyelo_neve, &be_rekord.telepules_nev, be_rekord.nyuszi_db, &be_rekord.nyuszi_kor, ido_formatum(be_rekord.megfigyelesi_ido));
			}
		}
	}
	close(beFileAllapot);
}



void uj_nyuszi(struct nyuszi_tipus uj){
	int beFileAllapot = open(FAJL, O_WRONLY|O_APPEND);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		if (write(beFileAllapot, &uj, sizeof(struct nyuszi_tipus)) < 0){
			printf("Hiba a fajlba iraskor!\n");
		}
		else{
			printf("felvitt adatok: %s|%s|%i|%s|%s\n",&uj.megfigyelo_neve, &uj.telepules_nev, uj.nyuszi_db, &uj.nyuszi_kor, ido_formatum(uj.megfigyelesi_ido));
		}
	}
	
	close(beFileAllapot);
}



void nyuszi_modositas(struct nyuszi_tipus uj, int sor){
	
	int tmpFileAllapot = open(TMP, O_WRONLY|O_CREAT, S_IRWXU);
	int beFileAllapot = open(FAJL, O_RDONLY);
	
	int hiba = 0;
	
	if(tmpFileAllapot < 0 || beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		printf("Fajlok elokeszitve! (1/3)\n");
		
		struct nyuszi_tipus be_rekord;
		int itt_tart_sor = 0;
		
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct nyuszi_tipus)) > 0){	
		
			itt_tart_sor++;
			
			if (itt_tart_sor != sor){
				if(write(tmpFileAllapot, &be_rekord, sizeof(struct nyuszi_tipus)) < 0){
					printf("Hiba tortent a %i. sor kiirasnal\n",itt_tart_sor);
					hiba = 1;
				}
				else{
					printf("Rekord mentve: %i\n",itt_tart_sor);
				}
			}
			else{
				uj.megfigyelesi_ido = be_rekord.megfigyelesi_ido;
				
				if (write(tmpFileAllapot, &uj, sizeof(struct nyuszi_tipus)) < 0){
					printf("Hiba tortent a %i. sor kiirasnal (a modositott erteknek)\n",itt_tart_sor);
					hiba = 1;
				}
				else{
					printf("Rekord mentve: %i\n",itt_tart_sor);
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

void torol(int sor){
	
	int tmpFileAllapot = open(TMP, O_WRONLY|O_CREAT, S_IRWXU);
	int beFileAllapot = open(FAJL, O_RDONLY);
	
	int hiba = 0;
	
	if(tmpFileAllapot < 0 || beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		printf("Fajlok elokeszitve! (1/3)\n");
		
		struct nyuszi_tipus be_rekord;
		int itt_tart_sor = 0;
		
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct nyuszi_tipus)) > 0){	
		
			itt_tart_sor++;
			
			if (itt_tart_sor == sor){
				// hagyja ki
				printf("Rekord megtalalva, torolve\n");
			}
			else{
				if (write(tmpFileAllapot, &be_rekord, sizeof(struct nyuszi_tipus)) < 0){
					printf("Hiba tortent a %i. sor kiirasnal\n",itt_tart_sor);
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

void statisztika(char telep[32]){
	int beFileAllapot = open(FAJL, O_RDONLY);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		struct nyuszi_tipus be_rekord;
		
		int ev = 0;
		int honap = 0;
		int darab = 0;
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct nyuszi_tipus)) > 0){	
			if ( strcmp(be_rekord.telepules_nev, telep) == 0 ){
				
				//char buff[10];
				//strftime(buff, 10, "%Y", localtime(&be_rekord.megfigyelesi_ido));
				//int c_ev = atoi(buff);
				//strftime(buff, 10, "%m", localtime(&be_rekord.megfigyelesi_ido));
				//int c_honap = atoi(buff);				
				
				struct tm * timeinfo;
				
				//time ( &be_rekord.megfigyelesi_ido );
				timeinfo = localtime ( &be_rekord.megfigyelesi_ido );
				int c_ev = timeinfo->tm_year+1900;
				int c_honap = timeinfo->tm_mon+1;
				
				if (ev == 0){
					ev = c_ev;
					honap = c_honap;
				}
				
				if (ev != c_ev || honap != c_honap){
					printf("(%i.%i): %i darab\n",ev,honap,darab);
					
					ev = c_ev;
					honap = c_honap;
					darab = be_rekord.nyuszi_db;
				}
				else{
					darab += be_rekord.nyuszi_db;
				}
				
			}
		}
		
		printf("(%i.%i): %i darab\n",ev,honap,darab);
	}
	close(beFileAllapot);
}

const char * ido_formatum(time_t t){
	static char buff[20];
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));
	return buff;
}