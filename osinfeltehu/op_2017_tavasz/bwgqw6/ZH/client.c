#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>


struct Client
{
	char address[100];
	pid_t service_car;
};
static int currentClient = 0;

void handler(int sig) {
	if (sig == SIGUSR1) {
		printf("Parent:> Child signal received processing order..\n");
	}
}

int main(int argc, char** argv) 
{
	pid_t cpid;
	pid_t service_car;
	int status;
	int pipefd[2];
	
	struct Client clients[50];
	char address_buffer[100];
	
	if (pipe(pipefd) == -1) 
	{
		perror("pipe error");
        exit(EXIT_FAILURE);
	}
	cpid = fork();	// creating parent-child processes
    if (cpid == -1) 
	{
		perror("Fork hiba");
		exit(EXIT_FAILURE);
    }
	if (cpid == 0) 
	{
		/* child */
		printf("Child:> Gyerek letrejott, varok 3 masodpercet majd kuldom a signalt...\n");
		sleep(3);
		kill(getppid(),SIGUSR1);
		printf("Child:> Kuldom a signalt a szulonek! Varom a visszajelzést\n");	
		sleep(3);
			
		close(pipefd[0]);
		write(pipefd[1], argv[1],sizeof(argv[1]));
		close(pipefd[1]);
		printf("Child:> Gyerek beirta az adatokat a csobe!\n");
		printf("Child:> Gyerek befejezte!\n");
	} 
    else 
    {    	
		// szulo process 
		signal(SIGUSR1,handler);
		pause();
		printf("Parent:> Megkaptam a jelet, megvarom mig a gyerek befejezi!\n");
		waitpid(cpid,&status,0); 
		close(pipefd[1]);
		printf("Parent:> Szulo elkezdi olvasni a csobol az adatokat!\n");
		read(pipefd[0],address_buffer,sizeof(address_buffer));
		printf("Parent:> A kapott cím: %s\n",address_buffer);
		strcpy(clients[currentClient].address, address_buffer);
		clients[currentClient].service_car = service_car;
		close(pipefd[0]);
		printf("Parent:> A(z) %d azonositoju es %s cimu klienst taroltam!\n",currentClient,address_buffer);
		++currentClient;
	}
}