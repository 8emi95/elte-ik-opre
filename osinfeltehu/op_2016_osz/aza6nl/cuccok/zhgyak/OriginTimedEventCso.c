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

	//------------------------------------------------------
	//pipe is empty - no read -  timer will tick
	//------------------------------------------------------
	int result=select(f+1,&watchedpipes_read,NULL,NULL,&tv);
	int j=rand()%100;
	write(f,&j,sizeof(j));
	//max filedescriptor number + 1 - first parameter, 
	//2.3.4. parameters: set of reading, set of writing, set of exceptions (only for sockets)
	//5. parameter - timer
	//result = count of usable filedescriptors
	//result = 0 means, that timer worked
	//result < 0 means an error
	printf("result of select %i\n",result); 
	//select gives back control, when there is some change in any of file descriptors or there is a possibility to read or write
	//at the end of a file, we can read - so it gives back control : (
	//after select, only that filedesc are available
	//which are ready for operation
	//others will disappear from set
	if (FD_ISSET(f+1,&watchedpipes_read)) 
	{ 
		printf("Now we can read from the pipe \n");
		int data;
		read(f,&data,sizeof(data));   
		printf("The data is %i\n",data);                          
	} 
	else 
	{
		printf("The timer was ticked\n"); 
	}
	//-------------------------------------------------
	//Now we write into the pipe - so it can read
	//-------------------------------------------------
	//we should use the copy of file set
	//while the timer shows the time back - it's null now - we have to overwrite!

	tv.tv_sec=10;

	pid_t child=fork();
	if (child>0)
	{
		//parent process
		sleep(5);
		int i=rand()%100;
		//writes to the pipe
		write(f,&i,sizeof(i));
		//waits for the child
		wait(NULL);   
		printf("Parent is over!\n");
	}
	else
	{
		watchedpipes_read=copy_watchedpipes_read;
		printf("Child: We are waiting for a pipe read event!\n");
		result=select(f+1,&watchedpipes_read,NULL,NULL,NULL);
		if (FD_ISSET(f,&watchedpipes_read)) 
		{
			printf("Child: Now we can read from the pipe \n");
			int data; char cdata;
			read(f,&data,sizeof(data));   
			printf("The data is: %i\n",data);
			printf("Child is over!\n");                          
		} 
	}
	unlink("namedpipe");
	return 0; 
}
