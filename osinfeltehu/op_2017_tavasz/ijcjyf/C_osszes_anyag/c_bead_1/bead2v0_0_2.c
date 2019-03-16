#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct Kerdesek {
   char  kerdes[100];
   char  valasz1[100];
   char  valasz2[100];
   char  valasz3[100];
   char  valasz4[100];
   char  datum[100];
   int   valaszokSzama;
};

void kerdesekBeolvasasa();


main(){

char charMenupont[10];
int menupont;

char* kerdesekFileNev= "kerdesek.txt";
char* ajandekFileTmpNev = "kerdesekTemp.txt";
char* ajandekFileCsereNev = "csere.txt";
FILE * kerdeslista;
FILE * kerdeslistaIdeiglenes;


char* kerdeslistaVeglegesNev = "veglegesKerdesek.txt";
FILE* kerdeslistaVegleges;


char nap[5];
char honap[5];
char napDatuma[5];
char ora[10];
char ev [5];


//enum ajandek;

int valaszokszama;

int valasztottKerdes;
int torleshezValasztottKerdes;
int modositashozValasztottKerdes;
char charValasztottKerdes;

					time_t t = time(NULL);
					struct tm *tm = localtime(&t);
				//	printf("asctime = %s\n", asctime(tm));
					

					
					//////////////////////////////////////

					 struct Kerdesek osszesKerdes[ 30 ];
					 int aktualisKerdes;
					
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
				/////////BEOLVASAS
				
				
				system("clear");
					aktualisKerdes=0;
					kerdeslista =fopen(kerdesekFileNev,"rt");		
				
					  if (kerdeslista == NULL) exit(EXIT_FAILURE);
					  
					while ((read = getline(&line, &len, kerdeslista)) != -1) {
					//	printf("Retrieved line of length %zu :\n", read);
					
						;
						if (line[0]=='K'){
							osszKeredesSzam++;
							 strcpy( osszesKerdes[aktualisKerdes].kerdes, line);
							 
							//printf("%s", line);
							//read = getline(&line, &len, kerdeslista);
							
							read = getline(&line, &len, kerdeslista);
							 
							if(line[0]=='V'){
								
								 strcpy( osszesKerdes[aktualisKerdes].valasz1, line);
								 read = getline(&line, &len, kerdeslista);
							}
							
							if(line[0]=='V'){
								
								 strcpy( osszesKerdes[aktualisKerdes].valasz2, line);
								 read = getline(&line, &len, kerdeslista);
							}
							
							if(line[0]=='V'){
								
								 strcpy( osszesKerdes[aktualisKerdes].valasz3, line);
								 read = getline(&line, &len, kerdeslista);
							}
							
							if(line[0]=='V'){
								
								 strcpy( osszesKerdes[aktualisKerdes].valasz4, line);
								 read = getline(&line, &len, kerdeslista);
							}
							
							if(line[0]=='D'){
								
								 strcpy( osszesKerdes[aktualisKerdes].datum, line);
								
								
							}
								
							// Book1.book_id = 6495407;
							// Book1.book_id = 6495407;
							printf("STRUCT\n");
							printf("%s",  osszesKerdes[aktualisKerdes].kerdes);
						
							printf("%s",  osszesKerdes[aktualisKerdes].valasz1);
							printf("%s",  osszesKerdes[aktualisKerdes].valasz2);
							printf("%s",  osszesKerdes[aktualisKerdes].valasz3);
							printf("%s",  osszesKerdes[aktualisKerdes].valasz4);
										
							aktualisKerdes++;
						
						}
						
						}
						
						
					fclose(kerdeslista);
				
				
				
					
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
		printf("Kerdes torlesehez                        --- 5,ös menüpont\n");
		printf("\nKilépés                                --- 6,ös menüpont\n");
				
		
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
					printf("\nKerem adja meg a valaszok szamat!");
					gets(charMenupont);
		
					if (sscanf(charMenupont,"%d", &valaszokSzama)!=1 | ( valaszokSzama>4 | valaszokSzama<1) ){
					printf("Hibás adatbevitel\n");
			
			
					printf("kérjük irja be ujra");
						fflush(stdin); //a input puffer kiürítése
		
		
					}
			
	
						printf("Irja be a kerdest!: ");
						characters = getline(&buffer[0],&bufsize,stdin);
						printf("%zu characters were read.\n",characters);
						printf("A kerdes: '%s'\n",buffer[0]);
						
						
						
						 i =0;
						 osszKeredesSzam++;
						 
						 strcpy( osszesKerdes[aktualisKerdes].kerdes, buffer[0]);
						 
						for (i; i< valaszokSzama; i++)
						{
							printf("Irja be a %d. valasztasi lehetoseget!: ", (i+1));
							characters = getline(&buffer[i+1],&bufsize,stdin);
						
						
						}
						 i =1;
						 strcpy( osszesKerdes[aktualisKerdes].valasz1, buffer[1]);
						 strcpy( osszesKerdes[aktualisKerdes].valasz2, buffer[2]);
						if (valaszokSzama>=3) {strcpy( osszesKerdes[aktualisKerdes].valasz3, buffer[3]);}
						if (valaszokSzama==4) strcpy( osszesKerdes[aktualisKerdes].valasz4, buffer[4]);
						 strcpy( osszesKerdes[aktualisKerdes].valasz4, asctime(tm));
					
					//kiirni fajlba is!
					//tomb tombositese
					
					printf("\nAdatbevitel megtortént\n");
					break;
					
					
					case (2):
					//Listazas 
					printf("LISTAZAS");
						i =0;
					for (i; i < osszKeredesSzam; i++){
							printf("%d.edik kerdes\n", (i+1));
							
							printf("%s",  osszesKerdes[i].kerdes);	
							printf("%s",  osszesKerdes[i].valasz1);
							printf("%s",  osszesKerdes[i].valasz2);
							printf("%s",  osszesKerdes[i].valasz3);
							printf("%s",  osszesKerdes[i].valasz4);
							printf("%s",  osszesKerdes[i].datum);			
							printf("\n");
					}
					
				   /* book 1 specification */
				 
					
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
					
					
				    //	    printf("%s",  osszesKerdes[valasztottKerdes].kerdes);
						
					//		printf("%s",  osszesKerdes[valasztottKerdes].valasz1);
					//		printf("%s",  osszesKerdes[valasztottKerdes].valasz2);
					//		printf("%s",  osszesKerdes[valasztottKerdes].valasz3);
					//		printf("%s",  osszesKerdes[valasztottKerdes].valasz4);
					//		printf("%s",  osszesKerdes[valasztottKerdes].datum);
							
							
						//printf("A valasztott kerdes : %d", valasztottKerdes);
						
						//Listazas 
					system("clear");
					
					kerdeslistaVegleges= fopen(kerdeslistaVeglegesNev,"a+");	
					
					
							fprintf(kerdeslistaVegleges,"%s", osszesKerdes[valasztottKerdes].kerdes);
							fprintf(kerdeslistaVegleges,"%s", osszesKerdes[valasztottKerdes].valasz1);
							fprintf(kerdeslistaVegleges,"%s",osszesKerdes[valasztottKerdes].valasz2);
							fprintf(kerdeslistaVegleges,"%s", osszesKerdes[valasztottKerdes].valasz3);
							fprintf(kerdeslistaVegleges,"%s", osszesKerdes[valasztottKerdes].valasz4);
							fprintf(kerdeslistaVegleges,"%s",  osszesKerdes[valasztottKerdes].datum);
							
					
					
					fclose(kerdeslistaVegleges);	
								
					
					break;
			
					case (4):
					//modosítás
					
					//listazas
					//melyik kerdest szeretné modosíani
					
					gets(charMenupont);
					
					if (sscanf(charMenupont,"%d", &modositashozValasztottKerdes)!=1){
					printf("Hibás adatbevitel\n");
			
				
					printf("kérjük irja be ujra");
					fflush(stdin); //a input puffer kiürítése
		
		
					}
					 
					struct Kerdesek modositottKerdes;  
					
					
					
					
					break;
					
					case (5):
					//törlés
					system("clear");
					//printf("Melyik kerdest toroljuk?");
					
				
					
					// bekeres
		
					
		
					gets(charMenupont);
					
					if (sscanf(charMenupont,"%d", &torleshezValasztottKerdes)!=1){
					printf("Hibás adatbevitel\n");
			
			
					printf("kérjük irja be ujra");
						fflush(stdin); //a input puffer kiürítése
		
		
					}
					torleshezValasztottKerdes--;
					printf("\n valasztottkerdes %d\n", torleshezValasztottKerdes);			
					printf(" osszes kerdes szam %d\n", osszKeredesSzam);		
					if (torleshezValasztottKerdes>osszKeredesSzam){
						
							printf("NEM LEP BE A TORLESHEZ");
					}else{
					
					//helyfoglalás a beíráshoz
					printf("BELEP A TEORLESHEZ");
					int a=0;
					for (a=torleshezValasztottKerdes; a < osszKeredesSzam; a++){
							
							osszesKerdes[a]= osszesKerdes[a+1];
							
					}
					osszKeredesSzam--;
					}
					
						
					break;
					
					case (6):

						printf("kilépés parancs leütve \n");
					break;
					
					default: break;
				
			}
			
			
			

		
fflush(stdin);
} while (menupont!=6);





printf("sikeres kilépés");


}

 void kerdesekBeolvasasa(){
	
	
	
	
}

