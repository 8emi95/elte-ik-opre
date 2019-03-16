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

void handler(int signumber){
	printf("Allomason van!\n");	
}


int main(int argc, char** argv)
{  
pid_t child1 = fork();

	signal(SIGUSR1, handler);
	
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
	
srand(time(NULL));
  int r;
  
  r = (rand() % atoi(argv[1]))+1;
  
 
  if(child1 < 0)
  {
    printf("Error.\n");
  }
  else if (child1 > 0) //parent
  {
		   printf("Parent Started\n");
		   sleep(2);
		   struct uzenet uz;
			uz.mtype = r;
			strcpy(uz.mtext,argv[1]);
		   status = msgsnd(uzenetsor, &uz, 1024, 0);
		   if ( status < 0 ) 
			{
				perror("msgsnd");
			}

			pause();
			struct uzenet rec_uz;
			status = msgrcv(uzenetsor, &rec_uz, 1024, 999, 0);
			printf("PARENT message: %s\n",  rec_uz.mtext);
			
			
			status = msgctl(uzenetsor, IPC_RMID, NULL);
			if ( status < 0 ) 
               perror("msgctl");
		    
			printf("Parent Ended\n");
		
    }
    else //kid
    {
	   int m;
	   
	   printf("Child 1 Started\n");
	   
	   int r_reason;
	   char reason[50];
	   r_reason = rand() % 101;
	   if (r_reason < 11){
		   strcpy(reason,"lefagyott");
	   } else if (r_reason > 11 && r_reason < 21){
		   strcpy(reason,"elcsuszott");
	   } else {
		   strcpy(reason,"beert");
		}
	   struct uzenet uz;
	   status = msgrcv(uzenetsor, &uz, 1024, r, 0);
	   
	   m = atoi(uz.mtext) - uz.mtype;
	   printf("Szamom: %i. utanam: %i %s\n", uz.mtype, m, reason);
	  
	 
		kill(getppid(), SIGUSR1);
		struct uzenet send_uz;
		send_uz.mtype = 999;
		strcpy(send_uz.mtext,reason);
		status = msgsnd(uzenetsor, &send_uz, 1024, 0);
	   
		printf("Child 1 Ended\n");
    }
  
  return 0;
}
