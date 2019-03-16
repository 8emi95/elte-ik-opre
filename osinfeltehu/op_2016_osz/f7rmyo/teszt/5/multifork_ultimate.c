//> /tmp/mqueue_server_key 

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 

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

int main (int argc, char **argv) 
{ 
	pid_t pids[MAX_CHILDREN]; 
	int childCount = 2;
	int i;

	for (i = 0; i < childCount; ++i) {
		if ((pids[i] = fork()) < 0) 
		{
			perror("fork");
			abort();
		} 
		else if (pids[i] == 0) 
		{
			//child i
			printf("[CHILD] Starting..(%d)\n", getpid());
			
			sleep(1);
			
		} 
		else if (pids[i] > 0) 
		{
			//parent
			printf("[PARENT] Starting..(%d)\n", getpid());
			int n=0;
			int status;
			pid_t pid;
			
			while (n < childCount) 
			{
				pid = wait(&status);
				printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
				++n;  // TODO(pts): Remove pid from the pids array.
			}
		}
	}

}