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
 //most irasra es  olvasasra is nyitva van!
 f=open("namedpipe",O_RDWR);
 struct pollfd poll_fds[5]; // poll file descriptor array
 poll_fds[0].fd=f; 	// file decriptor
 poll_fds[0].events=POLLIN;//|POLLOUT; //watch for  reading, writing
 //------------------------------------------------------
 //pipe is empty - no read -  timer will tick, POLLOUT -no timer tick
 //------------------------------------------------------
 //ha nem erkezik adat, 1mig fogunk varni:
 int result=poll(poll_fds,1,1000); //
 //1. parameter - poll_fds, the wathing file descriptors
 // int poll_fds.fd file descriptor, short poll_fds.events the watched events
 // short poll_fds-revents returnd occured events 
 //2. parameter - nfds_t type, number of wathing file descriptors 
 //3. parameter - time in milliseconds, if this parameter is negative, 
 // it means an infinite timeout
 //result > 0 POLLIN event occured, azaz erkezett vmilyen adat, (vmelyik adatcsoben?)
 //result = 0 means, that timer worked
 //result < 0 means an error
 printf("result of poll %i\n",result); 
 //poll gives back control, when there is some change in any of file 
 //descriptors according poll_fds.events
 //at the end of a file, we can read - so it gives back control : (
                           
 if (result==0) 
 {		// timer ellapsed 
   printf("The timer was ticked\n"); 
 }
 else 
 {
	if (result>0)
	{
	if (poll_fds[0].revents & POLLOUT) // ide nem jut a vezerles
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
 //Now we write into the pipe - so it can read
 printf("Second part!\n"); 
 //erkezik e adat a 0as csobe?:
 poll_fds[0].events=POLLIN;
 pid_t child=fork();
 if (child>0)
  { //parent process
   printf("Parent waites for a while...\n");
   sleep(3);
   printf("Parent writes a number to the pipes!\n");
   int i=rand()%100,status;
   write(f,&i,sizeof(i)); //writes to the pipe
   //erre a veletlenszamra var a gyerek
   wait(&status);   //waits for the child
  }
 else
  { // child process
   printf("Child poll is started!\n");
  int result=poll(poll_fds,1,8000); //
  if (result>0) 
  { 
   printf("The poll revents field is: %i\n",poll_fds[0].revents);
   //a 0as csovezeteknel rakerdezunk arra h milyen esemeny volt
   //ha ott polling volt , akkor oda erekzett adat
   //es utana engedem ra csaka read fuggvenyt
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
	printf("Returned poll: %i",result);
  }
 }
 unlink("namedpipe");
 return 0; 
}
