#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

volatile szamlalo = 0;
void handler(int sn){
	++szamlalo;
}
void elnok(int szavazok[])
{
	printf("Elnok vagyok");
	int i;
	for(  i = 0; i < (sizeof(szavazok)/sizeof(szavazok[0])); ++i)
	{
		szavazok[i] = rand() % 100;
	}
}
void pecset()
{
	kill(getppid(), SIGUSR1);
	printf("\nPecsetelo vagyok\n");
}
void ellenorzo()
{
	kill(getppid(), SIGUSR1);
	printf("\n Ellenorzo vagyok\n");
}
int main(int argc, char *argv[])
{
	srand(time(NULL));
	signal(SIGUSR1,handler);
	pid_t ellen = fork();
	int szavazoAr[argc - 1];
	if( ellen > 0)
	{
		pid_t pecset = fork();
		if( pecset > 0)
		{
			while(szamlalo <= 2) { usleep(100);};
			elnok(szavazoAr);
			wait(NULL); wait(NULL);
		}
	}
}

