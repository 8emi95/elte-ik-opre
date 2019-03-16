#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h> // for pipe()

void jelentkezes(){
	printf("Jelentkezés\n");
	
	FILE * f;
	f=fopen("rendezvenyek.dat","r");
	if (f==NULL){perror("File opening error\n"); return;}
	
	int err = 1; // ha nem 0, akkor hiba van
	char nev[160];
	char email[160];
	int azon;
	
	while(err != 0){
		fseek(f, 0, SEEK_SET);
		
		err = 0;
		
		printf("Adja meg a vendég nevét: ");
		fgets(nev, 160, stdin);
		printf("Adja meg a vendég e-mail címét: ");
		fgets(email, 160, stdin);
		
		printf("Adja meg a rendezvény azonosító számát (nem lehet kisebb 1-nél): ");
		char azonIn[160];
		fgets(azonIn, 160, stdin);
		azon = atoi(azonIn);
		
		if(azon < 1){
			printf("\nNem megfelelő azonosító formátum\n");
			err = 1;
			continue;
		}
		
		err = 1;
		char line[160];
		while (fgets(line,sizeof(line),f)){
			
			char *fAzon;
			const char s[2] = ";";
			fAzon = strtok(line, s);
			
			if(azon == atoi(fAzon)){
				err = 0;
				break;
			}
		}
		if(err == 1){
			printf("\nNincs ilyen azonosítóval rendelkező rendezvény\n");
		}
		
	}
	fclose(f);
	
	FILE * f2;
	f2=fopen("vendegek.dat","w");
	if (f2==NULL){perror("File opening error\n"); return;}
	char line[160];
	int hanyadik = 0;
	while (fgets(line,sizeof(line),f2)){
		hanyadik++;
	}
	
	time_t most = time(0);
	
	fprintf(f2, "%s;%s;%i;%s\n", strtok(nev, "\n"), strtok(email, "\n"), azon, strtok(ctime(&most), "\n")); //strtok string végéről levágja az újsor jeleket
	printf("\nVendeg sorszama: %i\n", hanyadik);
	fclose(f2);
}

void vendegListazas(){
	printf("Vendégek listázása\n");
	
	FILE * f;
	f=fopen("vendegek.dat","r");
	if (f==NULL){perror("File opening error\n"); return;}
	char line[160];
	int hanyadik = 0;
	while (fgets(line,sizeof(line),f)){
		
		char * nev;
		char * email;
		char * azon;
		char * ido;
		
		char *token;
		const char s[2] = ";";
		token = strtok(line, s);
		nev = token;
		token = strtok(NULL, s);
		email = token;
		token = strtok(NULL, s);
		azon = token;
		token = strtok(NULL, s);
		ido = token;
		
		hanyadik++;
		
		printf("Név: %s\tE-mail cím: %s\tEsemény azonosítója: %s\tJelentkezés ideje: %s\tSorszám: %i\n", nev, email, strtok(azon, "\n"), strtok(ido, "\n"), hanyadik);
	}
	fclose(f);
}

void vendegModositas(){
	printf("Vendég adatainak módosítása\n");
	
	FILE * f;
	f=fopen("vendegek.dat","w");
	if (f==NULL){perror("File opening error\n"); return;}
	FILE * f2;
	f2=fopen("temp.dat","w+");
	if (f2==NULL){perror("File opening error\n"); return;}
	
	int err = 1; // ha nem 0, akkor hiba van
	char nev[160];
	char email[160];
	int azon;
	int sorszam;
	int opcio;
	
	while(err != 0){
		fseek(f, 0, SEEK_SET);
		fseek(f2, 0, SEEK_SET);
		
		err = 0;
		
		printf("Ha adatot szeretne módosítani, írja be: 1\nHa adatot törölni szeretne, írja be: 2\n");
		char opcioIn[160];
		fgets(opcioIn, 160, stdin);
		opcio = atoi(opcioIn);
		
		if(opcio != 1 && opcio != 2){
			printf("Nem megfelelő opció\n");
			err = 1;
			continue;
		}
		
		if(opcio == 1){
			printf("Adja meg a módosítandó vendég új nevét: ");
			fgets(nev, 160, stdin);
			printf("Adja meg a módosítandó vendég új e-mail címét: ");
			fgets(email, 160, stdin);
			
			printf("Adja meg a módosítandó vendéghez tartozó rendezvény új azonosító számát (nem lehet kisebb 1-nél): ");
			char azonIn[160];
			fgets(azonIn, 160, stdin);
			azon = atoi(azonIn);
			
			if(azon < 1){
				printf("\nNem megfelelő azonosító formátum\n");
				err = 1;
				continue;
			}
		}
		
		printf("Adja meg a módosítandó/törlendő vendég sorszámát (nem lehet kisebb 1-nél): ");
		char sorszamIn[160];
		fgets(sorszamIn, 160, stdin);
		sorszam = atoi(sorszamIn);
		
		if(sorszam < 1){
			printf("\nNem megfelelő sorszám formátum\n");
			err = 1;
			continue;
		}
		
		int sor = 0;
		err = 1;
		char line[360];
		while (fgets(line,sizeof(line),f)){
			sor++;
			
			if(sor == sorszam && opcio == 1){
				
				const char s[2] = ";";
				char * ido;
				char tmp[360];
				strcpy(tmp, line);
				strtok(tmp, s); strtok(NULL, s); strtok(NULL, s); ido = strtok(NULL, s);
				
				fprintf(f2, "%s;%s;%i;%s\n", strtok(nev, "\n"), strtok(email, "\n"), azon, strtok(ido, "\n"));
				err = 0;
			}
			else if(sor == sorszam && opcio == 2){
				err = 0;
			}
			else{
				fprintf(f2, "%s", line);
			}
		}
		if(err == 1){
			printf("\nNincs ilyen sorszámmal rendelkező vendég\n");
		}
		
	}
	fclose(f);
	fclose(f2);
	
	remove("vendegek.dat");
    rename("temp.dat", "vendegek.dat");
	
}

void ujRendezveny(){
	printf("Új rendezvény létrehozása\n");
	
	FILE * f;
	f=fopen("rendezvenyek.dat","w");
	if (f==NULL){perror("File opening error\n"); return;}
	
	int err = 1; // ha nem 0, akkor hiba van
	char nev[160];
	int azon;
	
	while(err != 0){
		fseek(f, 0, SEEK_SET);
		
		err = 0;
		printf("Adja meg az új rendezvény nevét: ");
		
		fgets(nev, 160, stdin);
		
		printf("Adja meg az új rendezvény azonosító számát (nem lehet kisebb 1-nél): ");
		char azonIn[160];
		fgets(azonIn, 160, stdin);
		azon = atoi(azonIn);
		
		if(azon < 1){
			printf("\nNem megfelelő azonosító formátum\n");
			err = 1;
			continue;
		}
		
		
		char line[160];
		while (fgets(line,sizeof(line),f)){
			char *fAzon;
			const char s[2] = ";";
			fAzon = strtok(line, s);
			
			if(azon == atoi(fAzon)){
				err = 1;
				printf("\nMár van ilyen azonosítóval rendelkező rendezvény.\n");
				break;
			}
		}
		
	}
	fprintf(f, "%i;%s", azon, nev);
	
	fclose(f);
}

void torolRendezveny(){
	printf("Rendezvény törlése\n");
	
	FILE * f;
	f=fopen("rendezvenyek.dat","w");
	if (f==NULL){perror("File opening error\n"); return;}
	FILE * f2;
	f2=fopen("temp.dat","w+");
	if (f2==NULL){perror("File opening error\n"); return;}
	
	int err = 1; // ha nem 0, akkor hiba van
	int azon;
	
	while(err != 0){
		fseek(f, 0, SEEK_SET);
		fseek(f2, 0, SEEK_SET);
		
		err = 0;
			
		printf("Adja meg a törlendő rendezvény azonosító számát (nem lehet kisebb 1-nél): ");
		char azonIn[160];
		fgets(azonIn, 160, stdin);
		azon = atoi(azonIn);
		
		if(azon < 1){
			printf("\nNem megfelelő azonosító formátum\n");
			err = 1;
			continue;
		}
		
		err = 1;
		char line[360];
		while (fgets(line,sizeof(line),f)){
			
			char *fAzon;
			const char s[2] = ";";
			char tmp[360];
			strcpy(tmp, line);
			fAzon = strtok(tmp, s);
			
			if(azon == atoi(fAzon)){
				err = 0;
			}
			else{
				fprintf(f2, "%s", line);
			}
		}
		if(err == 1){
			printf("\nNincs ilyen azonosítójú rendezvény\n");
		}
		
	}
	fclose(f);
	fclose(f2);
	
	remove("rendezvenyek.dat");
    rename("temp.dat", "rendezvenyek.dat");
}

void handler(int signumber){}

void partnerkezd(char *nev, char *fAzon){
	srand(time(NULL));
	printf("%s", nev);
	signal(SIGUSR1,handler);
	int pipefd[2];
	pid_t pid;
	char sz[160];

	if (pipe(pipefd) == -1){
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1){
	   perror("Fork hiba");
	   exit(EXIT_FAILURE);
	}

	if (pid == 0){                    // gyerek process
		sleep(1);
		read(pipefd[0],sz,sizeof(sz));
		char rendezveny[sizeof(sz)];
		strcpy(rendezveny, sz);
		write(pipefd[1], rendezveny, sizeof(rendezveny));
		kill(getppid(),SIGUSR1);
		pause();
		
		//névsort megkapja
		int n;
		read(pipefd[0], &n, sizeof(n));
		char nevsorpartner[n][160];
		int megjelent[n]; //1-igen 0-nem
		int m = 0;
		int i;

		for(i=0; i<n; i++){
			read(pipefd[0], nevsorpartner[i], sizeof(nevsorpartner[i]));
			if( rand()%100 < 10 ){megjelent[i] = 0; m++; }
			else megjelent[i] = 1;
		}

		//a rendezvény sikeressége
		int r = rand()%100;
		write(pipefd[1], &r, sizeof(r));
		write(pipefd[1], &m, sizeof(m));
		for(i=0; i<m; i++){
			write(pipefd[1], nevsorpartner[i], sizeof(nevsorpartner[i]));
		}
		kill(getppid(),SIGUSR1);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(0); //a gyerek processből ki kell lépni
	}
	else{    // szulo process
		printf("Partner kiküldése helyszínre...\n");
		write(pipefd[1], nev, sizeof(nev));
		pause();
		read(pipefd[0],sz,sizeof(sz));
		printf("A partner megérkezett a következő esemény helyszínére: %s\n", sz);
		
		// névsor begyűjtése
		char nevsor[320][160];
	
		FILE * f;
		f=fopen("vendegek.dat","r");
		if (f==NULL){perror("File opening error\n"); return;}
		char line[160];
		int hanyadik = 0;
		while (fgets(line,sizeof(line),f)){
			
			char * nev;
			char * azon;
			const char s[2] = ";";
			nev = strtok(line, s);
			strtok(NULL, s);
			azon = strtok(NULL, s);
			
				
			if(strcmp(fAzon, azon) == 0){
				strcpy(nevsor[hanyadik++], nev);
			}
		}
		fclose(f);
		//névsor megvan
		write(pipefd[1], &hanyadik, sizeof(hanyadik));
		int i;
		for(i=0; i<hanyadik; i++){
			write(pipefd[1], nevsor[i], sizeof(nevsor[i]));
		}
		kill(pid,SIGUSR1);
		printf("A szervező kiküldte a vendéglistát!\n");
		pause();
		
		//sikeresség
		int n;
		read(pipefd[0],&n,sizeof(n));
		printf("A rendezvény sikeressége: %i\n", n);
		
		//nem jöttek el
		printf("Akik nem jöttek el:\n");
		read(pipefd[0],&n,sizeof(n));
		for(i=0; i<n; i++){
			read(pipefd[0],sz,sizeof(sz));
			printf("%s\n", sz);
		}

		close(pipefd[0]);
		close(pipefd[1]);
		fflush(NULL);
		printf("Rendezvény lebonyolítva.\n");
	   
	}
	
}

void lebonyolitas(){
	
	FILE * f;
	f=fopen("rendezvenyek.dat","r");
	if (f==NULL){perror("File opening error\n"); return;}
	FILE * f2;
	f2=fopen("temp.dat","w+");
	if (f2==NULL){perror("File opening error\n"); return;}
	
	fseek(f, 0, SEEK_SET);
	fseek(f2, 0, SEEK_SET);
	
	char line[360];
	fgets(line,sizeof(line),f);
		
	char *fAzon;
	char *nev;
	const char s[2] = ";";
	char tmp[360];
	strcpy(tmp, line);
	fAzon = strtok(tmp, s);
	nev = strtok(NULL, s);
	printf("%s", nev);
	
	printf("Rendezvény lebonyolítása.\nA következő rendezvény: %s\nBiztos, hogy le akarod bonyolítani? (i/n)\n", nev);
	char valasz[160];
	fgets(valasz, 160, stdin);
	
	if(valasz[0] == 'i'){
		
		partnerkezd(nev, fAzon);
		
		char line[360];
		while (fgets(line,sizeof(line),f))
			fprintf(f2, "%s", line);
		
		fclose(f);
		fclose(f2);
	
		remove("rendezvenyek.dat");
		rename("temp.dat", "rendezvenyek.dat");
	}else{
		fclose(f);
		fclose(f2);
		remove("temp.dat");
	}
		
}

int main(){

  printf("King of Stands\n");
  
  char menupont[160];
  menupont[0] = '0';
  while(menupont[0] != '7'){  

    printf("\nÜdvözöljük a menüben, válasszon az alábbi opciók közül:\n");
    printf("1 - Jelentkezés rendezvényre\n2 - Jelentkezettek listázása\n3 - Vendég adatainak módosítása/törlése\n4 - Új rendezvény indítása\n5 - Rendezvény törlése\n6 - Következő rendezvény lebonyolítása\n7 - Kilépés\n");
  
    fgets(menupont, 160, stdin);
    printf("Választott menüpont: %s", menupont);

    switch(menupont[0]){
      
      case '1':
	    jelentkezes();
		break;
		
	  case '2':
	    vendegListazas();
		break;
		
	  case '3':
	    vendegModositas();
		break;
		
	  case '4':
	    ujRendezveny();
		break;
		
	  case '5':
	    torolRendezveny();
		break;
		
	  case '6':
	    lebonyolitas();
		break;
		
	  case '7':
	    printf("Kilépés\n");
		break;
		
	  default:
	    printf("Nem létező menüpont.\n");
	  
    }
  
  }

  printf("Viszlát!\n");

  return 0;
}
