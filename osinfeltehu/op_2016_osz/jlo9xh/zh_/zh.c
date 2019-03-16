#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
 
void handler() {
    printf("Beertem!\n");
}
void handler2() {
    printf("Bevontattak!\n");
}
 
 int main(int argc, char *argv[])
 {
	int max_szer;
	max_szer=atoi(argv[1]);
	
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler2);
	
	int fd[2];
	int fd2[2];
	
	if (pipe(fd)<0) {
        printf("Cso letrehozasa nem sikerult.");
        exit(EXIT_FAILURE);
    }
	
	if (pipe(fd2)<0) {
        printf("Cso letrehozasa nem sikerult.");
        exit(EXIT_FAILURE);
    }
	
	pid_t gyerek = fork();
	
	if(gyerek>0)
	{
		close(fd[0]);
		close(fd2[1]);
		srand(time(NULL));
		int rn=(rand() % max_szer)+1;
		write(fd[1], &max_szer, sizeof(int));
		write(fd[1], &rn, sizeof(int));
		close(fd[1]);		
		pause();
		
		char valasz[20];
		read(fd2[0],valasz,sizeof(valasz));
		printf("Szulo: %s\n",valasz);
		close(fd[0]);
		
		int status;
		wait(&status);
	}
	else
	{
		int max,sorszam;
		sleep(3);
		close(fd[1]);
		close(fd2[0]);
		
		read(fd[0],&max,sizeof(int));
		read(fd[0],&sorszam,sizeof(int));
		close(fd[0]);
		
		srand(time(NULL));
		int rn=rand() %100;
		if(10 > rn)
		{
			printf("%i. vagyok meg %i van utannam az uzem SIKERTELEN\n",sorszam,max-sorszam);
			write(fd2[1], "Nagy volt a ho!", 15);
			kill(getppid(), SIGUSR2);
		}
		else
		{
			printf("%i. vagyok meg %i van utannam az uzem SIKERES\n",sorszam,max-sorszam);
			write(fd2[1], "Jo volt az ut!", 14);
			close(fd2[1]);
			kill(getppid(), SIGUSR1);
		}
		
		
	}
	return 0;
 }