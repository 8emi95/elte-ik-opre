#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <wait.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <signal.h>

char buff[100];

void handler(int signumber){
  printf("Valasz erkezett. Forras:  %i\n", signumber);
}

int ParentProc(int currentID, int child1Pid, int* child1Pipe, int* child1PipeBack, char* player)
{
	//parent
	
	//pipe:
	close(child1Pipe[0]);
	close(child1PipeBack[1]);

	strcpy(buff, "<<feladat>>");
	write(child1Pipe[1], buff, sizeof(buff) );

	strcpy(buff, player);
	write(child1Pipe[1], buff, sizeof(buff) );

	strcpy(buff, "1");
	// strcpy(buff, itoa(1));
	write(child1Pipe[1], buff, sizeof(buff) );

    sigset_t sigset;
    sigfillset(&sigset);
    sigdelset(&sigset,SIGTERM);

	//waiting for the childs to terminate:
	int status;
	waitpid(child1Pid,&status,0); 

	read(child1PipeBack[0], buff, sizeof(buff));
	printf("Sikeresseg: %s\n", 
		strcmp(buff, "successfull") ? "Sikerult" : "Nem sikerult");
	return strcmp(buff, "successfull") ? 1 : 0;

	close(child1Pipe[1]);
	close(child1PipeBack[0]);
}

void ChildProc(int* child1Pipe, int* child1PipeBack)
{
	//pipe
	close(child1Pipe[1]);
	close(child1PipeBack[0]);

	char job[100];
	char name[100];
	int orderNumber;
	char success[100];

	read(child1Pipe[0], buff, sizeof(buff));
	strcpy(job, buff);
	read(child1Pipe[0], buff, sizeof(buff));
	strcpy(name, buff);
	read(child1Pipe[0], buff, sizeof(buff));
	orderNumber = atoi(buff);
	strcpy(success ,rand() % 5 <= 4 ? "successfull" : "unsuccessfull");

	printf("\t%i. versenyzo (%s) feladata: %s. Eredmeny: %s\n", 
		orderNumber, name, job,
		strcmp(success, "successfull") ? "Sikerult" : "Nem sikerult");

	strcpy(buff, success);
	printf("\t ------- %s\n", buff);
	write(child1PipeBack[1], buff, sizeof(buff) );

	close(child1Pipe[0]);
	close(child1PipeBack[1]);

    kill(getppid(),SIGTERM);
}

int main(int argc, char* argv[])
{

  struct sigaction sigact;
  sigact.sa_handler=handler; //SIG_DFL,SIG_IGN
  sigemptyset(&sigact.sa_mask); //during execution of handler these signals will be blocked plus the signal    
  //now only the arriving signal, SIGTERM will be blocked
  sigact.sa_flags=0; //nothing special behaviour
  sigaction(SIGTERM,&sigact,NULL);
  sigaction(SIGUSR1,&sigact,NULL);

	if (argc < 2) {
		printf("Versenyzo(k) nevet kerem parancssori argumentumban megadni!\n");
		exit(1);
	}

	srand(time(NULL));
    int status; 
	//create pipe
	int child1Pipe[2];
	int child1PipeBack[2];
	if (pipe(child1Pipe) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	if (pipe(child1PipeBack) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	int i;
	char* fails[argc-1];
	int count = 0;
	for(i = 1; i <= argc; ++i) {
		pid_t child1 = fork(); 
		
		if (child1 < 0)
		{
			perror("fork"); 
			return 1; 
		}
		else if(child1 > 0)
		{
			if (ParentProc(	
				i, child1, child1Pipe, child1PipeBack, argv[i]) == 0)
			{
				fails[count] = argv[i];
				++count;
			} 
		}
		else
		{
			//child1
			ChildProc(child1Pipe, child1PipeBack);
		}

	}

	for(i = 0; i < count; ++i) { //bukottak újraküldése
		pid_t child1 = fork(); 
		
		if (child1 < 0)
		{
			perror("fork"); 
			return 1; 
		}
		else if(child1 > 0)
		{
			ParentProc(	
				i, child1, child1Pipe, child1PipeBack, argv[i]);
		}
		else
		{
			//child1
			ChildProc(child1Pipe, child1PipeBack);
		}
	}
	
	return 0;
}
