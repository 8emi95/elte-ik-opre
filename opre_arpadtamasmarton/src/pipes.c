#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {FALSE, TRUE} bool;
char pipeMessage[100];

char* ReadFromPipe(int pipe)
{
    read(pipe, pipeMessage, sizeof(pipeMessage));
    fflush(NULL);
    return pipeMessage;
}

void WriteToPipe(int pipe, char *text)
{
    write(pipe, text, strlen(text) + 1);
    fflush(NULL);
}

int main(void)
{
    int pipes[4][2];
    pid_t pids[2];
    //bool isChild1 = FALSE;

    if(pipe(pipes[0]) < 0 || pipe(pipes[1]) < 0 || pipe(pipes[2]) < 0 || pipe(pipes[3]) < 0)
    {
        printf("[ERROR] create pipes\n");
        return -1;
    }

    printf("[PARENT] Create child1\n");
    if((pids[0] = fork()) < 0)
    {
        printf("[ERROR] Fork error\n");
        return -1;
    }

    if(pids[0] == 0)
    {
        // Child1 code
        char *message;
        
        close(pipes[0][0]);
        close(pipes[1][1]);
        
        do
        {
			message = ReadFromPipe(pipes[1][0]);
            printf("[CHILD1] pipe message from parent: %s\n", message);
            if(atoi(message) != -1)
            {
				WriteToPipe(pipes[0][1], "TEST MESSAGE");
			}
		}
		while(atoi(message) != -1);
		
        WriteToPipe(pipes[0][1], "-1");
    }
    else if(pids[0] > 0)
    {
        // Parent code
        printf("[PARENT] Create child2\n");
		if((pids[1] = fork()) < 0)
		{
			printf("[ERROR] Fork error\n");
			return -1;
		}

		if(pids[1] == 0)
		{
			// Child2 code
			char *message;
			
			close(pipes[2][0]);
			close(pipes[3][1]);
			
			do
			{
				message = ReadFromPipe(pipes[3][0]);
				printf("[CHILD2] pipe message from parent: %s\n", message);
				if(atoi(message) != -1)
				{
					WriteToPipe(pipes[2][1], "TEST MESSAGE");
				}
			}
			while(atoi(message) != -1);
			
			WriteToPipe(pipes[2][1], "-1");
		}
        else if(pids[1] > 0)
        {
			//Parent Main code
			
			// Close child1 related pipe endings
			close(pipes[0][1]);
			close(pipes[1][0]);
			
			// Close child2 related pipe endings
			close(pipes[2][1]);
			close(pipes[3][0]);
			
			char *message;
			
			WriteToPipe(pipes[1][1], "TEST MESSAGE");
			message = ReadFromPipe(pipes[0][0]);
			printf("[PARENT] pipe message from child1: %s\n", message);
			WriteToPipe(pipes[1][1], "-1");
			message = ReadFromPipe(pipes[0][0]);
			printf("[PARENT] pipe message from child1: %s\n", message);
			
			WriteToPipe(pipes[3][1], "TEST MESSAGE");
			message = ReadFromPipe(pipes[2][0]);
			printf("[PARENT] pipe message from child2: %s\n", message);
			WriteToPipe(pipes[3][1], "-1");
			message = ReadFromPipe(pipes[2][0]);
			printf("[PARENT] pipe message from child2: %s\n", message);
		}
    }
    return 0;
}
