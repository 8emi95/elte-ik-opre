#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MSGSZ     128
#define MY_MQ_KEY 1775

typedef struct msgbuf {
    long    mtype;
    char    mtext[MSGSZ];
} message_buf;


int main(int argc, char **argv)
{
    //printf("\nStarting..\n");
    
	int i;
	int childCount = 1;
	
    pid_t pid = fork();
	
	for(i = 0; i < childCount; i++) {
	
		if (pid == 0)
		{
			// child process i      
			printf("\n[Child] starting..(%d)\n", getpid());
			
			int msqid;
			int msgflg = IPC_CREAT | 0666;
			key_t key;
			message_buf sbuf;
			message_buf  rbuf;
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
			
			
			//FIRST MSG
			char testStr[100] = "first test";
			
			//(void) strcpy(sbuf.mtext, "Did you get this?");
			(void) strcpy(sbuf.mtext, testStr);
			
			//(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);
			
			buf_length = strlen(sbuf.mtext) + 1 ;			

			/*
			 * Send a message.
			 */
			 
			//MQ STEP 1
			if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
			   printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
				perror("msgsnd");
				exit(1);
			}
			
			//MQ STEP 2
			strcpy(testStr,"second test");			
			
			(void) strcpy(sbuf.mtext, testStr);
			
			//(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);
			
			buf_length = strlen(sbuf.mtext) + 1 ;			
			
			if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
			   printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
				perror("msgsnd");
				exit(1);
			}

			else //only prints the last msg for debugging
			{ 
			  printf("[Child] Message: \"%s\" Sent\n", sbuf.mtext);
			}
			
			sleep(2);

			/*
			printf("\nChild debug 1");
			sleep(6);
			printf("\nChild debug 2");
			*/
			//receive response from parent	
			
			//MQ STEP 3
			
			if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
				perror("msgrcv");
				exit(1);
			}
			printf("\nChild debug 3");
			
			printf("\n [Child] From parent: %s\n", rbuf.mtext);	
			
				//end of child
				//sleep(5);
		}
		else if (pid > 0)
		{
			// parent process  
			printf("\n[Parent starting].. (%d)\n", getpid());		
			
			sleep(1);
			
			int msqid;
			key_t key;
			message_buf  rbuf;		
			message_buf sbuf;			
			size_t buf_length;

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
			 
			//MQ STEP 1 
			if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
				perror("msgrcv");
				exit(1);
			}

			printf("[Parent] From a child: %s\n", rbuf.mtext);			
			
			
			//MQ STEP 2
			if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
				perror("msgrcv");
				exit(1);
			}
			
			printf("[Parent] From a child: %s\n", rbuf.mtext);	
			
			//we got two messages, we send one back
			
			
			
			//MQ STEP 3
			char testStr[100] = "I hear you loud and clear";
			
			printf("\n[Parent] Replying to child: %s\n",testStr);
			
			(void) strcpy(sbuf.mtext, testStr);
			
			buf_length = strlen(sbuf.mtext) + 1 ;			

			sleep(1);	

			if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
			   printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
				perror("msgsnd");
				exit(1);
			}
			
			//end of parent
			//wait(NULL);
		}
		else
		{
			// fork failed
			printf("fork() failed!\n");
			return 1;
		}
	}

    printf("\nFinished..\n");

    return 0;
}