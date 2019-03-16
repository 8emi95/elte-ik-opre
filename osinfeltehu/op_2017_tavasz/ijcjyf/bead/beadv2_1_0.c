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
   char  valasz[4][100];
  // char  valasz2[100];
  // char  valasz3[100];
  // char  valasz4[100];
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
char charMenupont[10];/// LEHET 2 HOSSZU?
int i =0;
char* kerdesekFileNev= "kerdesek.txt";
FILE * kerdeslista;

//int valaszokSzama; Torolheto?
main(int argc, char *argv[]){

	veglegesKerdesekSzama=0;
	int menupont;
	FILE * kerdeslistaIdeiglenes;
	char* kerdeslistaVeglegesNev = "veglegesKerdesek.txt";
	FILE* kerdeslistaVegleges;

	int valasztottKerdes;
	int torleshezValasztottKerdes;
	int modositashozValasztottKerdes;
	char charValasztottKerdes;
	
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
				
				
	
			
	osszKeredesSzam= fajlbolStructba(osszesKerdes, "kerdesek.txt", 0);
	printf("osszKeredesSzam %i!\n ", osszKeredesSzam);		
	veglegesKerdesekSzama = fajlbolStructba(veglegesKerdesek, "veglegesKerdesek.txt", 0);
	printf("veglegesKerdesekSzama %i!\n ", veglegesKerdesekSzama);	
					
	do {
		printf("Üdvözololjuk e nemzeti konzultacio Focis kiadasaban!\n");		
		printf("\n");
		printf("\n");
		printf("Kerem valasszon az alábbbi menüpontokból\n");
		printf("\n");
		printf("Uj kerdes-valasz felvitele               --- 1,es menüpont\n");
		printf("Megleveo kerdesek-valaszok listázása     --- 2,es menüpont\n");
		printf("Vegleges kerdes valasztasa               --- 3,as menüpont\n");
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
					
					printf("Irja be a valasztott vegleges kerdes sorszamat!");
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
					fprintf(kerdeslistaVegleges,"%s", osszesKerdes[valasztottKerdes].valasz[0]);
					fprintf(kerdeslistaVegleges,"%s",osszesKerdes[valasztottKerdes].valasz[1]);
					fprintf(kerdeslistaVegleges,"%s", osszesKerdes[valasztottKerdes].valasz[2]);
					fprintf(kerdeslistaVegleges,"%s", osszesKerdes[valasztottKerdes].valasz[3]);
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
					printf("%s",  osszesKerdes[modositashozValasztottKerdes-1].valasz[0]);
					printf("%s",  osszesKerdes[modositashozValasztottKerdes-1].valasz[1]);
					printf("%s",  osszesKerdes[modositashozValasztottKerdes-1].valasz[2]);
					printf("%s",  osszesKerdes[modositashozValasztottKerdes-1].valasz[3]);
					printf("%s",  osszesKerdes[modositashozValasztottKerdes-1].datum);
					 
					 osszKeredesSzam--;
					 
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
					// fajl torlese
					kerdeslistaVegleges= fopen(kerdeslistaVeglegesNev,"w+");	
					fclose(kerdeslistaVegleges);
					// struct torlese
				     veglegesKerdesekSzama=0;
					 
				  
					  veglegesKerdesekSzama=0;
					memset(veglegesKerdesek, 0, sizeof(veglegesKerdesek));
					printf("\nKerdesek vegleges lista torolve\n");
					
					
					
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
			
			
			

		

 
} while (menupont!=8);

fflush(stdin);

 signal(SIGTERM,handler);

 int i = 0;
 
 if (argc==1){
	 
	 printf("A vizsgabiztost nem kuldtuk sehova ( parancssori paraméterbe kell megadni varos(oka)t!)\n");
	  printf("Aprogram kilép\n");
	 exit(1);
 }
 
 if (veglegesKerdesekSzama<3){
	 
	 printf("Nincs minimum 3 végleges kerdes!\n");
	  printf("\nA program kilép\n");
	 exit(1);
 }
 
 
			int varosokSzama = argc-1;

		   int pipeMLSZ[2]; // unnamed pipe file descriptor array
		   int pipeKBiz[2];
		 
           pid_t pid;
           // char array for reading from pipe

           if (pipe(pipeMLSZ) == -1) 
			{
               perror("Hiba a pipe nyitaskor!");
        		exit(EXIT_FAILURE);
           }
		   
		      if (pipe(pipeKBiz) == -1) 
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
	       // KERDEZOBIZTOS- szulo---------------------------------------------------------------------------------
				int index1 =0;
				char varos[100];

				printf("Bejelentkezünk az MSZL-hez!\n");
				
				struct Kerdesek kapottKerdes1;
				struct Kerdesek kapottKerdes2;
				struct Kerdesek kapottKerdes3;
				
				kill(getppid(),SIGTERM); 				
				close(pipeKBiz[1]);
				close(pipeMLSZ[0]);
				
				for (index1 ; index1 <varosokSzama; index1++){
					
					printf("A kerdezobiztos elkezdte olvasni a csobol az adatokat!\n");
					char latogatottVaros[100];  
					read(pipeKBiz[0],latogatottVaros,30);
				
					printf("\n");
					printf("A kerdezobiztos elutazott %s-re! \n", latogatottVaros);
					printf("\n");
				
				//	sleep(2);
					read(pipeKBiz[0],&kapottKerdes1,sizeof(kapottKerdes1));
					read(pipeKBiz[0],&kapottKerdes2,sizeof(kapottKerdes2));
					read(pipeKBiz[0],&kapottKerdes3,sizeof(kapottKerdes3));
				
				//	sleep(1);
				
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
			  
					write(pipeMLSZ[1], & kapottKerdes1 ,sizeof(kapottKerdes1));
					write(pipeMLSZ[1], & kapottKerdes2 ,sizeof(kapottKerdes2));
					write(pipeMLSZ[1], & kapottKerdes3 ,sizeof(kapottKerdes3));
					
				}
		
		
				close(pipeKBiz[0]);
				close(pipeMLSZ[1]);
			 
					
         }else {    // szulo process MLSZ ----------------------------------------------------------------------------
				
				close(pipeKBiz[0]);
				close(pipeMLSZ[1]);
				int index2 = 0;
				
				pause();///suspend()
				//Varunk a vizsgabiztosrA
				
				for (index2 ; index2 <varosokSzama; index2++){
			    printf("Kivalasztjuk random 3 kerdest !\n");
        	
				int randomSzamok[3];
				
				do {
					randomSzamok[2] = rand()%veglegesKerdesekSzama;
					randomSzamok[0] = rand()%veglegesKerdesekSzama;
					randomSzamok[1] = rand()%veglegesKerdesekSzama;
				}while (randomSzamok[2] ==randomSzamok[0] | randomSzamok[2] ==randomSzamok[1] | randomSzamok[0] ==randomSzamok[1]  );
	
				//	int k =123;
				//	write(pipeKBiz[1],&k ,sizeof(k));
				// int len = strlen(argv[1]);
				//write(pipeKBiz[1],argv[1] ,len);
				// char valami[10]="valami";
				// int len = strlen(valami);
				//write(pipeKBiz[1],"valami" ,strlen);
			
			
				write(pipeKBiz[1], argv[index2+1],30);
			    write(pipeKBiz[1], & veglegesKerdesek[randomSzamok[0]] ,sizeof(veglegesKerdesek[randomSzamok[0]]));
			    write(pipeKBiz[1], & veglegesKerdesek[randomSzamok[1]] ,sizeof(veglegesKerdesek[randomSzamok[1]]));
			    write(pipeKBiz[1], & veglegesKerdesek[randomSzamok[2]] ,sizeof(veglegesKerdesek[randomSzamok[2]]));
			    
				printf("Szulo beirta az adatokat a csobe!\n");
			
			    read(pipeMLSZ[0], &  veglegesKerdesek[0] ,sizeof( veglegesKerdesek[0]));
				read(pipeMLSZ[0], & veglegesKerdesek[1] ,sizeof(veglegesKerdesek[1]));
				read(pipeMLSZ[0], &  veglegesKerdesek[2] ,sizeof( veglegesKerdesek[2]));
   
			    valszokKiiratasa( veglegesKerdesek[0],1);
			    valszokKiiratasa( veglegesKerdesek[1],2);
			    valszokKiiratasa( veglegesKerdesek[2],3);
				
				
				
				 }
				
				close(pipeKBiz[1]);
				close(pipeMLSZ[0]);
   			   
				}
				  
 

//printf("sikeres kilépés");


}

void handler(int signumber){
  printf("A vizsgabiztos Keszen all az utazásokra! \n",signumber);
}

void osszesKerdesfajlbaIrasa(int osszKeredesSzam, struct Kerdesek osszesKerdes[]){
	
				int i =0;
				
				
				kerdeslista= fopen(kerdesekFileNev,"w+");	
						
				
				for (i; i<osszKeredesSzam;i++ ){
					
					fprintf(kerdeslista,"%s", osszesKerdes[i].kerdes);
					fprintf(kerdeslista,"%s", osszesKerdes[i].valasz[0]);
					fprintf(kerdeslista,"%s", osszesKerdes[i].valasz[1]);
					fprintf(kerdeslista,"%s", osszesKerdes[i].valasz[2]);
					fprintf(kerdeslista,"%s", osszesKerdes[i].valasz[3]);
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
					
							//int k = 0;
							//for (k; k < osszesKerdes[i].valaszokSzama; k++){
							//printf("%s",  osszesKerdes[i].valasz[k]);
							//}
							
							printf(" %s",  osszesKerdes[i].valasz[0]);
							printf(" %s",  osszesKerdes[i].valasz[1]);
							printf(" %s",  osszesKerdes[i].valasz[2]);
							printf(" %s",  osszesKerdes[i].valasz[3]);
							
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
									 strcpy( osszesKerdes[aktualisKerdes].valasz[0], buffer);
									 osszesKerdes[aktualisKerdes].valaszokSzama++;
									 break;
									
									case ('2'):
									 strcpy( osszesKerdes[aktualisKerdes].valasz[1], buffer);
									 osszesKerdes[aktualisKerdes].valaszokSzama++;
									break;
									
									case ('3'):
									 strcpy( osszesKerdes[aktualisKerdes].valasz[2], buffer);
									 osszesKerdes[aktualisKerdes].valaszokSzama++;
									break;
									
									case ('4'):
									strcpy( osszesKerdes[aktualisKerdes].valasz[3], buffer);
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
						 strcpy( osszesKerdes[ujkerdes].valasz[0], buffer[1]);
						 prepend(osszesKerdes[ujkerdes].valasz[0], ' ');
						  prepend(osszesKerdes[ujkerdes].valasz[0], '1');
						  prepend(osszesKerdes[ujkerdes].valasz[0], 'V');
						 strcpy( osszesKerdes[ujkerdes].valasz[1], buffer[2]);
						 prepend(osszesKerdes[ujkerdes].valasz[1], ' ');
						  prepend(osszesKerdes[ujkerdes].valasz[1], '2');
						  prepend(osszesKerdes[ujkerdes].valasz[1], 'V');
						  
						  strcpy(osszesKerdes[ujkerdes].valasz[2], buffer[5]);
						  strcpy(osszesKerdes[ujkerdes].valasz[3], buffer[5]);
						  
						  osszesKerdes[ujkerdes].valaszokSzama = 2;
						  
					   	 if (valaszokSzama>=3) {  osszesKerdes[ujkerdes].valaszokSzama++;strcpy(osszesKerdes[ujkerdes].valasz[2], buffer[3]); prepend(osszesKerdes[ujkerdes].valasz[2], ' ');  prepend(osszesKerdes[ujkerdes].valasz[2], '3'); prepend(osszesKerdes[ujkerdes].valasz[2], 'V');}
						 if (valaszokSzama==4) {osszesKerdes[ujkerdes].valaszokSzama++;strcpy(osszesKerdes[ujkerdes].valasz[3], buffer[4]);   prepend(osszesKerdes[ujkerdes].valasz[3], ' '); prepend(osszesKerdes[ujkerdes].valasz[3], '4'); prepend(osszesKerdes[ujkerdes].valasz[3], 'V');}
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
	
							printf("A %i. kerdes: %s \n", kerdesSorSzama+1, kerdesSor.kerdes);	
						
		
							printf("%i.  kerdesre adott valaszok szama:\n", kerdesSorSzama+1);	
							
							int i = 0;
							
							for (i; i< kerdesSor.valaszokSzama; i++){
							printf("%i edik valasz: %s", i+1, kerdesSor.valasz[i]);	
							printf("%i.es valaszt megjelolok: %d \n", i+1, kerdesSor.osszValasz[i]);	
								
							
							}
							printf("\n");
							printf("\n");
							
}