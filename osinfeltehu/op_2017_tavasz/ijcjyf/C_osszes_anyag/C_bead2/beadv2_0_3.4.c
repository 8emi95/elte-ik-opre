#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>

struct Kerdesek {
    int   valaszokSzama;
   char  kerdes[100];
   char  valasz1[100];
   char  valasz2[100];
   char  valasz3[100];
   char  valasz4[100];
   char  datum[100];
   
   
   /// második részhez
   
  // int osszValasz1 =0;
 //  int osszValasz2 =0;
  // int osszValasz3 =0;
  // int osszValasz4 =0;
   
   int osszValasz[4];
   
};

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


int osszKeredesSzam;
char *buffer[6];
char *sorBuffer;
size_t bufsize = 32;
size_t characters;
struct Kerdesek osszesKerdes[ 30 ];

int kivalasztottKerdesekSzama;
struct Kerdesek kivalasztottKerdesek[30];

int randomKivalasztottKerdesekSorszamai[3];
struct Kerdesek randomKivalasztottKerdesek[3];

int aktualisKerdes;
char charMenupont[10];
int i =0;
char* kerdesekFileNev= "kerdesek.txt";
FILE * kerdeslista;

//int valaszokSzama; Torolheto?
main(){

kivalasztottKerdesekSzama=0;
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
 int pipefd[2];		
 
					
 srand(time(NULL));					
					i =0;
					
					//helyfoglalás a beíráshoz
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
					
					char * line = NULL;
					size_t len = 0;
					ssize_t readFile;
				/////////BEOLVASAS
				
				
					system("clear");
					aktualisKerdes=0;
				
						
						
						
						char t;
						char buffer[100];
						int fh;
						int bytes_read;
						fh = open("kerdesek.txt",O_RDONLY);
						printf ("File handle %d\n",fh);
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
									
									 osszKeredesSzam++;
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
					//fclose(kerdeslista);
				
				
				
					
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
					osszKeredesSzam++;
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
					kivalasztottKerdesek[kivalasztottKerdesekSzama] = osszesKerdes[valasztottKerdes];
					kivalasztottKerdesekSzama++;
					
					
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
					printf("veglegels lista listazasa: \n");
					
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
						
					 close(  fh );
					break;
					
					case (8):

					printf("kilépés parancs leütve \n");
					
					break;
					
				//	case (9):
				//	readFileL();
				//	break;
				
					case (10):
					KerdesekKiListazasa(kivalasztottKerdesekSzama,kivalasztottKerdesek );
					
					
					break;
					
					default: break;
				
			}
			
			
			

		
fflush(stdin);
} while (menupont!=8);


 int status;
 int notacommonvalue=1;
 char sz[100];
 
 
 pid_t  child=fork(); //forks make a copy of variables
 if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
 if (child>0) //the parent process,  it can see the returning value of fork - the child variable!
 {			  // MLSZ
	
	
 
    //waitpid(child,&status,0); 
	
	
	// MSZL KIVALASZT 3 KERDEST
	/*
	int r=rand()%10;
	randomKivalasztottKerdesekSorszamai[0]=r;
	printf("MLSZ elso valasztasa: %i\n",randomKivalasztottKerdesekSorszamai[0]);
   
   
    r=rand()%10;
	randomKivalasztottKerdesekSorszamai[1]=r;
   printf("MLSZ masodik valasztasa: %i\n",randomKivalasztottKerdesekSorszamai[1]);
   
   r=rand()%10;
   randomKivalasztottKerdesekSorszamai[2]=r;
	printf("MLSZ masodik valasztasa: %i\n",randomKivalasztottKerdesekSorszamai[2]);
	
	
	randomKivalasztottKerdesekSorszamai[0]=0;
	*/
	// pause
	//Várjuk a bejelentkezést
/*	 sleep(1);
	// csővezetékbe átküldjük:
	 close(pipefd[0]); //Usually we close unused read end
  //   write(pipefd[1], osszesKerdes[0],sizeof(osszesKerdes[0]));
	// write(pipefd[1], &osszesKerdes[0].valaszokSzama,sizeof(osszesKerdes[0].valaszokSzama) );
	 int kuld = 5;
     write(pipefd[1], &kuld,sizeof(kuld) );
  
     close(pipefd[1]); // Closing write descriptor 
	         // wait();
	*/
			   int alma=8; // szulo process 
               printf("Szulo indul!\n");
               close(pipefd[0]); //Usually we close unused read end
               write(pipefd[1], &alma ,sizeof(alma));
               close(pipefd[1]); // Closing write descriptor 
               printf("Szulo beirta az adatokat a csobe!\n");
               //fflush(NULL); 	// flushes all write buffers (not necessary)
             	// waiting for child process (not necessary)  
              
	// csővezetékbe visszakapjuk a valaszokat.
	
	
	// kiiratjuk
	
				//	valszokKiiratasa(kerdesSor1,1);
				//	valszokKiiratasa(kerdesSor2,2);
				//	valszokKiiratasa(kerdesSor3,3);
	

 
 }
 else //child process
	  // a kérdezőbiztos
 {
	 
	 	   int korte; 	// child process
	      	// sleeping a few seconds, not necessary
               close(pipefd[1]);  //Usually we close the unused write end
				printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0],&korte,sizeof(korte)); // reading max 100 chars
        
               close(pipefd[0]); // finally we close the used read end
				printf("Gyerek olvasta uzenet: %d \n "  ,korte);
	
	
	 
	 
	 
	 
	 
	 
	 
	 
	 /* printf("Thechild \n",notacommonvalue);
	  notacommonvalue=5; //it changes the value of the copy of the variable
  
	// jelzes signallal h készen állok a fogadasra
	
  
  // Structok inicializálása
	struct Kerdesek kerdesSor1;
	struct Kerdesek kerdesSor2;
	struct Kerdesek kerdesSor3;
	
	kerdesSor1.valaszokSzama=4;
	kerdesSor2.valaszokSzama=3;
	kerdesSor3.valaszokSzama=4;
	
	
	kerdesSor1.osszValasz[0]=0;
	kerdesSor2.osszValasz[0]=0;
	kerdesSor3.osszValasz[0]=0;
		
	kerdesSor1.osszValasz[1]=0;
	kerdesSor2.osszValasz[1]=0;
	kerdesSor3.osszValasz[1]=0;
	
	kerdesSor1.osszValasz[2]=0;
	kerdesSor2.osszValasz[2]=0;
	kerdesSor3.osszValasz[2]=0;
	
	kerdesSor1.osszValasz[3]=0;
	kerdesSor2.osszValasz[3]=0;
	kerdesSor3.osszValasz[3]=0;
/*	int n;
	// Kerdesek fogadasa pipal 
				int vSzam;
				close(pipefd[1]);  //Usually we close the unused write end
				printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
				//read(pipefd[0],&n,sizeof(n)); // reading max 100 chars
				
				char sz[100];
			//	read(pipefd[0],sz,sizeof(sz));  
				//read(pipefd[0],kerdesSor1,sizeof(kerdesSor1)); 
				
				read(pipefd[0],&vSzam,sizeof(vSzam)); 
				close(pipefd[0]);			
			   printf("Gyerek olvasta uzenet: %d",vSzam );
				printf("\n");
                 // finally we close the used read end
	
	*/
	
	
	
	
	
             
	
	/*
		
				int randomPolgarokSzama=(rand()%11)+10; //10-20 közötti szam
				printf("Megkerdezett polgarok szama: %i\n",randomPolgarokSzama);
   
					int i =0;
					for (i; i<randomPolgarokSzama ; i++)
					{		
					kerdesSor1 = polgarMegkerdezese(kerdesSor1);
						
					}
					i = 0;
					for (i; i<randomPolgarokSzama ; i++)
					{		
					kerdesSor2=polgarMegkerdezese(kerdesSor2);
						
					}
					i = 0;
					for (i; i<randomPolgarokSzama ; i++)
					{		
					kerdesSor3=polgarMegkerdezese(kerdesSor3);
						
					}
					
				*/
					
 //printf("Thechild \n",notacommonvalue);
 					
 }

 
 
 

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

 void KerdesekKiListazasa(int osszKeredesSzam, struct Kerdesek osszesKerdes[]){

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
							printf("Valaszok szama: %i",  osszesKerdes[i].valaszokSzama);								
							printf("\n");
							 ;
					}
	
	
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
						  
					   	 if (valaszokSzama>=3) {strcpy(osszesKerdes[ujkerdes].valasz3, buffer[3]); prepend(osszesKerdes[ujkerdes].valasz3, ' ');  prepend(osszesKerdes[ujkerdes].valasz3, '3'); prepend(osszesKerdes[ujkerdes].valasz3, 'V');}
						 if (valaszokSzama==4) {strcpy(osszesKerdes[ujkerdes].valasz4, buffer[4]);   prepend(osszesKerdes[ujkerdes].valasz4, ' '); prepend(osszesKerdes[ujkerdes].valasz4, '4'); prepend(osszesKerdes[ujkerdes].valasz4, 'V');}
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
