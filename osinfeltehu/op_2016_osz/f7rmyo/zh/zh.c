#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <mqueue.h>

/*
1. child indul
2. child megkapja rng
3. +child megkapja az osszes szerelvenyt
4. 10% child fail
5. child print n maxn rng
6. child msg par succ
7. child msg par cause
*/

int main(int argc, char *argv[])
{
    srand(time(NULL));

    mqd_t mqd;

    struct mq_attr atributos = {.mq_maxmsg = 10, .mq_msgsize = 50};

    int i;
    int pid;
    int status;
	
	srand(time(NULL));
	int r = rand(); 
	
	int rng_train = 0;
	int priority;
    int succ_rng = 50;
	//succ_rng = (r2 % 100);			
	
	//for testing
	int num_children = 1;
	
	

	
	int num_trains = atoi(argv[1]);	
	//printf("\nparam: %d\n", num_trains);
	
	
	// num_children = atoi(argv[1]);
	
    char buffer[atributos.mq_msgsize];

    //for (i = 0; i < num_children; i++)
    //{
        if ((pid = fork() == 0))
        {
			//printf("\n debug %d",getpid());
			/*
            int prio = rand () % 3;

            char* msg = "Hi dude";

            strncpy (buffer, msg, sizeof(buffer));
			*/
			
			//1 child receives rng
            //if ((mqd = mq_open("/queue.txt", O_CREAT | O_WRONLY, 0666, &atributos)) == -1)
			if ((mqd = mq_open("/queue.txt", O_CREAT | O_RDWR, 0666, &atributos)) == -1)	
            {
                perror("child mq_open");
                exit(1);
            }
			
			if (mq_receive(mqd, buffer, sizeof(buffer), &priority) == -1)
			{
            perror("mq_recieve");
            exit(1);
			}		
			
			//printf("Received (%s): %s\n", (priority == 0) ? "NORMAL" : "URGENT", buffer);
			
			printf("[_CHILD] Received my train number: %s: %s\n", (priority == 0) ? "" : "", buffer);
			
			//2. child receives max trains
			
			if (mq_receive(mqd, buffer, sizeof(buffer), &priority) == -1)
			{
            perror("mq_recieve");
            exit(1);
			}	
			
			//printf("[CHILD] Received max train number: %s: %s\n", (priority == 0) ? "" : "", buffer);
			printf("[CHILD_] Received max train number: %s \n",  buffer);
			fflush( stdout );
			
			// 3. 10% kudarc
			printf("\ndebugchild 1\n");
			fflush( stdout );
			printf("\ndebugchild 1\n");
			fflush( stdout );
			//r = rand();
			//r = 50;
			//int succ_rng = r % 100;			
			//succ_rng = r2 % 100;			
			//succ_rng =50;			

			
            //char* msgc_1 = "Successful\0";	
			//char* msgc_2 = "Crashed\0";
			
			
			char msgc_1[15] = "Succesful";
			char msgc_2[15] = "Crashed";
			
			printf("\ndebugc 50\n");
			
			int crash = 0;
			
			if (succ_rng>10) //train is ok
			//if ((rand()%100)>10) //train is ok
			{
				strncpy (buffer, msgc_1, sizeof(buffer));			
				crash = 0;
			}
			else //train fails
			{
				strncpy (buffer, msgc_2, sizeof(buffer));				
				crash = 1;
			}	
			
			if (mq_send(mqd, buffer, sizeof(buffer), 2) == -1)
			{
				perror("mq_send");
				exit(1);
			}		
			
			char msgc_3[15] = "Lefagyott";
			strncpy (buffer, msgc_3, sizeof(buffer));
			
			if (crash==1) 
			{
				if (mq_send(mqd, buffer, sizeof(buffer), 2) == -1)
				{
				perror("mq_send");
				exit(1);
				}
			}	
			
			/*
			
            if (mq_send(mqd, buffer, sizeof(buffer), prio) == -1)
            {
                perror("mq_send");
                exit(1);
            }
			*/

            mq_close(mqd);

            exit(0);
        }

    //}

    // parent

    //if ((mqd = mq_open("/queue.txt", O_CREAT | O_RDONLY, 0666, &atributos)) == -1)
	if ((mqd = mq_open("/queue.txt", O_CREAT | O_RDWR , 0666, &atributos)) == -1)
    {
        perror("parent mq_open");
        exit(1);
    }

//    int priority;
	char* msg;

    for (i = 0; i < num_children; ++i)
    {
		
		r = rand(); 
		rng_train = (r % num_trains)+1;
		
		//1 par sends child rng
		//char* msg = "Hi dude";
		
		//msg = itoa(rng_train, msg, 10);
		
		sprintf(msg, "%d", rng_train);
		
		strncpy (buffer, msg, sizeof(buffer));		
		
		if (mq_send(mqd, buffer, sizeof(buffer), 1) == -1)	
        {
            perror("mq_send");
            exit(1);
        }
		
		sleep(1);
		//2 maxnum to child		
		
		char* msg2 = argv[1];
		
		strncpy (buffer, msg2, sizeof(buffer));		
		
		if (mq_send(mqd, buffer, sizeof(buffer), 1) == -1)	
        {
            perror("mq_send");
            exit(1);
        }
		
		//printf("\ndebugpar 1\n");		
		
		// 3 receive train status
		sleep(1);
		
		//printf("\ndebugpar 2\n");
		if (mq_receive(mqd, buffer, sizeof(buffer), &priority) == -1)
        {
            perror("mq_recieve");
            exit(1);
        }
		printf("\ndebugpar 3 %s\n",buffer);		
		printf("Received (%s): %s\n", (priority == 0) ? "NORMAL" : "URGENT", buffer);		
		
		if (strcmp(buffer,"Crashed")==0)
		{
			
			if (mq_receive(mqd, buffer, sizeof(buffer), &priority) == -1)
			{
				perror("mq_recieve");
				exit(1);
			}
			printf("\ndebugpar 3 %s\n",buffer);	
		}
		
		/*	
        if (mq_receive(mqd, buffer, sizeof(buffer), &priority) == -1)
        {
            perror("mq_recieve");
            exit(1);
        }
		*/
		
        //printf("Received (%s): %s\n", (priority == 0) ? "NORMAL" : "URGENT", buffer);

        pid_t childpid;

        if ((childpid = waitpid(-1, &status, 0)) > 0)
        {
            if (WIFEXITED(status))
                printf("PID %d exited normally.  Exit status: %d\n",
                       childpid, WEXITSTATUS(status));
            else
                if (WIFSTOPPED(status))
                    printf("PID %d was stopped by %d\n",
                           childpid, WSTOPSIG(status));
                else
                    if (WIFSIGNALED(status))
                        printf("PID %d exited due to signal %d\n.",
                               childpid,
                               WTERMSIG(status));
        }
    }

    mq_close(mqd);
}