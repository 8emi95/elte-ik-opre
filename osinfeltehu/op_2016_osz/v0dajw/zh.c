#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <wait.h>
#include <time.h>

#define MAX 100

struct message { 
     long mtype;
     char mtext[1024]; 
};

void handler(int x) { x = x; }

int main(int argc, char* argv[])
{
	
    pid_t child, child2;
    key_t key;
    int msgqueue;
    

    key = ftok(argv[0], 1);
    msgqueue = msgget(IPC_PRIVATE, 0600 | IPC_CREAT);
	
	
    if (msgqueue < 0)
    {
        perror("msgget - 39. sor");
        exit(1);
    }


    child = fork();
    if (child < 0)
    {
        perror("The fork calling was not succesful\n");
        exit(1);
    }

    /* CHILD  - KEVIN*/
    if (child == 0)
    {
		//printf("En a KEVIN vagyok \n");
        struct message msg1, msg2;
        int hany_uzenet;

        srand(time(NULL));
		
		hany_uzenet = (rand() % 31) + 20;
		printf("Kevin vagyok es  %d uzenetet fogok kuldeni! \n", hany_uzenet);	
		//printf("En a KEVIN vagyok megint\n");
		int i;// = 0;
		for (i = 0; i < hany_uzenet; i++)
		{
			//msg1 = { hany_uzenet, "Jatek!" }; 
			msg1.mtype = hany_uzenet;
			strcpy(msg1.mtext, "Jatek");
			/*if (msgsnd(msgqueue, &msg1, sizeof(struct message), 0) < 0)
			{
				perror("msgsnd");
				exit(1);
			}*/
			msgsnd(msgqueue, &msg1, sizeof(struct message), 0);
			//printf("Az i: %d: + %d +  %s \n", i, msg1.mtype, msg1.mtext);			
		}
		sleep(1);
		msgrcv(msgqueue, &msg2, sizeof(struct message), 0, 0);
		
		printf("Kevin vagyok, es a betoro ezt uzente:  %s \n", msg2.mtext);
		
		
		 exit(1);
    }
    
    else
    {
		child2 = fork();
		if (child2 < 0)
		{
			perror("The fork calling was not succesful\n");
			exit(1);
		}

		/* CHILD2 - BETÖRŐ */
		if (child2 == 0)
		{
			//printf("En a betoro vagyok  \n");
			struct message msg1, msg2;
			int hany_uzenet;
						
			msgrcv(msgqueue, &msg1, sizeof(struct message), 0, 0);
			
			//printf("Az uzenet: %s \n", msg1.mtext);
			hany_uzenet = msg1.mtype;
			//printf("En a betoro vagyok es %d alkalommal hanyattesek \n", hany_uzenet);
			printf("En a betoro vagyok es hanyattesek 1. alkalommal \n");
			int i;
			for (i = 1; i <hany_uzenet; i++)
			{
				msgrcv(msgqueue, &msg1, sizeof(struct message), 0, 0);
				printf("En a betoro vagyok es hanyattesek %d. alkalommal \n", i+1);
				//printf("%d En a betoro vagyok es uzenetet fogadok: %d +  %s +  \n", i, msg1.mtype, msg1.mtext);
			}
			
			msg2.mtype = 1;
			strcpy(msg2.mtext, "Csak varj, amig a kezunk koze nem kerulsz! \n");			
			msgsnd(msgqueue, &msg2, sizeof(struct message), 0);
			
			
			
			
			 exit(1);
		}
		
		
		/* PARENT  */
		else
		{
			//printf("En a szulo vagyok \n");
			

			waitpid(child, 0, 0); // !!!!!!!!!!!!!!!!!!
			waitpid(child2, 0, 0);
			printf("Gyerekek mind befejezodtek \n");
		}
		
		
		
        
    }

    return 0;
}

