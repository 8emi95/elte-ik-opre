#include <stdio.h>
#include <stdlib.h>
#include <poll.h> // poll
#include <errno.h>
#include <fcntl.h> //O_RDONLY,
#include <unistd.h> //close
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h> //rand

int main(){

 int f=mkfifo("namedpipe",0600);
 if (f<0){perror("error");exit(1);}

 printf("The named pipe poll test was started!\n");
 f=open("namedpipe",O_RDWR);
 struct pollfd poll_fds[5]; // poll file descriptor array
 poll_fds[0].fd=f; 	// file decriptor
 poll_fds[0].events=POLLIN;//|POLLOUT; //watch for  reading, writing

 int result=poll(poll_fds,1,1000); //

 printf("result of poll %i\n",result);

 if (result==0)
 {
   printf("The timer was ticked\n");
 }
 else
 {
	if (result>0)
	{
	if (poll_fds[0].revents & POLLOUT) // it never will be executed - the timer was ticked
		{
		printf("Data may be written to the pipe!\n");
		write(f,"Fradi!",6);
   		printf("Now we can read from the pipe \n");
   		char v[10];
   		read(f,v,8);
		printf("The data is %s\n",v);
		}
	}
	else
	printf("Error number: %i\n",errno);
 }
 //-------------------------------------------------
 //Now we write into the pipe - so it can be read
 printf("Second part!\n");
 poll_fds[0].events=POLLIN;
 pid_t child=fork();
 if (child>0)
  { //parent process
   printf("Parent waits for a while...\n");
   sleep(3);
   printf("Parent writes a number to the pipes!\n");
   int i=rand()%100,status;
   write(f,&i,sizeof(i)); //writes to the pipe
   wait(&status);   //waits for the child
  }
 else
  { // child process
   printf("Child poll is started!\n");
  int result=poll(poll_fds,1,8000); //
  if (result>0)
  {
   printf("The poll revents field is: %i\n",poll_fds[0].revents);
   if (poll_fds[0].revents & POLLIN) // POLLIN event occured
   {
   printf("Now we can read from the pipe \n");
   int data; char cdata;
   read(f,&cdata,sizeof(cdata));
   printf("The data is: %c\n",cdata);
   }
  }
  else
  {
	printf("Returned poll: %i\n",result);
  }
 }
 unlink("namedpipe");
 return 0;
}
