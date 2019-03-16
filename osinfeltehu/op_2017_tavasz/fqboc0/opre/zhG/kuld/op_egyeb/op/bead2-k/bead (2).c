#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid

void handler(int signumber){
  printf("Mindegyik expedicio keszen all az indulasra!\n");
}

void handler2(int signumber){
  printf("%i. processz megérkezett.\n",signumber);
}

int main(int argc, char *argv[])
{
	signal(SIGUSR1,handler);
	signal(SIGTERM,handler2);
	
	pid_t pid;
	id_t ParentID;
	ParentID = getpid();
	//int status;


	int pipefd[2];
  	int sz;
  	int status;

	 if (pipe(pipefd) == -1)
   	{
       	perror("Hiba a pipe nyitaskor!");
       	exit(EXIT_FAILURE);
   	}
	
	int i;
	for(i = 0; i < 4; ++i)
	{
		if (getpid() == ParentID) // csak a parent folyamatban csinalsz uj/gyerek folyamatot
		{						  // ezzel eklerulod, hogy a x. gyerek is csinaljon meg n gyereket.... (4 gyerek helyett 8 pl...)
			pid = fork();
		}
	}


	if (pid == 0)   // CHILD
	{
		//printf("Megérkeztem!\n",SIGUSR1);
   	 	sleep(3);
   		kill(getppid(),SIGUSR1);

		close(pipefd[1]);  //Usually we close the unused write end
	           printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
               printf("Gyerek olvasta uzenet: %s",sz);
	           printf("\n");
               close(pipefd[0]); // finally we close the used read end


		 
    		//1. parameter the pid number of process, we send the signal
    		// 		if -1, then eacho of the processes of the same uid get the signal
   		 // 		we kill our bash as well! The connection will close
    		//2. parameter the name or number of signal
    		printf("Child process ended\n");  
	} 
	else if(pid > 0)// PARENT
	{
		printf("A miniszteri biztos felkeri a(z) %d expediciot az indulasra!\n", argv[1]);
		pause(); //waits till a signal arrive 
    		//printf("Signal arrived\n",SIGUSR1);

		close(pipefd[0]); //Usually we close unused read end
		write(pipefd[1], "Hajra Fradi!",13);
		close(pipefd[1]); // Closing write descriptor 
              printf("Szulo beirta az adatokat a csobe!\n");

		
    		int status;
    		wait(&status);
    		printf("Parent process ended\n");
	}
	sleep(1);
	return 0;
}