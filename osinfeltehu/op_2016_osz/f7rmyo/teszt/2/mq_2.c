#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main()
{
    srand(time(NULL));

    mqd_t mqd;

    struct mq_attr atributos = {.mq_maxmsg = 10, .mq_msgsize = 50};

    int i;
    int pid;
    int status;
    int num_children = 2;

    char buffer[atributos.mq_msgsize];

    for (i = 0; i < num_children; i++)
    {
        if ((pid = fork() == 0))
        {
			printf("[CHILD] Starting..(%d)\n", getpid());
            int prio = rand () % 3;
			
			//msg 1

            char* msg = "Hi dude";

            strncpy (buffer, msg, sizeof(buffer));

            //if ((mqd = mq_open("/queue.txt", O_CREAT | O_WRONLY, 0666, &atributos)) == -1)
			if ((mqd = mq_open("/queue.txt", O_CREAT | O_RDWR, 0666, &atributos)) == -1)
            {
                perror("child mq_open");
                exit(1);
            }

            if (mq_send(mqd, buffer, sizeof(buffer), prio) == -1)
            {
                perror("mq_send");
                exit(1);
            }

			//msg 2
			
			char* msg2 = "Test 2";

            strncpy (buffer, msg2, sizeof(buffer));
			
			if (mq_send(mqd, buffer, sizeof(buffer), prio) == -1)
            {
                perror("mq_send");
                exit(1);
            }
			
			//receive from parent
			
			sleep(2);
			
			if (mq_receive(mqd, buffer, sizeof(buffer), &prio) == -1)
			{
				perror("mq_recieve");
				exit(1);
			}        
			printf("[CHILD] Received: %s\n", buffer);	
			
			//end of child
			
            mq_close(mqd);

            exit(0);
        }

    }

    // parent

    if ((mqd = mq_open("/queue.txt", O_CREAT | O_RDWR, 0666, &atributos)) == -1)
    {
        perror("parent mq_open");
        exit(1);
    }

    int priority;

    for (i = 0; i < num_children; ++i)
    {
		//receive 1
	
        if (mq_receive(mqd, buffer, sizeof(buffer), &priority) == -1)
        {
            perror("mq_recieve");
            exit(1);
        }

        //printf("Received (%s): %s\n", (priority == 0) ? "NORMAL" : "URGENT", buffer);
		printf("[PARENT] Received: %s\n", buffer);
		
		//receive 2
		
		if (mq_receive(mqd, buffer, sizeof(buffer), &priority) == -1)
        {
            perror("mq_recieve");
            exit(1);
        }        
		printf("[PARENT] Received: %s\n", buffer);	

		
		//reply to every child
		int prio=1;
		char* msgResp = "I hear you";
		strncpy (buffer, msgResp, sizeof(buffer));
		for (i = 0; i < num_children; ++i)
		{			
			if (mq_send(mqd, buffer, sizeof(buffer), prio) == -1)
			{
				perror("mq_send");
				exit(1);
			}	
		}
        pid_t childpid;
		/*
        if ((childpid = waitpid(-1, &status, 0)) > 0)
        {
            if (WIFEXITED(status))
				{
                //printf("PID %d exited normally.  Exit status: %d\n", childpid, WEXITSTATUS(status));
				}	   
            else
				{	
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
		*/
    }

    mq_close(mqd);
	
	sleep(5);
	msgctl(mqd, IPC_RMID, NULL);
}