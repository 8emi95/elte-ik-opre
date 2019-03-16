#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

typedef enum{ FALSE = 0, TRUE = 1 } bool;
struct player{ char name[30]; int id; };

void createJob(char *result){
	const char *jobs[] = { "Logikai", "Ugyessegi", "Gyilkossagi" };
	strcpy(result,jobs[rand()%3]);
}

void handler(int number){
	printf("Sigaction: %i\n",number);
}

int main(int argc, char **argv){
	srand(time(NULL));
	if( argc < 2 ){ printf("ERROR: No name given in argument!\n"); exit(EXIT_FAILURE); }

	int pipefd[2][2],y;
	pid_t pid;
	struct sigaction sigact;

	sigact.sa_handler = handler;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags=0;
	sigaction(SIGUSR1,&sigact,NULL);

	char feladat[100],name[30];
	int id = 1;
	bool success;

	if( pipe(pipefd[0]) == -1 || pipe(pipefd[1]) == -1 ){
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}

	pid = fork();
	if( pid == -1 ){
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}

	if( pid == 0 ){
		sleep(3);
		close(pipefd[0][1]);
		close(pipefd[1][0]);

		int failedCount = 0, tryCount = 0;
		do{
		while( id < argc && tryCount < id+failedCount ){
			read(pipefd[0][0],feladat,sizeof(feladat));
			read(pipefd[0][0],name,sizeof(name));
			read(pipefd[0][0],&id,sizeof(id));
			printf("Feladat: %s\n",feladat);
			printf("Jatekos neve: %s\n",name);
			printf("Sorszama: %d\n",id);

			success = (rand() % 2 == 0 );
			if( success ) printf("A feladat sikeresen teljesitve!\n");
			else{ printf("A feladat teljesitese sikertelen!\n"); failedCount++; }

			write(pipefd[1][1],&success,sizeof(success));
			kill(getppid(),SIGUSR1);

			tryCount++;
		}}while( tryCount < id+failedCount );

		close(pipefd[0][0]);
		close(pipefd[1][1]);
	}else{    // szulo process 
		close(pipefd[0][0]);
		close(pipefd[1][1]);

		sigset_t sigset;
		sigfillset(&sigset);
		sigdelset(&sigset,SIGUSR1);

		struct player failedPlayers[argc];
		int unsuccessful = 0;

		while( id < argc ){
			createJob(feladat);
			strcpy(name,argv[id]);

			write(pipefd[0][1],feladat,sizeof(feladat));
			write(pipefd[0][1],name,sizeof(name));
			write(pipefd[0][1],&id,sizeof(id));

			sigsuspend(&sigset);

			read(pipefd[1][0],&success,sizeof(success));
			printf("\n");
			if(success) printf("A feladat teljesitese sikeres volt %s (sorszama: %d) jatekos altal.\n",name,id);
			else{
				printf("Sikertelen probalkozas %s (sorszama: %d) jatekos altal.\n",name,id);
				strcpy(failedPlayers[unsuccessful].name,name);
				failedPlayers[unsuccessful].id = id;
				unsuccessful++;
			}

			id++;
		}

		printf("\n\n");
		int i;
		for(i = 0; i < unsuccessful; i++){
			createJob(feladat);

			write(pipefd[0][1],feladat,sizeof(feladat));
			write(pipefd[0][1],failedPlayers[i].name,sizeof(failedPlayers[i].name));
			write(pipefd[0][1],&failedPlayers[i].id,sizeof(failedPlayers[i].id));

			sigsuspend(&sigset);

			read(pipefd[1][0],&success,sizeof(success));
			if(success) printf("%s (sorszama: %d) masodik probalkozasra teljesitette a feladatot!\n",failedPlayers[i].name,failedPlayers[i].id);
			else printf("%s (sorszama: %d) masodik probalkozasra sem teljesitette a feladatot!\n",failedPlayers[i].name,failedPlayers[i].id);
			printf("\n");
		}

		close(pipefd[0][1]);
		close(pipefd[1][0]);

		fflush(NULL);
		wait(NULL);
	}
}

 
