#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_SIZE 512

int main(int argc, char *argv[])
{
    int pipe_file[2];
    char buffer[MAX_SIZE];
    //int status;

	int Xpipe = pipe(pipe_file);
	if (Xpipe < 0)
	{
		perror("pipe(pipe_file) error!");
		exit(-1);
	}

	pid_t child1 = fork();
	if (child1 < 0) 
	{
		perror ("fork() error!");
		exit(-1);
	}

    pid_t child2 = fork();
	if (child2 < 0) 
	{
		perror ("fork() error!");
		exit(-1);
	}

    if (0 == child1)		
	{
        srand(time(NULL));
        int r = (rand()%21) + 30;

		write(pipe_file[1],  &r, MAX_SIZE);
	
	    if(0 == child2)
	    {
            int rand_num;

		    read(pipe_file[0], &rand_num, MAX_SIZE);

            char jaj[MAX_SIZE] = "csak varj, mig a kezunk koze kerulsz";
		    write(pipe_file[1], jaj, strlen(jaj) + 1);
		
		    exit(1);
	    }

        exit(1);
    }
    else
    {
        read(pipe_file[0], buffer, MAX_SIZE);
		printf("%s", buffer);

        close(pipe_file[0]);
        close(pipe_file[1]);
    }
	return 0;
}