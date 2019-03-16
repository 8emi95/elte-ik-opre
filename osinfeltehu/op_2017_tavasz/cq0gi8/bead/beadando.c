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

#define FAJL "kerdes.data"
#define TMP "kerdes_temp.data"

struct kerdes_tipus{
	char kerdes[128];
	char valasz1[32];
	char valasz2[32];
	char valasz3[32];
	char valasz4[32];
	time_t date;
};

void fajl_ujrakeszites();
void listaz();
void listazkerdo2();
void listazkerdo();
void uj_kerdes(struct kerdes_tipus uj);
void kerdes_modositas(struct kerdes_tipus uj, int sor);
void torol(int sor);
const char * ido_formatum(time_t t);
void kerdoivberak(int sorszam);


int main(int argc,char** argv){

	char valasz[1];
	printf("\n\nELSO inditasnal hasznalja az 5-os\n\t'Fajl ujrairasa uresre' opciot!!!");
	
	do{
		
		
		
		
		
		printf("\n_________________________________\n");
		printf("Uj kerdes rogzitese: 1\n");
		printf("Kerdes modositasa: 2\n");
		printf("Kerdes torlese: 3\n");
		printf("Kerdesek listazasa: 4\n");
		printf("Fajl ujrairasa uresre.: 5\n");
		printf("Feltoltes regi adatokkal.: 6\n");
		printf("Kerdesek rogzitese kerdoivbe.: 7\n");
		printf("Kerdoiv listazasa: 8\n");
		printf("Kilepeshez nyomjon  X -et");
		
		
		printf("\n_________________________________\n");
		
		scanf("%s", valasz);
		
		
		

		
		if (strcmp(valasz, "1")==0){ // ----------------------------------------------------------
			struct kerdes_tipus uj;
			char s[128];
			printf("kerdes:\n");
			fgets(s, 128, stdin);
			scanf("%[^\n]s",&uj.kerdes);
			
			printf("1. valasz:\n");
			fgets(s, 128, stdin);
			scanf("%[^\n]s",&uj.valasz1);
			
			printf("2. valasz:\n");
			fgets(s, 128, stdin);
			scanf("%[^\n]s",&uj.valasz2);
			
			printf("3. valasz (lehet ures is)\n");
			fgets(s, 128, stdin);
			scanf("%[^\n]s",&uj.valasz3);
			
			printf("4. valasz (lehet ures is)\n");
			fgets(s, 128, stdin);
			scanf("%[^\n]s",&uj.valasz4);
			
			

			
			printf("atadott ertekek: %s\n%s\n%s\n%s\n%s",&uj.kerdes, &uj.valasz1, &uj.valasz2, &uj.valasz3, &uj.valasz4);
		
			
			uj.date = time(NULL);
			
			uj_kerdes(uj);
		}
		else if (strcmp(valasz, "2")==0){ // ----------------------------------------------------------
			int sor;
			printf("hanyadik kerdest modositsa:\n");
			scanf("%d",&sor);
			
			
			
			struct kerdes_tipus uj;
			char s[128];
			printf("kerdes:\n");
			fgets(s, 128, stdin);
			scanf("%[^\n]s",&uj.kerdes);
			
			printf("1. valasz:\n");
			fgets(s, 128, stdin);
			scanf("%[^\n]s",&uj.valasz1);
			
			printf("2. valasz:\n");
			fgets(s, 128, stdin);
			scanf("%[^\n]s",&uj.valasz2);
			
			printf("3. valasz (lehet ures is)\n");
			fgets(s, 128, stdin);
			scanf("%[^\n]s",&uj.valasz3);
			
			printf("4. valasz (lehet ures is)\n");
			fgets(s, 128, stdin);
			scanf("%[^\n]s",&uj.valasz4);
			
			uj.date = time(NULL);
			
			kerdes_modositas(uj, sor);
		}
		else if (strcmp(valasz, "3")==0){ // ----------------------------------------------------------
			int sor;
			printf("hanyadik kerdest torolje:\n");
			scanf("%d",&sor);
			torol(sor);
		}
		else if (strcmp(valasz, "4")==0){ // ----------------------------------------------------------
			listaz();
		}
		else if (strcmp(valasz, "5")==0){ // ----------------------------------------------------------
			fajl_ujrakeszites();
		}
		else if (strcmp(valasz, "6")==0){ // ----------------------------------------------------------
			struct kerdes_tipus uj;
			
			
			strcpy(uj.kerdes,"Véleménye szerint miért rossz a magyar foci?");
			strcpy(uj.valasz1,"Nincs elég lab a sok magyar focistához");
			strcpy(uj.valasz2,"Nem is rossz");
			strcpy(uj.valasz3,"Mert nincs eleg penzunk C. Ronaldohoz");
			strcpy(uj.valasz4,"");
			uj.date = time(NULL);
			
			struct tm * timeinfo;
			time ( &uj.date );
			timeinfo = localtime ( &uj.date );
			timeinfo->tm_year -= 3;
			timeinfo->tm_mon -= 1;
			
			uj.date = mktime ( timeinfo );
			
			uj_kerdes(uj);
			// --
			strcpy(uj.kerdes,"Milyen gyakran néz futballt?");
			strcpy(uj.valasz1,"2 naponta");
			strcpy(uj.valasz2,"Állandóan");
			strcpy(uj.valasz3,"Nem nézek");
			strcpy(uj.valasz4,"Azt ütővel játsszák?");
			uj.date = time(NULL);
			
			time ( &uj.date );
			timeinfo = localtime ( &uj.date );
			timeinfo->tm_year -= 3;
			timeinfo->tm_mon -= 1;
			
			uj.date = mktime ( timeinfo );
			
			uj_kerdes(uj);
			// --
			strcpy(uj.kerdes,"Mit vár egy magyar focistától?");
			strcpy(uj.valasz1,"Legyen szép a felesége");
			strcpy(uj.valasz2,"Legyen szép az én feleségem");
			strcpy(uj.valasz3,"Sört");
			strcpy(uj.valasz4,"Verje meg a C.Ronaldót");
			uj.date = time(NULL);
			
			time ( &uj.date );
			timeinfo = localtime ( &uj.date );
			timeinfo->tm_year -= 3;
			timeinfo->tm_mon += 1;
			
			uj.date = mktime ( timeinfo );
			
			uj_kerdes(uj);
			// --
			strcpy(uj.kerdes,"Hány forintot adna egy hivatásos focistának?");
			strcpy(uj.valasz1,"5 Ft");
			strcpy(uj.valasz2,"1 000 000 Ft");
			strcpy(uj.valasz3,"");
			strcpy(uj.valasz4,"");
			uj.date = time(NULL);
			
			time ( &uj.date );
			timeinfo = localtime ( &uj.date );
			timeinfo->tm_year -= 3;
			timeinfo->tm_mon += 1;
			
			uj.date = mktime ( timeinfo );
			
			uj_kerdes(uj);
			
			strcpy(uj.kerdes,"Teszt kerdes");
			strcpy(uj.valasz1,"Valasz1");
			strcpy(uj.valasz2,"Valasz2");
			strcpy(uj.valasz3,"Valasz3");
			strcpy(uj.valasz4,"Valasz4");
			uj.date = time(NULL);
			
			time ( &uj.date );
			timeinfo = localtime ( &uj.date );
			timeinfo->tm_year -= 3;
			timeinfo->tm_mon += 1;
			
			uj.date = mktime ( timeinfo );
			
			uj_kerdes(uj);

		} else if (strcmp(valasz, "7")==0){ // ----------------------------------------------------------
			printf("Adja meg a rogzitendo kerdesek sorszamait! (Nyomjon 0-at, ha vegzett!)\n");

			int sorszamok[32];
			int i=0;
			int sorszam=0;
			while(i<32) {
				scanf("%i",&sorszam);
				if(sorszam==0) {
					i = 33;
				} else {
					kerdoivberak(sorszam);
				}
				i++;
			}
			listazkerdo2();
			

		} else if (strcmp(valasz, "8")==0){ // ----------------------------------------------------------
			listazkerdo();
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
	
	if (remove("kerdoiv.data") == 0) printf("Fajl torolve\n");
	else printf("Nem talaltam fajlt, ami torolheto!\n");
	
	int tempFileAllapot = open("kerdoiv.data", O_WRONLY|O_CREAT, S_IRWXU);
	if(tempFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		printf("Fajl letrehozva!\n");
	}
	close(tempFileAllapot);
}

void listaz(){
	int beFileAllapot = open(FAJL, O_RDONLY);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		struct kerdes_tipus be_rekord;
		int sor = 0;
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct kerdes_tipus)) > 0){	
			sor++;
			printf("%i:%s\n\t1, %s\n\t2, %s\n\t3, %s\n\t4, %s\nHozzaadva: %s\n",sor, &be_rekord.kerdes, &be_rekord.valasz1, &be_rekord.valasz2, &be_rekord.valasz3, &be_rekord.valasz4, ido_formatum(be_rekord.date));
		}
	}
	close(beFileAllapot);
}

void listazkerdo2(){
	
	int beFileAllapot = open("kerdoiv.data", O_RDONLY);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		struct kerdes_tipus be_rekord;
		int sor = 0;
		
		int pipePtC[2]; //parent-to-child
		int pipeCtP[2]; //child-to-parent unnamed pipe file descriptor array
		pid_t pid;
		char sz[100];	// char array for reading from pipe szulonek
		char gy[100];	//gyereknek

		if (pipe(pipePtC) == -1) 
		{
			perror("Hiba a 'Szulo ir gyereknek' pipe nyitaskor!");
			exit(EXIT_FAILURE);
		}
		if (pipe(pipeCtP) == -1) 
		{
			perror("Hiba a 'gyerek ir szulonek' pipe nyitaskor!");
			exit(EXIT_FAILURE);
		}
			pid = fork();	// creating parent-child processes
			if (pid == -1) 
		{
			perror("Fork hiba");
			exit(EXIT_FAILURE);
		}

		if (pid == 0) {	// child process

			sleep(1);

			close(pipePtC[0]);  //olvaso
			close(pipeCtP[1]);	//iro	

			write(pipeCtP[1], "Ready",6);
			
			char str[sizeof(be_rekord.kerdes)];
			int i=0;
			
			for(i=0; i<3; ++i) {
				sleep(2);
				read(pipePtC[0],&str,sizeof(be_rekord.kerdes));
				printf("Gyerek olvasta uzenet:\n%s",str);
				printf("\n");
				
				int a=0; int b=0; int c=0; int d=0;
				int j;
				srand ( time(NULL) );
				int polgarok=rand()%10+11;
				printf("Gyerek: valaszok szama: %d\n", polgarok);
				for(j=0; j<polgarok; ++j) {
					int randk = rand()%4;
					if(randk==0) a++;
					if(randk==1) b++;
					if(randk==2) c++;
					if(randk==3) d++;
				}
				printf("Gyerek kuldi a: %d\n", a);
				write(pipeCtP[1], &a, sizeof(int));
				sleep(1);
				printf("Gyerek kuldi b: %d\n", b);
				write(pipeCtP[1], &b, sizeof(int));
				sleep(1);
				printf("Gyerek kuldi c: %d\n", c);
				write(pipeCtP[1], &c, sizeof(int));
				sleep(1);
				printf("Gyerek kuldi d: %d\n", d);
				write(pipeCtP[1], &d, sizeof(int));
			}
			
			close(pipePtC[0]); // finally we close the used read end
		} 
		else 
		{    // szulo process 
	
			int fp = open("kerdoiv.data", O_RDONLY);  //sorok megszamolasa randomhoz
			if (fp < 0)
			{
				printf("Could not open file");
			}
		 
			int sorokszama = 0;
			char c;
			struct kerdes_tipus buffer[sizeof(struct kerdes_tipus)];
			while (read(fp, &buffer, sizeof(struct kerdes_tipus)) > 0){
				sorokszama = sorokszama + 1;
			}
			close(fp);
			
			
			//printf("Szulo indul!\n");
			close(pipePtC[0]);	//irni fog ezert lezarom az olvasot
			close(pipeCtP[1]);	//olvasni fog ezert lezarom az irot

			read(pipeCtP[0], sz, sizeof(sz));
			printf("A gyerek azt uzeni : %s \n", sz);

			//random kerdes generalas
			srand ( time(NULL) );
			int r1=rand()%sorokszama+1;
			int r2=rand()%sorokszama+1; 
			int r3=rand()%sorokszama+1; 
			while(r2 == r1) r2=rand()%sorokszama+1;
			while(r3 == r1 || r3 == r2) r3=rand()%sorokszama+1;
			printf("Kerdesek szama:%i\n",sorokszama);
			sor=0;
			while (read(beFileAllapot, &be_rekord, sizeof(struct kerdes_tipus)) > 0){
				sor++;
				int val[4]={0,0,0,0};
				if(sor == r1 || sor == r2 || sor == r3) {
					write(pipePtC[1], &be_rekord.kerdes,sizeof(be_rekord.kerdes));
					printf("Szulo kuldi: %s\n",be_rekord.kerdes);

				
					int i=0;
					
					for(i=0; i<4; ++i) {
						read(pipeCtP[0], &val[i], sizeof(int));
						sleep(1);
					}
				sleep(1);
				printf("Valaszok:\n\tA: %d\n\tB: %d\n\tC: %d\n\tD: %d\n",val[0],val[1],val[2],val[3]);
				}
			}

			close(pipePtC[1]); // Closing write descriptor 
			fflush(NULL); 	// flushes all write buffers (not necessary)
			wait();		// waiting for child process (not necessary)
			// try it without wait()
			printf("Szulo befejezte!");	
		}
		exit(EXIT_SUCCESS);
		
		
	}
	close(beFileAllapot);
}



void listazkerdo(){
	int beFileAllapot = open("kerdoiv.data", O_RDONLY);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		struct kerdes_tipus be_rekord;
		int sor = 0;
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct kerdes_tipus)) > 0){	
			sor++;
			printf("\n%i:%s\n\t1, %s\n\t2, %s\n\t3, %s\n\t4, %s\nHozzaadva: %s\n\n",sor, &be_rekord.kerdes, &be_rekord.valasz1, &be_rekord.valasz2, &be_rekord.valasz3, &be_rekord.valasz4, ido_formatum(be_rekord.date));
		}
	}
	close(beFileAllapot);
}

void kerdoivberak(int sorszam){
	
	int beFileAllapot = open(FAJL, O_RDONLY);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else {
		
		struct kerdes_tipus be_rekord;
		int sor = 0;
		while (read(beFileAllapot, &be_rekord, sizeof(struct kerdes_tipus)) > 0){
			sor++;
			if(sor == sorszam) {
				int kerdoiv_allapot = open("kerdoiv.data", O_WRONLY|O_APPEND);
				
				if(kerdoiv_allapot < 0){
					printf("Hiba a kerdoiv megnyitasakor!\n");
				}
				else{
					if (write(kerdoiv_allapot, &be_rekord, sizeof(struct kerdes_tipus)) < 0){
						printf("Hiba a fajlba iraskor!\n");
					}
					else{
						printf("\nFelvitt adatok: %s|%s|%s|%s|%s|%s\n",&be_rekord.kerdes, &be_rekord.valasz1, &be_rekord.valasz2, &be_rekord.valasz3, &be_rekord.valasz4, ido_formatum(be_rekord.date));
					}
				}
				
				close(kerdoiv_allapot);
				

			}
		}
	}
	close(beFileAllapot);
	
}

void uj_kerdes(struct kerdes_tipus uj){
	int beFileAllapot = open(FAJL, O_WRONLY|O_APPEND);
	
	if(beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		if (write(beFileAllapot, &uj, sizeof(struct kerdes_tipus)) < 0){
			printf("Hiba a fajlba iraskor!\n");
		}
		else{
			printf("\nFelvitt adatok: %s|%s|%s|%s|%s|%s\n",&uj.kerdes, &uj.valasz1, &uj.valasz2, &uj.valasz3, &uj.valasz4, ido_formatum(uj.date));
		}
	}
	
	close(beFileAllapot);
}

void kerdes_modositas(struct kerdes_tipus uj, int sor){
	
	int tmpFileAllapot = open(TMP, O_WRONLY|O_CREAT, S_IRWXU);
	int beFileAllapot = open(FAJL, O_RDONLY);
	
	int hiba = 0;
	
	if(tmpFileAllapot < 0 || beFileAllapot < 0){
		printf("Hiba a fajl megnyitasakor!\n");
	}
	else{
		
		printf("Fajlok elokeszitve! (1/3)\n");
		
		struct kerdes_tipus be_rekord;
		int itt_tart_sor = 0;
		
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct kerdes_tipus)) > 0){	
		
			itt_tart_sor++;
			
			if (itt_tart_sor != sor){
				if(write(tmpFileAllapot, &be_rekord, sizeof(struct kerdes_tipus)) < 0){
					printf("Hiba tortent a %i. sor kiirasnal\n",itt_tart_sor);
					hiba = 1;
				}
				else{
					printf("Rekord mentve: %i\n",itt_tart_sor);
				}
			}
			else{
				uj.date = be_rekord.date;
				
				if (write(tmpFileAllapot, &uj, sizeof(struct kerdes_tipus)) < 0){
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
		
		struct kerdes_tipus be_rekord;
		int itt_tart_sor = 0;
		
	
		while (read(beFileAllapot, &be_rekord, sizeof(struct kerdes_tipus)) > 0){	
		
			itt_tart_sor++;
			
			if (itt_tart_sor == sor){
				// hagyja ki
				printf("Rekord megtalalva, torolve\n");
			}
			else{
				if (write(tmpFileAllapot, &be_rekord, sizeof(struct kerdes_tipus)) < 0){
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

const char * ido_formatum(time_t t){
	static char buff[20];
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));
	return buff;
}