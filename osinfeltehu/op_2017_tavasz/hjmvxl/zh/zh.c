#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>

//SZÜLŐ == KÖZPONT
//UTAS == GYEREK1
//AUTÓS == GYEREK2

//A FELADAT == GYEREK1 ADJA ÁT KONZOL PARAMÉTERT A SZÜLŐNEK ÉS ÍRJA KI

struct utas {
	char* phone;
	char* address;
}

int main(int argc, char *argv[])
{
   int pipefd[2]; // unnamed pipe file descriptor array
   pid_t pid;
   char sz[100];  // char array for reading from pipe

   if (pipe(pipefd) == -1)
   {
	   perror("Hiba a pipe nyitaskor!");
	   exit(EXIT_FAILURE);
   }
   pid = fork();        // creating parent-child processes
   if (pid == -1)
   {
	   perror("Fork hiba");
	   exit(EXIT_FAILURE);
   }

   if (pid > 0)
	{                    // parent process
	   sleep(3);        // sleeping a few seconds, not necessary
	   close(pipefd[1]);  //Usually we close the unused write end
	   printf("Kozpont elkezdi olvasni a csobol az adatokat!\n");
	   read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
	   printf("Kozpont olvasta uzenet: %s",sz);
	   printf("\n");
	   
	   //wait for the end of child process
	   int status;
	   waitpid(pid,&status,0);
	   
	   close(pipefd[0]); // finally we close the used read end
   }
   else
   {    // gyerek process
	   printf("Utas indul!\n");
	   close(pipefd[0]); //Usually we close unused read end
	   write(pipefd[1], argv[1],13);
	   close(pipefd[1]); // Closing write descriptor
	   printf("Utas beirta az adatokat a csobe!\n");
	   fflush(NULL);    // flushes all write buffers (not necessary)
	   printf("Utas befejezte!\n");
	   kill(pid);
   }
   
   exit(EXIT_SUCCESS);  // force exit, not necessary
}