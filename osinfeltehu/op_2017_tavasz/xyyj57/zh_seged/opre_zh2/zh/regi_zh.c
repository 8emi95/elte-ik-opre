#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
//call it with some parameters from the command line 

int f;
int szavazokszama;
int fd[2];

void ellenorzo()
{
	kill(getppid(), SIGUSER1);
	printf("\Ellenorzo vagyok\n");
	close(fd[1]);
	while(read(fd[0],&azonosito,sizeof(int))
	{
		printf("Ellenor : %i\n",azonosito);
		fflush(stdout);
	}
	close(fd[0]);
	printf("Ellenorzo vagyok csobol olvasok");
}

void pecset()
{
	kill(getppid(), SIGUSER1);
	printf("\nPecsetelo vagyok\n");
	read(fd[0],%f,sizeof(int));
	close(fd[0]);
	printf("Pecsetelo olvas a csobol!");
}

void elnok()
{
	printf("Elnok vagyok\n");
	
	int i, azonosito;
	printf("Elnok vagyok");
	close(fd[0]);
	
	for(i=0;i<szavazokszama;++i)
	{
		azonosito=rand()%10000;
		write(fd[1]);
		
	}
}

volatile szamlalo=0;
void handler(int sn)
{
	szamlalo++;
}

int main(int argc,char ** argv) 
//char** means an array of character arrays = array of strings
{
	signal(SIGUSER1,handler);
	if(!2) perror("egy argumentum .... \n"); exit(1);
	szavazokszama=atoi(argv[1])

	if(pipe(fd)<0) perror("Hibas cso letrehozasa\n"); exit(EXIT_FAILURE);
	
	pid_t ellen=fork() //nagyobb mint 0
	if(ellen>0)
	{
		pid_t pecsetelo=fork();
		if(pecsetelo>0)
		{
			while(szamlalo!=2){;usleep;} // pause helyett, usleep vmennyi
			elnok();	// elnok dolgait csinalja
			wait(null); wait(null); // ua. wait mint gyerek folyamat!!!
		}
		else
		{
			pecset();
		}
	}
	else
	{
		
	}




 return 0;
}