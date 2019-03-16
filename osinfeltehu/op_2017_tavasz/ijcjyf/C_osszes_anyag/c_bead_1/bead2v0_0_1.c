#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>




main(){

char charMenupont[10];
int menupont;

char charAjandekMenupont[10];
int ajandekSwitch;

int keresszam=0;


int babaszam=0;
int kifestoszam=0;
int autoszam=0;
int labdaszam=0;
int konyvszam=0;
int kirakoszam=0;
 

char* kerdesekFileNev= "kerdesek.txt";
char* ajandekFileTmpNev = "kerdesekTemp.txt";
char* ajandekFileCsereNev = "csere.txt";
FILE * kerdeslista;
FILE * kerdeslistaIdeiglenes;


char* kerdeslistaVeglegesNev = "veglegesKerdesek.txt";
FILE* kerdeslistaVegleges;

char gyerekVezeteknev[20];
char gyerekKeresztnev[20];
char gyerekVezeteknevEllenorzes[20];
char gyerekKeresztnevEllenorzes[20];
char nap[5];
char honap[5];
char napDatuma[5];
char ora[10];
char ev [5];

char varos[20];
char cim[30];
char ajandek[10];

char varosSzures[20];
char ajandekSzures[10];
//enum ajandek;

int valaszokszama;

int valasztottKerdes;
char charValasztottKerdes;

					time_t t = time(NULL);
					struct tm *tm = localtime(&t);
				//	printf("asctime = %s\n", asctime(tm));
					

					
					//////////////////////////////////////
					int osszKeredesSzam=0;
					int valaszokSzama;
					int i =0;
					//char *buffer;
					char *buffer[5];
					size_t bufsize = 32;
					size_t characters;
					i =0;
					
					//helyfoglalás abeíráshoz
					for (i; i<5 ; i++)
						{
							buffer[i] = (char *)malloc(bufsize * (sizeof(char)));
							if( buffer[i] == NULL)
							{
								perror("Unable to allocate buffer");
								exit(1);
							}	
					}
					
					// helyfoglalás a kiratáshoz
					
					char * line = NULL;
					size_t len = 0;
					ssize_t read;
				
					
	do {
		printf("Üdvözololjuk e nemzeti konzultacio Focis kiadasaban!\n");		
		printf("\n");
		printf("\n");
		printf("Kerem valasszon az alábbbi menüpontokból\n");
		printf("\n");
		printf("Uj kerdes-valasz felvitele               --- 1,es menüpont\n");
		printf("Megleveo kerdesek-valaszok listázása     --- 2,es menüpont\n");
		printf("Vegleges kerdesek kivalasztasa           --- 3,as menüpont\n");
		printf("Kerdes-valasz modositasa                 --- 4,es menüpont\n");
		printf("\nKilépés                                --- 5,ös menüpont\n");
				
		
//		if(scanf("%d", &menupont)!=1 | ( menupont>6 | menupont<1) ){
//			printf("Hibás adatbevitel\n");
//			printf("kérjük irja be ujra");
		
//			}
			
			
		//gets fuggvényel				
		gets(charMenupont);
		
		if (sscanf(charMenupont,"%d", &menupont)!=1 | ( menupont>6 | menupont<1) ){
			printf("Hibás adatbevitel\n");
			
			
			printf("kérjük irja be ujra");
			fflush(stdin); //a input puffer kiürítése
		
		
			}
			
	
			
			
			switch (menupont){
			
					case (1): 
					//KERDESEK FELVITELE
					printf("\nKerem adja meg a kerdesek szamat!");
					gets(charMenupont);
		
					if (sscanf(charMenupont,"%d", &valaszokSzama)!=1 | ( valaszokSzama>4 | valaszokSzama<1) ){
					printf("Hibás adatbevitel\n");
			
			
					printf("kérjük irja be ujra");
						fflush(stdin); //a input puffer kiürítése
		
		
					}
			
			
					kerdeslista =fopen(kerdesekFileNev,"a+");
					kerdeslistaIdeiglenes =fopen(ajandekFileTmpNev,"wt");
					
					
				//	fseek(stdin,0,SEEK_END);
					
				//	scanf( "%s %s", &gyerekVezeteknevEllenorzes, &gyerekKeresztnevEllenorzes);
				//	scanf("");
				  	 // fflush(stdin);
				//
				
						printf("Irja be a kerdest!: ");
						characters = getline(&buffer[0],&bufsize,stdin);
						printf("%zu characters were read.\n",characters);
						printf("A kerdes: '%s'\n",buffer[0]);
						 i =0;
						 osszKeredesSzam++;
						for (i; i< valaszokSzama; i++)
						{
							printf("Irja be a %d. valasztasi lehetoseget!: ", (i+1));
							characters = getline(&buffer[i+1],&bufsize,stdin);
							//printf("%zu characters were read.\n",characters);
							//printf("You typed: '%s'\n",buffer);
						
						}
						 i =1;
					//	fprintf(ajandeklista, "%s %s %s %s %s %s\n",  buffer[0] , buffer[1],buffer[2],buffer[3],buffer[4], asctime(tm));
							
						//KERSES BEIRÁSA fajlba
						fprintf(kerdeslista,"%d%d  %s", osszKeredesSzam,valaszokSzama,buffer[0]);
						printf("kiirva");						
						
						//KERSES VALASZOK BEIRÁSA fajlba	
						for (i; i< valaszokSzama+1; i++)
						{
							fprintf(kerdeslista, "%s", buffer[i]);
							//printf("kiirva");
						
						}
						fprintf(kerdeslista, "%s",   asctime(tm));
						fclose(kerdeslista);
					
					
					
					printf("\nAdatbevitel megtortént\n");
					break;
					
					
					case (2):
					//Listazas 
					system("clear");
					
					kerdeslista =fopen(kerdesekFileNev,"rt");		
				
					  if (kerdeslista == NULL) exit(EXIT_FAILURE);
					  
					while ((read = getline(&line, &len, kerdeslista)) != -1) {
					//	printf("Retrieved line of length %zu :\n", read);
					
					
						printf("%s", line);
					
						}
					fclose(kerdeslista);
				
					
					break;
			
					
					case (3): 
					//vegleges kerdesek kivalasztasa
					system("clear");
					
					// bekeres
		
					
		
					gets(charMenupont);
					
					if (sscanf(charMenupont,"%d", &valasztottKerdes)!=1 | ( valasztottKerdes>4 | valasztottKerdes<1) ){
					printf("Hibás adatbevitel\n");
			
			
					printf("kérjük irja be ujra");
						fflush(stdin); //a input puffer kiürítése
		
		
					}
					
						//printf("A valasztott kerdes : %d", valasztottKerdes);
						
						//Listazas 
					system("clear");
					
					kerdeslista =fopen(kerdesekFileNev,"rt");		
					kerdeslistaVegleges= fopen(kerdeslistaVeglegesNev,"a+");	
					  if (kerdeslista == NULL) exit(EXIT_FAILURE);
					  
					  int counter = 0;
					  int valasztottKerdesChar;
					while ((read = getline(&line, &len, kerdeslista)) != -1) {
					//	printf("Retrieved line of length %zu :\n", read);
					
						valasztottKerdesChar =valasztottKerdes-'0';
						
						printf("KERES %d %c , %c\n",valasztottKerdes,valasztottKerdesChar,line[0] );	
						
						if (valasztottKerdes==line[0]){
							
								printf("Belepett" );	
							
							fprintf(kerdeslistaVegleges,"%s", line);
							for (counter=0; counter<3; counter++){
							read = getline(&line, &len, kerdeslista);
							
							
							//printf("%s", line);
								fprintf(kerdeslistaVegleges,"%s", line);
					
							
							}
						
						}
						
						
						
							
					}
					fclose(kerdeslista);
					fclose(kerdeslistaVegleges);	
								
					
					break;
			
					case (4):

					system("clear");
					keresszam=0;
					babaszam=0;
					kifestoszam=0;
					autoszam=0;
					labdaszam=0;
					konyvszam=0;
					kirakoszam=0;
					
					printf("Listazas városokra:  \n");
					printf("\nKerek egy város nevet!\n");	
					scanf("%s", &varosSzures);
					printf("\n");
					
					kerdeslista =fopen("ajadekok.txt","rt");		
																		
					while (fscanf(kerdeslista, "%s %s %s %s %s %s %s %s %s %s", gyerekVezeteknev,gyerekKeresztnev,varos,cim,ajandek, nap, honap, napDatuma, ora, ev)!=EOF){
					
						if (strcmp(varosSzures,varos) == 0){
					
							printf("%s %s %s %s %s %s %s %s %s %s\n", gyerekVezeteknev,gyerekKeresztnev,varos,cim,ajandek,nap, honap, napDatuma, ora, ev);

							keresszam++;
							if (strcmp("baba",ajandek) == 0){
					
							babaszam++;
							} 
							
							if (strcmp("labda",ajandek) == 0){
					
							labdaszam++;
							} 
							if (strcmp("auto",ajandek) == 0){
					
							autoszam++;
							} 
							if (strcmp("kirako",ajandek) == 0){
					
							kirakoszam++;
							} 
							if (strcmp("kifesto",ajandek) == 0){
					
							kifestoszam++;
							} 
							if (strcmp("konyv",ajandek) == 0){
					
							konyvszam++;
							} 

						}
					}
					
					
					fclose(kerdeslista);
					printf("nincs tobb találat!\n");
					printf("\nÖsszes találat: %d\n",keresszam);
					printf("Összes ajandek: Labda: %d  Auto: %d Kirako: %d Kifesto: %d  Konyv:%d Baba:%d\n",labdaszam,autoszam, kirakoszam, kifestoszam, konyvszam, babaszam);
					
					
					break;
					
					case (5):

						printf("kilépés parancs leütve \n");
					break;
					
					default: break;
				
			}
			
			
			

		
fflush(stdin);
} while (menupont!=5);





printf("sikeres kilépés");


}

