#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

void handler(int signumber){
  printf("Utas megerkezett\n");
}

int main(int argc, char ** argv)
{
	int pipe1[2];
	char szoveg[1000];

	if (pipe(pipe1) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	signal(SIGTERM,handler);
	
	pid_t utas=fork();
	if (utas<0) 
	{
		perror("Fork hiba");
		exit(1);
	}
	if (utas>0) //szülő
	{ 
		//várunk hogy megérkezzen az utas
		pause();
		
		//printf("Hozzarendelunk egy autot\n");
		
		int status;
		waitpid(utas,&status,0);		
		
		//kiírjuk a lakcimet
		close(pipe1[1]);
		read(pipe1[0],szoveg,sizeof(szoveg));
		printf("Kapott lakcim: ");
		printf("%s\n",szoveg);
		close(pipe1[0]);
	}
	else //gyerek
	{
		//jelzünk hogy megérkezett az utas
		kill(getppid(),SIGTERM); 
		
		sleep(3);
		
		strcpy(szoveg, "");
		//lakcím elküldése
		int i;
		for (i=1;i<argc;i++){
			strcat(szoveg, argv[i]);
			strcat(szoveg, " ");
		}
		close(pipe1[0]);
		write(pipe1[1], &szoveg, sizeof(szoveg));
		close(pipe1[1]);
		
		//befejezi a működését
		kill(getpid(),SIGKILL);
	}
}