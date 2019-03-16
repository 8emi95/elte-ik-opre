#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>


volatile sig_atomic_t usr_interrupt = 0;

void handler(int signumber){
	usr_interrupt = 1;
	printf("Signal Received!\n");
}

int main(int argc, char* argv[])
{
    int fd;
    printf("Fifo start!\n");
    int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); // creating named pipe file
    // S_IWGRP, S_IROTH (other jog), file permission mode
    // the file name: fifo.ftc
    // the real fifo.ftc permission is: mode & ~umask 
    if (fid==-1)
    {
		printf("Error number: %i",errno);
		exit(EXIT_FAILURE);
    }
    printf("Mkfifo vege, fork indul!\n");
    char number[10];
	int num;

  signal(SIGUSR1,handler);

	
	pid_t pid; 
	pid = fork();

	
	if(pid > 0)	// parent
	{

		printf("Irja be a szamot\n");
		scanf("%d",&num);
		sprintf(number,"%d",num);
		printf("Writing in the pipe. Number: %s\n",number);
		
		fd=open("fifo.ftc",O_WRONLY);
		write(fd,&number,sizeof(number));

		close(fd);
		unlink("fifo.ftc");
		
	}
	else if(pid == 0)	//child
	{
		char read_number[10];
		fd=open("fifo.ftc",O_RDONLY);
		read(fd,&read_number,sizeof(&read_number));
		int _num = atoi(read_number);
		printf("Child read number: %d\n");
		close(fd);
		int i = 0;
		for(; i < _num;i++)
		{
			printf("%d\n",rand()%100);
		}
		kill(getppid(),SIGUSR1);
		exit(0);
		
	}
	
}