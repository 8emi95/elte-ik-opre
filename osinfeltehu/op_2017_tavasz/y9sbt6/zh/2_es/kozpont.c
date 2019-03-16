#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>

#define READING (size_t)0
#define WRITING (size_t)1

int main(int argc, char *argv[])
{
	pid_t child;
	int status;
	char addr[100];
	
	while (1)
	{
		mkfifo("y9sbt6", S_IRUSR | S_IWUSR);
		
	printf("lakcim: ");
    fgets(addr, sizeof(addr), stdin);
	addr[sizeof(addr) - 1] = '\0';
	
	child = fork();	// creating parent-child processes
	if (child == -1) 
	{
	   perror("Fork hiba");
	   return 1;
	}

	if (child > 0) // szülő
	{
		int address_fifo = open("y9sbt6", O_RDONLY);
		char address[100];
		printf("kozpont: varom az utast\n");
		read(address_fifo, address, sizeof(address));
		printf("kozpont: az utas cime: %s\n", address);
		waitpid(child, &status, 0);
		close(address_fifo);
		unlink("y9sbt6");;
		
	}
	else if (child == 0) // gyerek
	{
		char* args[] = { "./utas", addr, NULL };
		execv("./utas", args);
	}
	}
	
	return 0;
}

 
