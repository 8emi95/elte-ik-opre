#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#include <signal.h>

struct versenyzo {
	char nev[128];
	int feladat;
	int sorszam;
};

void handler(int signumber)
{
	printf("Signal with number %i has arrived\n",signumber);
}

int main(int argc, char** argv)
{
	signal(SIGUSR1, handler);
	int Versenyzok = 5;

	struct versenyzo elso;
	//printf("Add meg a versenyzo nevet:\n");
	//char nev[128];
	//scanf("%127[^\n]",nev);
	if( argc < 2 ) {
		perror("Hiba nincs nev megadva!");
		return 0;
	}

	strcpy(elso.nev, argv[1]);
	elso.sorszam = Versenyzok+1;
	Versenyzok++;

	//** csovezetek
	int pipefd[2]; // unnamed pipe file descriptor array
	char uzenet[1024];
	if ( pipe(pipefd) == - 1 ) {
		perror("Hiba a pipe nyitasakor!");
		exit(EXIT_FAILURE);
	}
	// vissza csovezetek
	int rePipefd[2]; // unnamed pipe file descriptor array
	char vissza[1024];
	if ( pipe(rePipefd) == - 1 ) {
		perror("Hiba a masodik pipe nyitasakor!");
		exit(EXIT_FAILURE);
	}

	srand(time(NULL));

	//** fork
	pid_t  child=fork(); //forks make a copy of variables
	if (child<0){perror("The fork calling was not succesful\n"); exit(1);} 
	if (child>0) //the parent process, it can see the returning value of fork - the child variable!
	{
		close(pipefd[0]); //Usually we close unused read end
		close(rePipefd[1]); //Usually we close unused read end

		while(akt < Versenyzok ) {
			//elso.feladat = 3;
			elso.feladat = rand()%11;

			char trenerUzenet[1024];
			sprintf(trenerUzenet,"%d; %s; %d;", elso.feladat, elso.nev, elso.sorszam);
			printf("A trener kuldi a feladatott a versenyzonek.\n");
			write(pipefd[1], trenerUzenet, strlen(trenerUzenet)+1);
			close(pipefd[1]); // Closing write descriptor
			fflush(NULL); 	// flushes all write buffers (not necessary)

			pause();
			printf("A trener vette a jelzest.\n");


			read(rePipefd[0],vissza,sizeof(vissza)); // a read var addig mig a masik ir bele, ez egy szinkronizacios eszkoz
			close(rePipefd[0]); // finally we close the used read end
			printf("A trener olvasta a valaszt: %s\n", vissza);

			wait();
			akt++;
		}

		printf("A trener vege.\n\n");
	}
	else //child process
	{
		printf("Indul a versenyzo!\n");
		close(rePipefd[0]); //Usually we close unused read end
		close(pipefd[1]);  //Usually we close the unused write end
		read(pipefd[0],uzenet,sizeof(uzenet)); // a read var addig mig a masik ir bele, ez egy szinkronizacios eszkoz
		close(pipefd[0]); // finally we close the used read end
		char vNev[128];
		int vFel;
		int vNum;
		sscanf(uzenet,"%d; %[^;]; %d;", &vFel, &vNev, &vNum);
		printf("A versenyzo olvasta az uzenetet.\n");
		printf("A feladat: %d Neve: %s Sorszama: %d", vFel, vNev, vNum);
		printf("\n");

		int siker = rand()%2;
		//printf("Eredmeny: %d\n", siker);
		char eredm[32];
		if( siker ) {
			strcpy(eredm,"Siker");
		}
		else {
			strcpy(eredm,"Vesztett");
		}
		printf("A versenyzo kiirja az eredmenyt: %s\n", eredm);
		write(rePipefd[1], eredm, strlen(eredm)+1);
		close(rePipefd[1]); // Closing write descriptor
		fflush(NULL); 	// flushes all write buffers (not necessary)
		printf("A verenyzo vissza kuldte az eredmenyt.\n");

		printf("A versenyzo jelzest kuld.\n");
		kill(getppid(),SIGUSR1); 
		printf("A versenyzo vege.\n\n");
	}
}
