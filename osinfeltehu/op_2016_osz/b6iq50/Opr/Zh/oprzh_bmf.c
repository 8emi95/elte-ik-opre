#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

#define CSONEV "egyedicso_b6iq50.ftc"

#define FAJL "bmf.data"
#define TMP "bmf_temp.data"

/*
szülő - gyerek
gyerek megkapja, hogy ő hanyadik szerelvény (1-random)
	és megkapja hogy hány szerelvény van (paranscsorról)
gyerek 10%-kal elcsúszik vagy lefagy
gyerek kiírja hogy ő hanyadik szerelvény
		még hány lesz utána
		illetve, hogy célba ért-e
gyerek (ha végzett) üzen a szülőnek, hogy sikeresen célba ért-e vagy bevontatták (utóbbi esetben ok)
*/

int pipefd[2];
int szerelvenyek_db_szama=0;
int sajat_szer_szam=0;
int szerelvenyek_szama=0;

volatile gyerek_olvashat = 0;
volatile szulo_olvashat = 0;

void gyerek_indul(){
	gyerek_olvashat=1;
}

void szulo_indul(){
	szulo_olvashat=1;
}

const char * ido_formatum(time_t t);

int main(int argc,char** argv){
	
	szerelvenyek_db_szama = atoi(argv[1]);
	//int szerelvenyek_db_szama = 5;
	
	if (pipe(pipefd) == -1)
	{
		perror("Hiba a cso nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	signal(SIGUSR1, gyerek_indul);
	signal(SIGUSR2, szulo_indul);
	
	pid_t pid = fork();
	printf("Folyamat elindult.\n");
	
	if(pid == 0){
		printf("Gyerek elindult, es var.\n");
		
		while(gyerek_olvashat==0){
			printf("...\n");
		} //fut amíg nem a gyerek nem olvashat
		printf("Gyerek tovabb haladt.\n");

		printf("Gyerek olvas a csobol.\n");
		read(pipefd[0], &sajat_szer_szam, sizeof(&sajat_szer_szam));
		read(pipefd[0], &szerelvenyek_szama, sizeof(&szerelvenyek_szama));
		
		//printf("sajat_szer_szam : %i\n", sajat_szer_szam);
		//printf("szerelvenyek_szama : %i\n", szerelvenyek_szama);
		//gyerek randomol, hogy a szerelvény épségben beér-e a végállomásra
		char beert_e[42];
		
		srand(time(NULL));
		int r = rand() % 100; //random int 0 és 99 között
		//printf("r jelenleg: %i\n", r);
		if(r >=89){
			
			strcpy(beert_e,"A szerelvenyt bevontattak a kovetkezo okkal: ");
			
			if(r>=94){
				strcpy(beert_e,"Elcsuszott egy bananhejon.\n");
			}else{strcpy(beert_e,"Lefagyott a telben.\n");}
			
		}else{strcpy(beert_e, "A szerelveny sikeresen megerkezett a vegallomasra.\n");}
		
		int varhato_szerleveny_szam=szerelvenyek_szama-sajat_szer_szam;
		printf("A gyerek jelenleg a %i. szerelveny, es utana meg %i szerelveny varhato.\n", sajat_szer_szam, varhato_szerleveny_szam);
		printf("%s", beert_e);
		
		printf("Gyerek atadja a jelentest a szulonek.\n");
		write(pipefd[1], beert_e, sizeof(beert_e));
		fflush(NULL);
		kill(pid, SIGUSR2);
		
		printf("Gyerek leallt.\n");
		
	}else{
		printf("Szulo elindult.\n");
		printf("Szulo kiosztja a szerelvenyszamokat.\n");
		
		srand(time(NULL));
		int aktualis_szer_szam=(rand() % szerelvenyek_db_szama)+1;
		//printf("aktualis_szer_szam : %i\n",aktualis_szer_szam);
		//printf("szerelvenyek_db_szama : %i\n",szerelvenyek_db_szama);
		
		printf("Szülő atadja a gyereknek a szamat, es hogy osszesen hany szerelveny van.\n");
		write(pipefd[1], &aktualis_szer_szam, sizeof(&aktualis_szer_szam));
		write(pipefd[1], &szerelvenyek_db_szama, sizeof(&szerelvenyek_db_szama));
		fflush(NULL);
		kill(pid, SIGUSR1);
		
		while(szulo_olvashat==0){
			printf(".....\n");
		} //fut amíg nem a gyerek nem olvashat
		printf("Szulo tovabb haladt.\n");
		printf("Szulo olvas a csobol.\n");
		char szerelveny_jelentes[84];
		read(pipefd[0], szerelveny_jelentes, sizeof(szerelveny_jelentes));
		printf("Szulo megkapta az uzenetet: %s\n", szerelveny_jelentes);
	}
	
	close(pipefd[0]);
	close(pipefd[1]);
	
	return 0;
}


const char * ido_formatum(time_t t){
	static char buff[20];
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));
	return buff;
}