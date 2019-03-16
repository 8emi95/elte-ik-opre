#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
//call it with some parameters from the command line 

void handler(int signumber){
  printf("\nSignal with number %i has arrived\n",signumber);
}


int main(int argc,char ** argv) 

{
	
	if (argc>0)
	{
	signal(SIGTERM,handler);
	srand(time(NULL));
	int szulir[2];
	int gyerir[2];
	pid_t pid;
	

	if(pipe(szulir) == -1){
		perror("Hiba a cső megnyitásakor.");
		exit(EXIT_FAILURE);
	}
	
	if(pipe(gyerir) == -1){
		perror("Hiba a cső megnyitásakor.");
		exit(EXIT_FAILURE);
	}
	
	pid = fork();
	if(pid == -1){
		perror("Hiba a forknal.");
		exit(EXIT_FAILURE);
	}
	
	if(pid > 0){
		char siker[1000];
		char feladat[1000];
		int szam=3;
		strcpy(feladat,"feladat");
		write(szulir[1],argv[1],1000);
		write(szulir[1],feladat,1000);
		write(szulir[1],&szam,1000);
		pause();
		read(gyerir[0],siker,13);
		printf("%s\n",siker);
		wait();
		
		
		
	}else{
		//gyerekfolyamat
		char feladat[1000];
		char nev[1000];
		int szam=0;
		char sikergy[1000];
		read(szulir[0],nev,1000);
		read(szulir[0],feladat,1000);
		read(szulir[0],&szam,1000);
		printf("Nev: %s\n",nev);
		printf("Feladat: %s\n",feladat);
		printf("Szam: %d\n",szam);
		int random = rand() % 2;	
		if(random==0)
		{
			strcpy(sikergy,"Sikerült    ");
			write(gyerir[1],sikergy,13);
			printf("\nSikerült");
		}
		else
		{
			strcpy(sikergy,"Nem Sikerült");
			write(gyerir[1],sikergy,13);
			printf("\nNem Sikerült");
		}
		kill(getppid(),SIGTERM); 
		exit(EXIT_SUCCESS);
	}
	}
 return 0;
}