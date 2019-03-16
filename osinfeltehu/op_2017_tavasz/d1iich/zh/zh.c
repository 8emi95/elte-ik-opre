#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  //fork miatt 
#include <sys/wait.h> //pid miatt
#include <errno.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

void handler(int signumber)
{
  signal(SIGTERM,SIG_DFL);
}

int main(int argc, char* argv[])
{
	if (argc < 2) 
	{
    printf("Nincs cim!\n");
    exit(1);
	}
	
	int csovezetekhelye[2];
    char tempcsostring[100];
	char cim[15];
	pid_t utas; //gyerekfolyamat
	pid_t sofor;
	signal(SIGTERM,handler);
	if (strlen(argv[1]) >= sizeof(cim)) 
	{
    printf("Tul hosszu cim: %s\n", argv[1]);
    exit(1);
	}
	strcpy(cim, argv[1]);


	if (pipe(csovezetekhelye) == -1) 
	{
        perror("Hiba a csővezeték nyitásakor!");
    }
	utas = fork();
	
	if(utas > 0 )	//központ
	{	
		printf("[KP]Az utas cimere varok\n");
		close(csovezetekhelye[1]);
		read(csovezetekhelye[0],tempcsostring,sizeof(tempcsostring));
		printf("[KP]Az utas cime: %s\n",tempcsostring);
		int status;
		wait(&status);
		printf("KP vege.\n");
		
	}else
	{
		close(csovezetekhelye[0]);
		write(csovezetekhelye[1],cim,strlen(cim));
        close(csovezetekhelye[1]);
		printf("UTAS vege.\n");
	}
	
	return 0;
}