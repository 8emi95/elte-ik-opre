//> /tmp/mqueue_server_key 

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <signal.h>

#define SERVER_KEY_PATHNAME "/tmp/mqueue_server_key" 
#define PROJECT_ID 'M' 
#define QUEUE_PERMISSIONS 0660 
#define MAX_CHILDREN 10

struct message_text { 
int qid; 
char buf [200]; 
}; 

struct message { 
long message_type; 
struct message_text message_text; 
}; 

int signalArrived = 0;

void sighandler(int signal)
{    
    if(signal == SIGCONT)
    {
        signalArrived = 1;
    }
}

int childProc()
{
	key_t server_queue_key; 
	int server_qid, myqid; 

	struct message my_message, return_message; 

	// create my client queue for receiving messages from server 
	if ((myqid = msgget (IPC_PRIVATE, 0660)) == -1) 
	{ 
		perror ("msgget: myqid"); 
		exit (1); 
	} 

	if ((server_queue_key = ftok (SERVER_KEY_PATHNAME, PROJECT_ID)) == -1) 
	{ 
		perror ("ftok"); 
		exit (1); 
	} 

	if ((server_qid = msgget (server_queue_key, 0)) == -1) 
	{ 
		perror ("msgget: server_qid"); 
		exit (1); 
	} 

	my_message.message_type = 1; 
	my_message.message_text.qid = myqid; 
	int sleeping = 0;

	printf ("Please type a message (or sleep to make the server sleep and wake to wake the server): "); 
	while (fgets (my_message.message_text.buf, 198, stdin)) 
	{ 
		// remove newline from string 
		int length = strlen (my_message.message_text.buf); 
		if (my_message.message_text.buf [length - 1] == '\n') 
			my_message.message_text.buf [length - 1] = '\0'; 
		
		if ( strcmp(my_message.message_text.buf,"wake")!=0 ) 
		{
			if (sleeping == 0)
			{			
				// send message to server 
				if (msgsnd (server_qid, &my_message, sizeof (struct message_text), 0) == -1) 
				{ 
					perror ("client: msgsnd"); 
					exit (1); 
				} 			
			
				if ( strcmp(my_message.message_text.buf,"sleep")!=0 ) 
				{			
					// read response from server 
					if (msgrcv (myqid, &return_message, sizeof (struct message_text), 0, 0) == -1) 
					{ 
						perror ("client: msgrcv"); 
						exit (1); 
					} 
					
					// process return message from server 
					printf ("Message received from server: %s\n\n", return_message.message_text.buf); 
					printf ("Please type a message: "); 
				}
				else
				{
					sleeping = 1;
				}
			}
			else
			{
				printf ("\nServer is sleeping, no echo. Please type a message: "); 
			}			
		}
		else 
		{
			//send the signal
			kill(getppid(), SIGCONT);
			sleeping = 0;
		}
	} 
	
	// remove message queue 
	if (msgctl (myqid, IPC_RMID, NULL) == -1) 
	{ 
		perror ("client: msgctl"); 
		exit (1); 
	} 
	
	printf ("Client: bye\n"); 
	exit (0); 
}

int parentProc()
{
	signal(SIGCONT,sighandler);

	key_t msg_queue_key; 
	int qid; 
	struct message message; 
	if ((msg_queue_key = ftok (SERVER_KEY_PATHNAME, PROJECT_ID)) == -1) 
	{ 
		perror ("ftok"); 
		exit (1); 
		} 

	if ((qid = msgget (msg_queue_key, IPC_CREAT | QUEUE_PERMISSIONS)) == -1) 
	{ 
		perror ("msgget"); 
		exit (1); 
	} 

	printf ("Server: Hello, World!\n"); 

	while (1) 
	{ 
		// read an incoming message 
		if (msgrcv (qid, &message, sizeof (struct message_text), 0, 0) == -1) 
		{ 
			perror ("[PARENT], msgrcv"); 
			exit (1); 
		} 
		
		printf ("Server: message received: %s\n", message.message_text.buf); 
		
		// process message 
		int length = strlen (message.message_text.buf); 
		char buf [20]; sprintf (buf, " %d", length); 
		strcat (message.message_text.buf, buf); 
		int client_qid = message.message_text.qid; 
		message.message_text.qid = qid; 
		
		if ( strcmp(message.message_text.buf,"sleep") == 0 ) //client wants the server to sleep		
		{
			printf("SERVER: Going to sleep until a signal wakes me");
			while(signalArrived == 0)
			{
				sleep(1); // idle 				
			}			
		}
		else
		{
			// send reply message to client 	
			if (msgsnd (client_qid, &message, sizeof (struct message_text), 0) == -1) 
			{ 
				perror ("msgget"); 
				exit (1); 
			} 
			
			printf ("Server: response sent to client.\n"); 
		}
	}

}

int main (int argc, char **argv) 
{ 
	pid_t pids[MAX_CHILDREN]; 
	int childCount = 1;
	int i;
	int pid;

	for (i = 0; i < childCount; ++i) 
	{
		pid = fork();
		if (pid < 0) 
		{
			perror("fork");
			abort();
		} 
		else if (pid == 0) 
		{
			//child i
			printf("[CHILD] Starting..(%d)\n", getpid());
			
			childProc();
			break;			
		} 	
		else
		{
			pids[i] = pid;
		}		
	}
	
	if (pid > 0) 
	{
		//parent
		printf("[PARENT] Starting..(%d)\n", getpid());
		
		parentProc();
		/*
		int n=0;
		int status;
		pid_t pid;
		
		while (n < childCount) 
		{
			pid = wait(&status);
			printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
			++n;  // TODO(pts): Remove pid from the pids array.
		}
		*/
	}

}