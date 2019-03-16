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


void  osszesKerdesfajlbaIrasa(int osszKeredesSzam, struct Kerdesek osszesKerdes[]);
void kerdesekBeolvasasa();
void kerdesekFelvitele(int osszKeredesSzam, struct Kerdesek osszesKerdes[]);

int osszKeredesSzam;
char *buffer[5];
size_t bufsize = 32;
size_t characters;
struct Kerdesek osszesKerdes[ 30 ];

int aktualisKerdes;
char charMenupont[10];
int i =0;
int valaszokszama;
char* kerdesekFileNev= "kerdesek.txt";
FILE * kerdeslista;


main(){


int menupont;
char* ajandekFileTmpNev = "kerdesekTemp.txt";
char* ajandekFileCsereNev = "csere.txt";
FILE * kerdeslistaIdeiglenes;
char* kerdeslistaVeglegesNev = "veglegesKerdesek.txt";
FILE* kerdeslistaVegleges;

char nap[5];
char honap[5];
char napDatuma[5];
char ora[10];
char ev [5];

int valasztottKerdes;
int torleshezValasztottKerdes;
int modositashozValasztottKerdes;
char charValasztottKerdes;
int valaszokSzama;
					
					
					
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
								
						
						/*	printf("STRUCT\n");
							printf("%s",  osszesKerdes[aktualisKerdes].kerdes);
							printf("%s",  osszesKerdes[aktualisKerdes].valasz1);
							printf("%s",  osszesKerdes[aktualisKerdes].valasz2);
							printf("%s",  osszesKerdes[aktualisKerdes].valasz3);
							printf("%s",  osszesKerdes[aktualisKerdes].valasz4);
						*/				
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
		printf("Vegleges lista torlese                   --- 6,ös menüpont\n");
		printf("Vegleges lista listazasa                 --- 7,ös menüpont\n");
		printf("\nKilépés                                --- 8,ös menüpont\n");		
		
//		if(scanf("%d", &menupont)!=1 | ( menupont>6 | menupont<1) ){
//			printf("Hibás adatbevitel\n");
//			printf("kérjük irja be ujra");
		
//			}
			
			
		//gets fuggvényel				
		gets(charMenupont);
		
		if (sscanf(charMenupont,"%d", &menupont)!=1 | ( menupont>8 | menupont<1) ){
			printf("Hibás adatbevitel\n");
			
			
			printf("kérjük irja be ujra");
			fflush(stdin); //a input puffer kiürítése
		
		
			}
			
	
			
			
			switch (menupont){
			
					case (1): 
					//KERDESEK FELVITELE
					printf("kerdes felvitele\n");
					
					kerdesekFelvitele(osszKeredesSzam,osszesKerdes);
					osszKeredesSzam++;
					//kiirni fajlba is!
					//tomb tombositese
					
					printf("\nAdatbevitel megtortént\n");
					osszesKerdesfajlbaIrasa(osszKeredesSzam,osszesKerdes );
					printf("\nKiiras megtortent\n");
					break;
					
					
					case (2):
					//Listazas 
					printf("LISTAZAS, osszesen %d kerdes\n ", osszKeredesSzam);
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
					printf("Melyik kerdest szeretne modositani?");
					gets(charMenupont);
					
					if (sscanf(charMenupont,"%d", &modositashozValasztottKerdes)!=1){
					printf("Hibás adatbevitel\n");
			
				
					printf("kérjük irja be ujra");
					fflush(stdin); //a input puffer kiürítése
		
		
					}
					 
					printf("%s",  osszesKerdes[modositashozValasztottKerdes-1].kerdes);
					printf("%s",  osszesKerdes[modositashozValasztottKerdes-1].valasz1);
					printf("%s",  osszesKerdes[modositashozValasztottKerdes-1].valasz2);
					printf("%s",  osszesKerdes[modositashozValasztottKerdes-1].valasz3);
					printf("%s",  osszesKerdes[modositashozValasztottKerdes-1].valasz4);
					printf("%s",  osszesKerdes[modositashozValasztottKerdes-1].datum);
					 
					kerdesekFelvitele(modositashozValasztottKerdes-1,osszesKerdes ); 
					
					osszesKerdesfajlbaIrasa(osszKeredesSzam,osszesKerdes );
					printf("\nKiiras megtortent\n");
					
					
					break;
					
					case (5):
					//törlés
					system("clear");
					printf("Melyik kerdest toroljuk?");
		
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
					
					kerdeslistaVegleges= fopen(kerdeslistaVeglegesNev,"w+");	
					fclose(kerdeslistaVegleges);
					printf("Kerdesek vegleges lista torolve");
					
					
					
					break;
					
					case (7):
					printf("veglegels lista listazasa: \n");
					
							
					kerdeslistaVegleges= fopen(kerdeslistaVeglegesNev,"rt");	
						
					if (kerdeslista == NULL) exit(EXIT_FAILURE);
					  
					while ((read = getline(&line, &len, kerdeslistaVegleges)) != -1) {
					//	printf("Retrieved line of length %zu :\n", read);
					
						printf("%s", line);
					
					}
					break;
					
					case (8):

					printf("kilépés parancs leütve \n");
					
					break;
					
					default: break;
				
			}
			
			
			

		
fflush(stdin);
} while (menupont!=8);





printf("sikeres kilépés");


}

void osszesKerdesfajlbaIrasa(int osszKeredesSzam, struct Kerdesek osszesKerdes[]){
	
				int i =0;
				
				
				kerdeslista= fopen(kerdesekFileNev,"w+");	
						
				
				for (i; i<osszKeredesSzam;i++ ){
					
					fprintf(kerdeslista,"%s", osszesKerdes[i].kerdes);
					fprintf(kerdeslista,"%s", osszesKerdes[i].valasz1);
					fprintf(kerdeslista,"%s",osszesKerdes[i].valasz2);
					fprintf(kerdeslista,"%s", osszesKerdes[i].valasz3);
					fprintf(kerdeslista,"%s", osszesKerdes[i].valasz4);
					fprintf(kerdeslista,"%s",  osszesKerdes[i].datum);
							
				
					
					
					
				}
					
					
					
					
					fclose(kerdeslista);
}

 void kerdesekBeolvasasa(){
	
	
	
	
}

void prepend(char* s, char t)
{

    memmove(s + 1, s, strlen(s) + 1);

		s[0]=t;
   
}


 void kerdesekFelvitele(int ujkerdes, struct Kerdesek osszesKerdes[]){
				

					int valaszokSzama;
					time_t t = time(NULL);
					struct tm *tm = localtime(&t);
					
					printf("\nKerem adja meg a valaszok szamat!");
					gets(charMenupont);
		
					if (sscanf(charMenupont,"%d", &valaszokSzama)!=1 | ( valaszokSzama>4 | valaszokSzama<1) ){
						printf("Hibás adatbevitel\n");
						printf("kérjük irja be ujra");
						fflush(stdin); //a input puffer kiürítése
						osszKeredesSzam--;
					}else{

					printf("Irja be a kerdest!: ");
					characters = getline(&buffer[0],&bufsize,stdin);
					printf("%zu characters were read.\n",characters);
					printf("A kerdes: '%s'\n",buffer[0]);
					i =0;
					
				
					
					strcpy( osszesKerdes[ujkerdes].kerdes,buffer[0]);
					 
					// osszesKerdes[ujkerdes].kerdes[0]='K';
					 prepend(osszesKerdes[ujkerdes].kerdes, 'K');
					 
					for (i; i< valaszokSzama; i++)
					{
							printf("Irja be a %d. valasztasi lehetoseget!: ", (i+1));
							characters = getline(&buffer[i+1],&bufsize,stdin);
						
						
					}
					i =1;
						 strcpy( osszesKerdes[ujkerdes].valasz1, buffer[1]);
						  prepend(osszesKerdes[ujkerdes].valasz1, 'K');
						 strcpy( osszesKerdes[ujkerdes].valasz2, buffer[2]);
						  prepend(osszesKerdes[ujkerdes].valasz2, 'V');
					   	 if (valaszokSzama>=3) {strcpy(osszesKerdes[ujkerdes].valasz3, buffer[3]);  prepend(osszesKerdes[ujkerdes].valasz3, 'V');}
						 if (valaszokSzama==4) {strcpy(osszesKerdes[ujkerdes].valasz4, buffer[4]); prepend(osszesKerdes[ujkerdes].valasz4, 'V');}
						 strcpy( osszesKerdes[ujkerdes].datum, asctime(tm));
						 prepend(osszesKerdes[ujkerdes].datum, 'D');
					}
}
