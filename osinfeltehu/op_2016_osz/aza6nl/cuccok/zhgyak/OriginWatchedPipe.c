#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
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

	struct timeval tv;
	//the select will wait for operation at last for 10 seconds
	tv.tv_sec=3;
	tv.tv_usec=0;

	printf("The named pipe select test was started!\n");
	f=open("namedpipe",O_RDWR);
	fd_set watchedpipes_read, copy_watchedpipes_read;
	//group of file descriptors
	FD_ZERO(&watchedpipes_read);
	//watch for  input
	FD_SET(f,&watchedpipes_read);
	//copy of filedesciptor set - for select
	copy_watchedpipes_read=watchedpipes_read;

	pid_t child=fork();
	if (child>0)
	{
		printf("Parent start!\n");
		//parent process
		//sleep(5);
		srand(time(NULL));
		int i=rand()%100;
		//writes to the pipe
		printf("Parent write!\n");
		write(f,&i,sizeof(i));
		char str[] = "Ez az uzenet!";
		write(f,&str,sizeof(char)*50);
		//waits for the child
		wait(NULL);   
		printf("Parent stop!\n");
	}
	else
	{
		watchedpipes_read=copy_watchedpipes_read;
		printf("Child: We are waiting for a pipe read event!\n");
		int result=select(f+1,&watchedpipes_read,NULL,NULL,NULL);
		if (FD_ISSET(f,&watchedpipes_read)) 
		{
			printf("Child: Now we can read from the pipe \n");
			int data;
			read(f,&data,sizeof(data));   
			printf("The data is: %i\n", data);                          
		}
		if (FD_ISSET(f,&watchedpipes_read)) 
		{
			printf("Child: Now we can read from the pipe \n");
			char str[50];
			read(f, &str, sizeof(char)*50); 			
			printf("The data is: %s\n", str);                          
		} 
		printf("Child is over!\n");
	}
	unlink("namedpipe");
	return 0; 
}
