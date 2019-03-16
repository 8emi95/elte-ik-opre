#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>

struct Kerdesek {
    int   valaszokSzama;
   char  kerdes[100];
   char  valasz1[100];
   char  valasz2[100];
   char  valasz3[100];
   char  valasz4[100];
   char  datum[100];
   
   
   /// második részhez
   
   
   int osszValasz[4];
   
};

void handler(int signumber);
void readFileL();
void  osszesKerdesfajlbaIrasa(int osszKeredesSzam, struct Kerdesek osszesKerdes[]);
void kerdesekBeolvasasa();
void kerdesekFelvitele(int osszKeredesSzam, struct Kerdesek osszesKerdes[]);
void prepend(char* s, char t);
void KerdesekKiListazasa(int osszKeredesSzam, struct Kerdesek osszesKerdes[]);
// masodik reszhez
struct Kerdesek polgarMegkerdezese(struct Kerdesek kerdesSor);
//void randomValaszadas(struct Kerdesek kerdesSor);
void valszokKiiratasa(struct Kerdesek kerdesSor, int kerdesSorSzama);
int fajlbolStructba(struct Kerdesek osszesKerdes[], char filenev[], int osszKeredesSzam);


int osszKeredesSzam;
char *buffer[6];
char *sorBuffer;
size_t bufsize = 32;
size_t characters;
struct Kerdesek osszesKerdes[ 30 ];

int veglegesKerdesekSzama;
struct Kerdesek veglegesKerdesek[30];

int kivalasztottKerdesekSzama;

struct Kerdesek randomKivalasztottKerdesek[3];

int aktualisKerdes;
char charMenupont[10];
int i =0;
char* kerdesekFileNev= "kerdesek.txt";
FILE * kerdeslista;

//int valaszokSzama; Torolheto?
main(int argc, char *argv[]){

 veglegesKerdesekSzama=0;
int menupont;
char* ajandekFileTmpNev = "kerdesekTemp.txt";
char* ajandekFileCsereNev = "csere.txt";// törölhető?
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
//int valaszokSzama; Torolheto?
 //int pipefd[2];		
 	int fh;
					
 srand(time(NULL));					
					i =0;
					
					//helyfoglalás az uj kerdes beirasahoz
				for (i; i<6 ; i++)
						{
							buffer[i] = (char *)malloc(bufsize * (sizeof(char)));
							if( buffer[i] == NULL)
							{
								perror("Unable to allocate buffer");
								exit(1);
							}	
					}
					
					// helyfoglalás a kiratáshoz
					
			
			osszKeredesSzam= fajlbolStructba(osszesKerdes, "kerdesek.txt", 0);
			veglegesKerdesekSzama = fajlbolStructba(veglegesKerdesek, "veglegesKerdesek.txt", 0);
	
					
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
		printf("Kérdezobiztos kikuldese                  --- 8,ös menüpont\n");		
		
//		if(scanf("%d", &menupont)!=1 | ( menupont>6 | menupont<1) ){
//			printf("Hibás adatbevitel\n");
//			printf("kérjük irja be ujra");
		
//			}
			
			
		//gets fuggvényel				
		//gets(charMenupont);
		//char s[10];
		
		
		fgets( charMenupont, 10, stdin );
		
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
					
					//kiirni fajlba is!
					//tomb tombositese
					
					printf("\nAdatbevitel megtortént\n");
					osszesKerdesfajlbaIrasa(osszKeredesSzam,osszesKerdes );
					printf("\nKiiras megtortent\n");
					break;
					
					
					case (2):
					//Listazas 
					KerdesekKiListazasa(osszKeredesSzam,osszesKerdes );
					
				   /* book 1 specification */
					
					
					break;
			
					
					case (3): 
					//vegleges kerdesek kivalasztasa
					system("clear");
					
					KerdesekKiListazasa(osszKeredesSzam,osszesKerdes );
					
					printf("Irja be a vegleges kerdes sorszamat!");
					// bekeres
					fgets( charMenupont, 10, stdin );
					//gets(charMenupont);
					
					if (sscanf(charMenupont,"%d", &valasztottKerdes)!=1 ){
					printf("Hibás adatbevitel\n");
			
			
					printf("kérjük irja be ujra");
						fflush(stdin); //a input puffer kiürítése
		
		
					}
					
				
					system("clear");
					valasztottKerdes--;
					kerdeslistaVegleges= fopen(kerdeslistaVeglegesNev,"a+");	
					
					
					
					
					fprintf(kerdeslistaVegleges,"%s", osszesKerdes[valasztottKerdes].kerdes);
					fprintf(kerdeslistaVegleges,"%s", osszesKerdes[valasztottKerdes].valasz1);
					fprintf(kerdeslistaVegleges,"%s",osszesKerdes[valasztottKerdes].valasz2);
					fprintf(kerdeslistaVegleges,"%s", osszesKerdes[valasztottKerdes].valasz3);
					fprintf(kerdeslistaVegleges,"%s", osszesKerdes[valasztottKerdes].valasz4);
					fprintf(kerdeslistaVegleges,"%s",  osszesKerdes[valasztottKerdes].datum);
							
					
					
					fclose(kerdeslistaVegleges);	
					
					
					fh = open("veglegesKerdesek.txt",O_WRONLY|O_APPEND);
					
					/*
					write(fh,&osszesKerdes[valasztottKerdes].kerdes, sizeof(osszesKerdes[valasztottKerdes].kerdes));
					write(fh,&osszesKerdes[valasztottKerdes].valasz1, sizeof(osszesKerdes[valasztottKerdes].valasz1));
					write(fh,&osszesKerdes[valasztottKerdes].valasz2, sizeof(osszesKerdes[valasztottKerdes].valasz2) );
					write(fh,&osszesKerdes[valasztottKerdes].valasz3, sizeof(osszesKerdes[valasztottKerdes].valasz3) );
					write(fh,&osszesKerdes[valasztottKerdes].valasz4, sizeof(osszesKerdes[valasztottKerdes].valasz4) );
					write(fh,&osszesKerdes[valasztottKerdes].datum, sizeof(osszesKerdes[valasztottKerdes].datum) );
					
					close(fh);
					
					*/
					veglegesKerdesek[veglegesKerdesekSzama] = osszesKerdes[valasztottKerdes];
					veglegesKerdesekSzama++;
					
					
					break;
			
					case (4):
					//modosítás
					
					//listazas
					KerdesekKiListazasa(osszKeredesSzam,osszesKerdes );
					//melyik kerdest szeretné modosíani
					printf("Melyik kerdest szeretne modositani?");
					//gets(charMenupont);
					fgets( charMenupont, 10, stdin );
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
					KerdesekKiListazasa(osszKeredesSzam,osszesKerdes );
					printf("Melyik kerdest toroljuk?");
		
					//gets(charMenupont);
					fgets( charMenupont, 10, stdin );
					if (sscanf(charMenupont,"%d", &torleshezValasztottKerdes)!=1){
					printf("Hibás adatbevitel\n");
			
			
					printf("kérjük irja be ujra");
						fflush(stdin); //a input puffer kiürítése
		
		
					}
					
					printf("\n valasztottkerdes %d\n", torleshezValasztottKerdes);			
					printf(" osszes kerdes szam %d\n", osszKeredesSzam);	
					torleshezValasztottKerdes--;					
					if (torleshezValasztottKerdes>osszKeredesSzam){
						
						//	printf("NEM LEP BE A TORLESHEZ");
							
					}else{
					
					//helyfoglalás a beíráshoz
					//printf("BELEP A TEORLESHEZ");
					int a=0;
					for (a=torleshezValasztottKerdes; a < osszKeredesSzam-1; a++){
							
							osszesKerdes[a]= osszesKerdes[a+1];
							
					}
					osszKeredesSzam--;
					}
					osszesKerdesfajlbaIrasa(osszKeredesSzam,osszesKerdes );
						
					break;
					
					
					case (6):
					
					kerdeslistaVegleges= fopen(kerdeslistaVeglegesNev,"w+");	
					fclose(kerdeslistaVegleges);
					printf("Kerdesek vegleges lista torolve");
					
					
					
					break;
					
					case (7):
					
					///Fajl beolvasással, rendszerhívással:
					
					/*printf("veglegels lista listazasa: \n");
					
					printf("\n");
				    int fh;
				    char t;
					
					int bytes_read;
					fh = open("veglegesKerdesek.txt",O_RDONLY);
					
					int i =0;
				
					 //buffer[65];
					while ((bytes_read = read(fh, &t, 1)) > 0) {
						if (t=='K'){printf("\n");}
						printf ("%c",t );
						}
						
					 close(  fh );*/
					 
					///sima listázással
					 
					KerdesekKiListazasa(veglegesKerdesekSzama,veglegesKerdesek );
					
					
					break;
					 
					 
					break;
					
					case (8):

					printf("kilépés parancs leütve \n");
					
					break;
					
				//	case (9):
				//	readFileL();
				//	break;
				
				
					
					default: break;
				
			}
			
			
			

		
fflush(stdin);

 signal(SIGTERM,handler);

} while (menupont!=8);


		   int pipefd[2]; // unnamed pipe file descriptor array
		 
           pid_t pid;
           char sz[100];  // char array for reading from pipe

           if (pipe(pipefd) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
        		exit(EXIT_FAILURE);
           }
		   
	
	  
			pid = fork();	// creating parent-child processes
           if (pid == -1) 
	   {
               perror("Fork hiba");
               exit(EXIT_FAILURE);
           }

           if (pid == 0) 
	   {		   
	       // KERDEZOBIZTOS
		   int inedxx =0;
		   char varos[100];
		   for (inedxx ; inedxx <2; inedxx++){
				printf("Bejelentkezünk az MSZL-hez!\n");
				
				struct Kerdesek kapottKerdes1;
				struct Kerdesek kapottKerdes2;
				struct Kerdesek kapottKerdes3;
				
				kill(getppid(),SIGTERM); 				

				
				
			//	close(pipefd[1]);  //Usually we close the unused write end
		//		read(pipefd[0],&varos,sizeof(varos));
		//	printf("A kerdezobiztos megerkezett %s -be ! \n",varos );
				printf("A kerdezobiztos elkezdte olvasni a csobol az adatokat!\n");
				read(pipefd[0],&kapottKerdes1,sizeof(kapottKerdes1));
				read(pipefd[0],&kapottKerdes2,sizeof(kapottKerdes2));
				read(pipefd[0],&kapottKerdes3,sizeof(kapottKerdes3));
			//	 close(pipefd[0]);

            //   close(pipefd[0]); // finally we close the used read end
			   
			   	printf("%s \n ", kapottKerdes1.kerdes);
				printf("%s \n ", kapottKerdes2.kerdes);
				printf("%s \n ", kapottKerdes3.kerdes);
				 
				 // polgarok megkerdezese
			   
			   int randomPolgarokSzama=(rand()%11)+10; //10-20 közötti szam
			   printf("Megkerdezett polgarok szama: %i\n",randomPolgarokSzama);
   
					int i =0;
					for (i; i<randomPolgarokSzama ; i++)
					{		
						kapottKerdes1 = polgarMegkerdezese(kapottKerdes1);
						kapottKerdes2=polgarMegkerdezese(kapottKerdes2);
						kapottKerdes3=polgarMegkerdezese(kapottKerdes3);
					}
			
			  
			  // sleep(2); 
			  
			
			  
				write(pipefd[1], & kapottKerdes1 ,sizeof(kapottKerdes1));
			    write(pipefd[1], & kapottKerdes2 ,sizeof(kapottKerdes2));
			    write(pipefd[1], & kapottKerdes3 ,sizeof(kapottKerdes3));
				
				
					kill(getppid(),SIGTERM);
					
					pause();//sleep(3);
					
			}
		
			  
			  close(pipefd[0]);
			  close(pipefd[1]);
					
           } 
           else 
           {    // szulo process MLSZ 
				
				int index2 = 0;
				 for (index2 ; index2 <2; index2++){
			    printf("Kivalasztjuk random 3 kerdest !\n");
                //Usually we close unused read end
			//	printf("Varjuk a kerdezobiztos bejelentkezeset %s -bol!\n",  argv[1]);
			//	printf("Ennyi parancssori param van %i -bol!\n", argc);
			
				int randomSzamok[3];
				
				do {
					randomSzamok[2] = rand()%veglegesKerdesekSzama;
					randomSzamok[0] = rand()%veglegesKerdesekSzama;
					randomSzamok[1] = rand()%veglegesKerdesekSzama;
				}while (randomSzamok[2] ==randomSzamok[0] | randomSzamok[2] ==randomSzamok[1] | randomSzamok[0] ==randomSzamok[1]  );
	
				//	printf("Varjuk a kerdezobiztos bejelentkezeset %s -bol!\n",  argv[index2]);
				pause(); 
				printf("Bejelentkezett\n",SIGTERM);
				
				//close(pipefd[0]);
				//write(pipefd[1], & argv[index2+1] ,sizeof( argv[index2+1]));
			    write(pipefd[1], & veglegesKerdesek[randomSzamok[0]] ,sizeof(veglegesKerdesek[randomSzamok[0]]));
			    write(pipefd[1], & veglegesKerdesek[randomSzamok[1]] ,sizeof(veglegesKerdesek[randomSzamok[1]]));
			    write(pipefd[1], & veglegesKerdesek[randomSzamok[2]] ,sizeof(veglegesKerdesek[randomSzamok[2]]));
			    
				pause();
				printf("Bejelentkezett\n",SIGTERM);
				//   close(pipefd[1]); // Closing write descriptor 
				
				printf("Szulo beirta az adatokat a csobe!\n");
			   
			    read(pipefd[0], &  veglegesKerdesek[0] ,sizeof( veglegesKerdesek[0]));
				read(pipefd[0], & veglegesKerdesek[1] ,sizeof(veglegesKerdesek[1]));
				read(pipefd[0], &  veglegesKerdesek[2] ,sizeof( veglegesKerdesek[2]));
   
			    valszokKiiratasa( veglegesKerdesek[0],1);
			    valszokKiiratasa(veglegesKerdesek[1],2);
			    valszokKiiratasa( veglegesKerdesek[2],3);
				kill(pid,SIGTERM);
				 }
				//kill(pid,SIGTERM);
				close(pipefd[1]);
   			    close(pipefd[0]);
				}
				  
				  
			
			   
			  //  kill(getppid(),SIGTERM); 
							 
              // fflush(NULL); 	// flushes all write buffers (not necessary)
              // wait();		// waiting for child process (not necessary)
				// try it without wait()
	     //  printf("Szulo befejezte!\n");	
	 // }
 
 

//printf("sikeres kilépés");


}

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
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

 void KerdesekKiListazasa(int osszKeredesSzam, struct Kerdesek osszesKerdes[]){

	printf("LISTAZAS, osszesen %d kerdes\n ", osszKeredesSzam);
						i =0;
					for (i; i < osszKeredesSzam; i++){
							printf("%d.edik kerdes\n", (i+1));
							printf("\n");
							printf("%s",  osszesKerdes[i].kerdes);	
							printf("%s",  osszesKerdes[i].valasz1);
							printf("%s",  osszesKerdes[i].valasz2);
							printf("%s",  osszesKerdes[i].valasz3);
							printf("%s",  osszesKerdes[i].valasz4);
							printf("%s",  osszesKerdes[i].datum);
							printf("Valaszok szama: %i",  osszesKerdes[i].valaszokSzama);								
							printf("\n");
							printf("\n");
							 
					}
	
	
}
int fajlbolStructba(struct Kerdesek osszesKerdes[], char filenev[], int aktualisKerdes){
	
	
	
	system("clear");
						aktualisKerdes=0;
				
						char t;
						char buffer[100];
						int fh;
						int bytes_read;
						
						fh = open(filenev,O_RDONLY);
					//	printf ("File handle %d\n",fh);
						int i =0;	
						//sorBuffer =&buffer[0];
							 //buffer[65];
							 
						while ((bytes_read = read(fh, &t, 1)) > 0) {
							
							buffer[i] = t;
							i++;
					
							//printf ("%c",t );
						
							if (t=='\n'){
									
							
								i =0;
								
								if (buffer[0]=='K'){
									

								strcpy( osszesKerdes[aktualisKerdes].kerdes, buffer);
									 osszesKerdes[aktualisKerdes].valaszokSzama=0;
									// strcpy( osszesKerdes[aktualisKerdes].valasz1, sorBuffer);//??
								}else if(buffer[0]=='V'){
									
									switch (buffer[1]){
			
									case ('1'):
									 strcpy( osszesKerdes[aktualisKerdes].valasz1, buffer);
									 osszesKerdes[aktualisKerdes].valaszokSzama++;
									 break;
									
									case ('2'):
									 strcpy( osszesKerdes[aktualisKerdes].valasz2, buffer);
									 osszesKerdes[aktualisKerdes].valaszokSzama++;
									break;
									
									case ('3'):
									 strcpy( osszesKerdes[aktualisKerdes].valasz3, buffer);
									 osszesKerdes[aktualisKerdes].valaszokSzama++;
									break;
									
									case ('4'):
									strcpy( osszesKerdes[aktualisKerdes].valasz4, buffer);
									osszesKerdes[aktualisKerdes].valaszokSzama++;
									break;
									
									}
								}else if(buffer[0]=='D'){
								
								 strcpy( osszesKerdes[aktualisKerdes].datum, buffer);
								aktualisKerdes++;
								}
								
								
								memset(buffer, 0,65);
										
							}

						}
						
						  close(  fh );
	
	return aktualisKerdes;
	
}





void prepend(char* s, char t)
{

    memmove(s + 1, s, strlen(s) + 1);

		s[0]=t;
   
}

void readFileL() {
 
	char t;
	char buffer[65];
	int fh;
    int bytes_read;
    fh = open("kerdesek.txt",O_RDONLY);
    printf ("File handle %d\n",fh);
	int i =0;	
	sorBuffer =&buffer[0];
		 //buffer[65];
	while ((bytes_read = read(fh, &t, 1)) > 0) {
		
		buffer[i] = t;
		i++;
		printf ("%c",t );
		
		if (t=='\n'){
				
			printf ("sorveg!, %s \n",sorBuffer );
			i =0;
			
			memset(buffer, 0,65);
					
		}

	}
		
		
		 
		 
		/* while (gotten = read(fh,buffer,64)) {
                buffer[gotten] = '\0';
				
                printf("******%s",buffer);
                }*/
	
}





 void kerdesekFelvitele(int ujkerdes, struct Kerdesek osszesKerdes[]){
				
					osszKeredesSzam++;
					int valaszokSzama;
					time_t t = time(NULL);
					struct tm *tm = localtime(&t);
					
					printf("\nKerem adja meg a valaszok szamat!");
					fgets( charMenupont, 10, stdin );
					//gets(charMenupont);
		
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
					 prepend(osszesKerdes[ujkerdes].kerdes, ' ');
					 prepend(osszesKerdes[ujkerdes].kerdes, 'K');
					 
					for (i; i< valaszokSzama; i++)
					{		
				
							printf("Irja be a %d. valasztasi lehetoseget!: ", (i+1));
							characters = getline(&buffer[i+1],&bufsize,stdin);
						
						
					}
					i =1;
						 strcpy( osszesKerdes[ujkerdes].valasz1, buffer[1]);
						 prepend(osszesKerdes[ujkerdes].valasz1, ' ');
						  prepend(osszesKerdes[ujkerdes].valasz1, '1');
						  prepend(osszesKerdes[ujkerdes].valasz1, 'V');
						 strcpy( osszesKerdes[ujkerdes].valasz2, buffer[2]);
						 prepend(osszesKerdes[ujkerdes].valasz2, ' ');
						  prepend(osszesKerdes[ujkerdes].valasz2, '2');
						  prepend(osszesKerdes[ujkerdes].valasz2, 'V');
						  
						  strcpy(osszesKerdes[ujkerdes].valasz3, buffer[5]);
						  strcpy(osszesKerdes[ujkerdes].valasz4, buffer[5]);
						  
						  osszesKerdes[ujkerdes].valaszokSzama = 2;
						  
					   	 if (valaszokSzama>=3) {  osszesKerdes[ujkerdes].valaszokSzama++;strcpy(osszesKerdes[ujkerdes].valasz3, buffer[3]); prepend(osszesKerdes[ujkerdes].valasz3, ' ');  prepend(osszesKerdes[ujkerdes].valasz3, '3'); prepend(osszesKerdes[ujkerdes].valasz3, 'V');}
						 if (valaszokSzama==4) {osszesKerdes[ujkerdes].valaszokSzama++;strcpy(osszesKerdes[ujkerdes].valasz4, buffer[4]);   prepend(osszesKerdes[ujkerdes].valasz4, ' '); prepend(osszesKerdes[ujkerdes].valasz4, '4'); prepend(osszesKerdes[ujkerdes].valasz4, 'V');}
						 strcpy( osszesKerdes[ujkerdes].datum, asctime(tm));
						 prepend(osszesKerdes[ujkerdes].datum, ' ');
						 prepend(osszesKerdes[ujkerdes].datum, 'D');
					}
}


struct Kerdesek polgarMegkerdezese(struct Kerdesek kerdesSor){

    int r=rand()%kerdesSor.valaszokSzama;
   //printf("Random number %i\n",r);
   kerdesSor.osszValasz[r]++;
	
return kerdesSor;
}

void valszokKiiratasa( struct Kerdesek kerdesSor, int kerdesSorSzama){
	
			
		
							printf("%i.  kerdesre adott valaszok szama:\n", kerdesSorSzama);	
							
							int i = 0;
							for (i; i< kerdesSor.valaszokSzama; i++){
							
							printf("%i.es valaszt megjelolok: %d \n", i, kerdesSor.osszValasz[i]);	
								
							
							}
							printf("\n");
							printf("\n");
							
}
