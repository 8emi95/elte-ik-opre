#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <errno.h>
#include <fcntl.h> //O_RDONLY,
#include <unistd.h> //close
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h> //rand
#include <signal.h> 

void handler(int s){
 printf("Handler says: SIGTERM arrived\n");
}
int main(){
 
 int p[2],status;
 pipe(p);

 sigset_t sigset; //blocks all signals before calling  pselect 
 sigfillset(&sigset);
 sigprocmask(SIG_BLOCK,&sigset,NULL);
 

 pid_t child=fork();
 if (child>0){
   struct timespec ts;
   ts.tv_sec=5; //the pselect will wait for operation at last for 5 seconds
   ts.tv_nsec=0; //value of timespec is not changing 
  
   fd_set watchedpipes_read;
   FD_ZERO(&watchedpipes_read); //group of file descriptors
   FD_SET(p[0],&watchedpipes_read); //watch for  input, the reading end of pipe
 
   
   struct sigaction sa; //SIGTERM and  handler is connected 
   sa.sa_handler=handler;
   sigfillset(&sa.sa_mask);
   sigdelset(&sa.sa_mask,SIGKILL); 
             //only SIGKILL won't be blocked during the execution of handler
   sa.sa_flags=0;
   sigaction(SIGTERM,&sa,NULL);

   
   sigset_t blocked;
   sigfillset(&blocked); //each SIGNAl except SIGTERM and SIGKILL are blocked  
   sigdelset(&blocked,SIGTERM);
   sigdelset(&blocked,SIGKILL);
   //------------------------------------------------------
   //pipe is empty - no read, no timeout -   a signal arrives
   //------------------------------------------------------
   printf("pselect will wait a SIGTERM signal or timer tick\n");
   int result=pselect(p[0]+1,&watchedpipes_read,NULL,NULL,&ts,&blocked);
   //max filedescriptor number + 1 - first parameter, 
   //2.3.4. parameters: set of reading, set of writing, set of exceptions (only sockets)
   //5. parameter - timer
   //6. parameter - signalset
   
   //result = count of usable filedescriptors
   //result = 0 means, that timer worked
   //result < 0 means an error - now it can mean, that a signal arrives
      
   printf("result of pselect %i\n",result); 
   //select gives back control, when there is some change in any of file descriptors or there is a possibility to read or write
   //at the end of a file, we can read - so it gives back control : (
   
   if (result<0)                           
   {
     if (errno==EINTR) //a signal was caught 
     {
       printf("The non-blocked signal, SIGTERM  was arrived\n");
       printf("The time remained back is not shown (it is the original value) %i\n",ts.tv_sec);
     }
     else 
     {
      printf("Some non-known error...\n");
     } 
   }
   if (result==0)
   {
    printf("Timer ticked \n");
   }
   wait(&status);
  } 
  else
  {
   printf("Child process \n");
   //sleep(10); //timer tick
   kill(getppid(),SIGTERM);

  }
  close(p[0]);
  close(p[1]);
  return 0; 
}
