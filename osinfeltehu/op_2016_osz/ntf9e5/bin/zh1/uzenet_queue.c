#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

struct uzenet{
	long mtype;
	char mtext[1024];
};

int main(int argc, char** argv)
{  
	int uzenetsor, status;
	key_t kulcs;
	
	kulcs = ftok(argv[0], 1);
	uzenetsor = msgget( kulcs, 0600 | IPC_CREAT);
	if (uzenetsor < 0){
		perror("msgget\n");
		return 1;
	}

  char message[50];
  char message_read[50];

	int firstpipe[2];
	int secondpipe[2];
 if (pipe(firstpipe) == -1) 
 {
   perror("Hiba a pipe nyitaskor!");
   exit(EXIT_FAILURE);
 }
 if (pipe(secondpipe) == -1) 
 {
   perror("Hiba a pipe nyitaskor!");
   exit(EXIT_FAILURE);
 }
 
 
 
 
 pid_t child1 = fork();
  if(child1 < 0)
  {
    printf("Error.\n");
  }
  else if (child1 > 0)
  {
    pid_t child2 = fork();
    if (child2 < 0)
    { 
       printf("Error\n");
    }
    else if (child2 > 0)
    {
		   printf("Parent Started\n");
		 //  sleep(6);
		   
		  struct uzenet uz = { 3, "Pentek van!" };
		 struct uzenet uz2 = { 6, "Vasarnap van!" };	
			status = msgsnd( uzenetsor, &uz, 1024, 0);
			status = msgsnd(uzenetsor, &uz2, 1024, 0);
			if ( status < 0 ) 
			{
				perror("msgsnd");
			}
		   
/*		   strcpy(message,"55");
		   close(firstpipe[0]);
		   close(secondpipe[0]);
		   write(firstpipe[1], message, sizeof(message));
		   strcpy(message,"60");
		   write(secondpipe[1], message, sizeof(message));
		   close(firstpipe[1]);
		   close(secondpipe[1]);
		   close(pipefd[0][0]);
		   write(pipefd[0][1], message, sizeof(message));
		   close(pipefd[0][1]);
			fflush(NULL);
			strcpy(message,"something else message");
		close(pipefd[1][0]);
		     write(pipefd[1][1], message, sizeof(message));
		    close(pipefd[1][1]);*/
			
			status = msgctl(uzenetsor, IPC_RMID, NULL);
			if ( status < 0 ) 
               perror("msgctl");
			printf("Parent Ended\n");
		   //sleep(1);
		
    }
    else
    {
		//sleep(1);
	   printf("Child 2 Started\n");
	   
	   
	   struct uzenet uz;
	   status = msgrcv(uzenetsor, &uz, 1024, 3, 0);
	   printf("%i %s\n", uz.mtype, uz.mtext);
/*	   close(secondpipe[1]);
	   read(secondpipe[0], message_read, sizeof(message_read));
	   close(secondpipe[0]);
	   close(pipefd[1][1]);
	   read(pipefd[1][0], message_read, sizeof(message_read));
	   close(pipefd[1][0]); 
	   printf("Child 2: %s\n", message_read);
	   fflush(NULL);
	    close(pipefd[1]);
		read(pipefd[0], message_read, sizeof(message_read));
		close(pipefd[0]);
		printf("Child 2: %i\n", atoi(message_read) + 1 );*/
		printf("Child 2 Ended\n");
	   //sleep(1);
    }
  }
  else
  {
	//	sleep(1);
		struct uzenet uz;
	   status = msgrcv(uzenetsor, &uz, 1024, 6, 0);

     printf("Child 1 Started\n");
	 	   printf("%i %s\n", uz.mtype, uz.mtext);
	 printf("Child 1 Ended\n");
	 //sleep(1);
   }
  return 0;
}
