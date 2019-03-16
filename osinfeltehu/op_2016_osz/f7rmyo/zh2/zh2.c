//> /tmp/mqueue_server_key 

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <signal.h>
#include <time.h>
#include <stdlib.h>


#define SERVER_KEY_PATHNAME "/tmp/mqueue_server_key" 
#define PROJECT_ID 'M' 
#define QUEUE_PERMISSIONS 0660 
#define MAX_CHILDREN 10
#define CHILDS_NOW 2

struct message_text { 
int qid; 
char buf [200]; 
}; 

struct message { 
long message_type; 
struct message_text message_text; 
}; 

int signalArrived = 0;
pid_t pids[MAX_CHILDREN]; 

void sig_handler(int signo)
{
	switch(signo)
	{
		case SIGINT:		
			printf("received SIGINT\n");
			break;
		case SIGUSR1:		
			printf("received SIGUSR\n");
			break;
		case SIGCONT:
			signalArrived = 1;
			break;
	}
}

int childProc(int par)
{
	printf("");
	
	return 0;
}


int childProcKevin(int par)
{
	printf("[KEVIN] This is the kevin proc.\n");
	
	
	srand(time(NULL));
	int r = (rand() % 30)+20; 
	
	printf("[KEVIN] I have %d toys... throwing!\n",r);
	
	sleep(2);
	
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

	//printf ("Please type a message: "); 
	//while (fgets (my_message.message_text.buf, 198, stdin)) 
	//{ 
		char snum[5];
		//itoa(r, snum, 10);
		sprintf(snum, "%d", r);
		strcpy(my_message.message_text.buf, snum);
		//printf("\ndebug --%d-- --%s-- --%s--test\n",r,snum,my_message.message_text.buf);

		// remove newline from string 
		int length = strlen (my_message.message_text.buf); 
		if (my_message.message_text.buf [length - 1] == '\n') 
			my_message.message_text.buf [length - 1] = '\0'; 
			
		// send message to server 
		if (msgsnd (server_qid, &my_message, sizeof (struct message_text), 0) == -1) 
		{ 
			perror ("client: msgsnd"); 
			exit (1); 
		} 
		
		// read response from server 
		if (msgrcv (myqid, &return_message, sizeof (struct message_text), 0, 0) == -1) 
		{ 
			perror ("client: msgrcv"); 
			exit (1); 
		} 
		
		// process return message from server 
		printf ("[KEVIN] Robber says: %s\n\n", return_message.message_text.buf); 
		//printf ("Please type a message: "); 
	//} 
	
	//kevin waits for a signal from the robber
	
	while(signalArrived == 0)
		{
			sleep(1); // idle 
			//printf("\n[COMPANY] debug: sleeploop ");
		} 
	printf("[KEVIN] The robber is coming back!\n");	
	
	//kevin uses the paint bucket
	
	strcpy(my_message.message_text.buf, "festek");
	
	
	if (msgsnd (server_qid, &my_message, sizeof (struct message_text), 0) == -1) 
	{ 
		perror ("client: msgsnd"); 
		exit (1); 
	} 
	
	//kevin hears the reply from paint
	
	if (msgrcv (myqid, &return_message, sizeof (struct message_text), 0, 0) == -1) 
	{ 
		perror ("client: msgrcv"); 
		exit (1); 
	} 		
	
	printf ("[KEVIN] Robber says after the paint: %s\n", return_message.message_text.buf); 

	// remove message queue 
	if (msgctl (myqid, IPC_RMID, NULL) == -1) 
	{ 
		perror ("client: msgctl"); 
		exit (1); 
	} 
	
	//kevin places traps
	int i=1;
	for (i=1; i<=5; ++i)
	{
		r = (rand() % 100); 	
		
		if (r>50)
		{
			printf("[KEVIN] Placing a trap at door %d.\n", i);		
		}
		else
		{
			printf("[KEVIN] No trap at door %d.\n", i);	
		}	
	}	
	
	//printf ("Client: bye\n"); 
	exit (0); 
	
	return 0;
}


int childProcRobber(int par)
{
	printf("[ROBBER] This is the robber proc.\n");
	
	//setting up queue
	
	int toycount=0;
	
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

	
		// read an incoming message 
		if (msgrcv (qid, &message, sizeof (struct message_text), 0, 0) == -1) 
		{ 
			perror ("msgrcv"); 
			exit (1); 
		} 
		
		//msg to int toys
		
		//printf ("[ROBBER] Kevin made me fall! Toys: %d\n",toycount); 
		printf ("[ROBBER] Kevin made me fall! Toys: %s\n",message.message_text.buf); 
		
		// process message 
		int length = strlen (message.message_text.buf); 
		char buf [20]; 
		//sprintf (buf, " %d", length); 
		//strcat (message.message_text.buf, buf); 
		strcpy(message.message_text.buf, "csak varj, amig a kezunk koze kerulsz!");
		int client_qid = message.message_text.qid; 
		message.message_text.qid = qid; 
		
		// send reply message to client 	
		if (msgsnd (client_qid, &message, sizeof (struct message_text), 0) == -1) 
		{ 
			perror ("msgget"); 
			exit (1); 
		} 
		
		printf ("[ROBBER] I have threatened Kevin!\n"); 
		
		//robber signals kevin
		kill(pids[0], SIGCONT);
		
		printf ("[ROBBER] I have alerted Kevin.. accidentally..\n"); 
		
				
		//festék
		
		if (msgrcv (qid, &message, sizeof (struct message_text), 0, 0) == -1) 
		{ 
			perror ("msgrcv"); 
			exit (1); 
		} 
		
		printf ("[ROBBER] Kevin attacked me with: %s\n", message.message_text.buf); 
		
	//effectiveness
	srand(time(NULL));
	int r = (rand() % 100); 
	
	if (r>50)
	{
		strcpy(message.message_text.buf, "Na megallj csak!");
	}	
	else
	{
		strcpy(message.message_text.buf, "Ezt nem uszod meg szarazon!");
	}
	
	if (msgsnd (client_qid, &message, sizeof (struct message_text), 0) == -1) 
	{ 
		perror ("msgget"); 
		exit (1); 
	} 

	
	sleep(2);
	
	return 0;
}



int parProc(int par)
{
	int n=0;
	int status;
	pid_t pid;
		
		while (n < CHILDS_NOW) 
		{
			pid = wait(&status);
			printf("\nChild with PID %ld exited with status 0x%x.\n", (long)pid, status);
			++n;  // TODO(pts): Remove pid from the pids array.
		}
	
	return 0;
}

int main (int argc, char **argv) 
{ 
	//pid_t pids[MAX_CHILDREN]; 
	int childCount = 2;
	int i;
	int pid;
	
	signal(SIGINT, sig_handler);
	signal(SIGUSR1, sig_handler);
	signal(SIGCONT, sig_handler);

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
			//printf("[CHILD] Starting..(%d)\n", getpid());
			
			if (i==0)
			{	
				childProcKevin(i);
			}
			else if (i==1)
			{
				childProcRobber(i);
			}		
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
		
		parProc(0);
		
		printf("[PARENT] Finished.(%d)\n", getpid());
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