#include <stdio.h>
#include <stdlib.h>
#include <poll.h> // poll
#include <errno.h>
#include <fcntl.h> //O_RDONLY,
#include <unistd.h> //close
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h> //rand

int main()
{
	int f=mkfifo("namedpipe",0600);
	if (f<0){perror("error");exit(1);}

	printf("The named pipe poll test was started!\n");
	f=open("namedpipe",O_RDWR);
	struct pollfd poll_fds[5]; // poll file descriptor array
	poll_fds[0].fd=f; 	// file decriptor
	poll_fds[0].events=POLLIN;//|POLLOUT; //watch for  reading, writing

	int result=poll(poll_fds,2,1000); //

	poll_fds[0].events=POLLIN;
	pid_t child=fork();
	if (child>0)
	{
		printf("Parent start\n");
		//parent process
		srand(time(NULL));
		int i=rand()%100;
		printf("Parent writes a number %i to the pipe!\n", i);
		write(f,&i,sizeof(i)); //writes to the pipe
		srand(time(NULL));
		int j=42;
		printf("Parent writes a number %i to the pipe!\n", j);
		write(f,&j,sizeof(j)); //writes to the pipe
		wait(NULL);   //waits for the child
		printf("Parent stop\n");
	}
	else
	{
		// child process
		printf("Child poll is started!\n");
		int result=poll(poll_fds,2,8000); //
		if (result>0) 
		{ 
			printf("The poll revents field is: %i\n",poll_fds[0].revents);
			if (poll_fds[0].revents & POLLIN) // POLLIN event occured
			{
				printf("Now we can read from the pipe \n");
				int data;
				read(f,&data,sizeof(data));   
				printf("The data is: %i\n",data);                          
			}
		}
		else
		{
			printf("Returned poll: %i",result);
		}
		printf("Child start\n");
	}
	unlink("namedpipe");
	return 0; 
}
