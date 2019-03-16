#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MSGSZ     128
#define MY_MQ_KEY 1554

typedef struct msgbuf {
    long    mtype;
    char    mtext[MSGSZ];
} message_buf;


int main(int argc, char **argv)
{
    //printf("\nStarting..\n");
    
    pid_t pid = fork();

    if (pid == 0)
    {
        // child process        
		printf("\n[Child] starting..(%d)\n", getpid());
		
		int msqid;
		int msgflg = IPC_CREAT | 0666;
		key_t key;
		message_buf sbuf;
		size_t buf_length;

		/*
		 * Get the message queue id for the
		 * "name" 1234, which was created by
		 * the server.
		 */
		//key = 1234;
		key = MY_MQ_KEY;

		/* //for debugging
		(void) fprintf(stderr, "\nmsgget: Calling msgget(%#lx,\
		%#o)\n",
		key, msgflg);
		*/
		if ((msqid = msgget(key, msgflg )) < 0) {
			perror("msgget");
			exit(1);
		}
		else 
		{
		 //(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);
		}

		/*
		 * We'll send message type 1
		 */
		 
		sbuf.mtype = 1;
		
		//(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);
		
		(void) strcpy(sbuf.mtext, "Did you get this?");
		
		//(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);
		
		buf_length = strlen(sbuf.mtext) + 1 ;
		
		

		/*
		 * Send a message.
		 */
		if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
		   printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
			perror("msgsnd");
			exit(1);
		}

	    else 
	    {
		  printf("Message: \"%s\" Sent\n", sbuf.mtext);
		}	
			//end of child
			//sleep(5);
    }
    else if (pid > 0)
    {
        // parent process  
		printf("\n[Parent starting].. (%d)\n", getpid());		
		
		sleep(5);
		
		int msqid;
		key_t key;
		message_buf  rbuf;

		/*
		 * Get the message queue id for the
		 * "name" 1234, which was created by
		 * the server.
		 */
		//key = 1234;
		key = MY_MQ_KEY;

		if ((msqid = msgget(key, 0666)) < 0) {
			perror("msgget");
			exit(1);
		}

		
		/*
		 * Receive an answer of message type 1.
		 */
		if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
			perror("msgrcv");
			exit(1);
		}

		/*
		 * Print the answer.
		 */
		printf("%s\n", rbuf.mtext);			
		
		//end of parent
		//wait(NULL);
    }
    else
    {
        // fork failed
        printf("fork() failed!\n");
        return 1;
    }

    printf("\nFinished..\n");

    return 0;
}