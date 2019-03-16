#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
//call it with some parameters from the command line 

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
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
		write(szulir[1],argv[0],1000);
		write(szulir[1],"feladat",1000);
		write(szulir[1],"3",1000);
		printf("irtam\n");
		pause();
		read(gyerir[0],siker,1000);
		printf("%s\n",siker);
		
		
	}else{
		//gyerekfolyamat
		char feladat[1000];
		char nev[1000];
		int szam=0;
		printf("olvasok\n");
		read(szulir[0],nev,1000);
		read(szulir[0],feladat,1000);
		read(szulir[0],&szam,1000);
		printf("olvastam\n");
		printf("Nev: %s\n",nev);
		printf("Nev: %s\n",feladat);
		printf("Nev: %d\n",szam);
		int random = rand() % 2;	
		if(random==0)
		{
			write(gyerir[1],"Sikerült",1000);
			printf("Sikerült\n");
		}
		else
		{
			write(gyerir[1],"Nem Sikerült",1000);
			printf("Nem Sikerült\n");
		}
		kill(getppid(),SIGTERM); 
		exit(EXIT_SUCCESS);
	}
	}
 return 0;
}