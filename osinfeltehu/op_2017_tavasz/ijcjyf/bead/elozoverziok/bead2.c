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
 

char* ajandekFileNev= "ajadekok.txt";
char* ajandekFileTmpNev = "ajadekokTemp.txt";
char* ajandekFileCsereNev = "csere.txt";
FILE * ajandeklista;
FILE * ajandeklistaIdeiglenes;


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


					time_t t = time(NULL);
					struct tm *tm = localtime(&t);
				//	printf("asctime = %s\n", asctime(tm));
					

					
					//////////////////////////////////////
					int i =0;
					char *buffer;
					  size_t bufsize = 32;
					  size_t characters;

						buffer = (char *)malloc(bufsize * (sizeof(char)));
						if( buffer == NULL)
						{
							perror("Unable to allocate buffer");
							exit(1);
						}
						
						
				
					
	do {
		printf("Üdvözololjuk e nemzeti konzultacio Focis kiadasaban!\n");
				
		printf("\n");
		printf("\n");
		printf("Kerem valasszon az alábbbi menüpontokból\n");
		printf("Uj kerdes-valasz felvitele      --- 1,es menüpont\n");
		printf("Listázás--               --- 2,es menüpont\n");
		printf("Listázás-- szűrés ajándékra     --- 3,as menüpont\n");
		printf("Listázás-- szűrés városra       --- 4,es menüpont\n");
		printf("\nKilépés                         --- 5,ös menüpont\n");
				
		
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
		
					if (sscanf(charMenupont,"%d", &keresszam)!=1 | ( keresszam>6 | keresszam<1) ){
					printf("Hibás adatbevitel\n");
			
			
					printf("kérjük irja be ujra");
						fflush(stdin); //a input puffer kiürítése
		
		
					}
			
			
					ajandeklista =fopen(ajandekFileNev,"a+t");
					ajandeklistaIdeiglenes =fopen(ajandekFileTmpNev,"wt");
				//	fseek(stdin,0,SEEK_END);
					
				//	scanf( "%s %s", &gyerekVezeteknevEllenorzes, &gyerekKeresztnevEllenorzes);
				//	scanf("");
				  	 // fflush(stdin);
				//
				
						printf("Type something: ");
						characters = getline(&buffer,&bufsize,stdin);
						printf("%zu characters were read.\n",characters);
						printf("You typed: '%s'\n",buffer);
						 i =0;
						for (i; i< keresszam; i++)
						{
							printf("Type something: ");
							characters = getline(&buffer,&bufsize,stdin);
							printf("%zu characters were read.\n",characters);
							printf("You typed: '%s'\n",buffer);
						
						}
	
						fprintf(ajandeklista, "kerdes%svalasz1%s valasz2%s\n",  buffer , buffer, asctime(tm));
							fprintf(ajandeklista, "kerdes%s valasz1%s valasz2%s\n",  buffer , buffer, asctime(tm));
					

					//Ellenorzés a nev szerepelt-e már
					/*	while (fscanf(ajandeklista, "%s %s %s %s %s %s %s %s %s %s", gyerekVezeteknev,gyerekKeresztnev,varos,cim,ajandek,nap, honap, napDatuma, ora, ev)!=EOF){
					
								if ((strcmp(gyerekVezeteknevEllenorzes,gyerekVezeteknev) == 0) & (strcmp(gyerekKeresztnevEllenorzes,gyerekKeresztnev) == 0)){
								printf("találat es keresztne vezeteknev\n");
								
								}else{
								fprintf(ajandeklistaIdeiglenes,"%s %s %s %s %s %s %s %s %s %s\n", gyerekVezeteknev,gyerekKeresztnev,varos,cim,ajandek, nap, honap, napDatuma, ora, ev);
								}
							
						}
						*/				

					
					fclose(ajandeklistaIdeiglenes);
					fclose(ajandeklista);
					
					rename(ajandekFileNev, ajandekFileCsereNev);
					rename(ajandekFileTmpNev, ajandekFileNev);	//ideiglenes lesz az ajandeklista
					rename(ajandekFileCsereNev,ajandekFileTmpNev);
					
					printf("\nAdatbevitel megtortént\n");
					break;
					
					
					case (2):
					//Listazas 
					system("clear");
					keresszam=0;
					babaszam=0;
					kifestoszam=0;
					autoszam=0;
					labdaszam=0;
					konyvszam=0;
					kirakoszam=0;	
					
					ajandeklista =fopen("ajadekok.txt","rt");		
																		
					while (fscanf(ajandeklista, "%s %s %s %s %s %s %s %s %s %s", gyerekVezeteknev,gyerekKeresztnev,varos,cim,ajandek, nap, honap, napDatuma, ora, ev)!=EOF){
					printf("%s %s %s %s %s %s %s %s %s %s\n", gyerekVezeteknev,gyerekKeresztnev,varos,cim,ajandek, nap, honap, napDatuma, ora, ev);
				
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
					fclose(ajandeklista);
					
					printf("\nÖsszes ajandek: %d\n",keresszam);
					printf("Összes ajandek: Labda: %d  Auto: %d Kirako: %d Kifesto: %d Konyv:%d Baba:%d\n",labdaszam,autoszam, kirakoszam, kifestoszam, konyvszam, babaszam);
					printf("\nKérések száma: %d\n",keresszam);
					
					
					break;
			
					
					case (3): 
					
					system("clear");
					keresszam=0;
					babaszam=0;
					kifestoszam=0;
					autoszam=0;
					labdaszam=0;
					konyvszam=0;
					kirakoszam=0;
					

					printf("Listazas Ajandekra:  \n");
					printf("\nKerek egy ajandek nevet (baba,auto,kirako,kifesto,konyv,labda)!\n");	
					scanf("%s", &ajandekSzures);
					
					
					ajandeklista =fopen("ajadekok.txt","rt");		
				//	while (fscanf(ajandeklista, "%s %s %s %s %s", gyerekVezeteknev, gyerekKeresztnev, varos, cim, ajandek) != EOF) {
				//	printf("%s %s %s %s %s\n", gyerekVezeteknev, gyerekKeresztnev, varos, cim, ajandek );
				//														}
					while (fscanf(ajandeklista, "%s %s %s %s %s %s %s %s %s %s", gyerekVezeteknev,gyerekKeresztnev,varos,cim,ajandek,nap, honap, napDatuma, ora, ev)!=EOF){
					
					if (strcmp(ajandekSzures,ajandek) == 0){
					printf("%s %s %s %s %s %s %s %s %s %s\n", gyerekVezeteknev,gyerekKeresztnev,varos,cim,ajandek, nap, honap, napDatuma, ora, ev);
					keresszam++;
			
					}
					}
					fclose(ajandeklista);
					printf("nincs tobb találat!");
					
					//printf("\nÖsszes találat: %d\n",keresszam);
				
					
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
					
					ajandeklista =fopen("ajadekok.txt","rt");		
																		
					while (fscanf(ajandeklista, "%s %s %s %s %s %s %s %s %s %s", gyerekVezeteknev,gyerekKeresztnev,varos,cim,ajandek, nap, honap, napDatuma, ora, ev)!=EOF){
					
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
					
					
					fclose(ajandeklista);
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