#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <errno.h>
#include <fcntl.h> //O_RDONLY,
#include <unistd.h> //close
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h> //rand
int main(){
 int p[2];
 
 struct timeval tv;
 tv.tv_sec=5; //the select will wait for operation at last for 5 seconds
 tv.tv_usec=0; //
 pipe(p);

 fd_set watchedpipes_read, copy_watchedpipes_read;
 FD_ZERO(&watchedpipes_read); //group of file descriptors
 FD_SET(p[0],&watchedpipes_read); //watch for  input, the reading end of pipe
 copy_watchedpipes_read=watchedpipes_read; //copy of filedesciptor set - for select
 
 //------------------------------------------------------
 //pipe is empty - no read -  timer will tick
 //------------------------------------------------------
 printf("select will wait for read, but timer ticks sooner - after 5 seconds\n");
 int result=select(p[0]+1,&watchedpipes_read,NULL,NULL,&tv);
 //max filedescriptor number + 1 - first parameter, 
 //2.3.4. parameters: set of reading, set of writing, set of exceptions (only sockets)
 //5. parameter - timer
 //result = count of usable filedescriptors
 //result = 0 means, that timer worked
 //result < 0 means an error
 printf("result of select %i\n",result); 
 //select gives back control, when there is some change in any of file descriptors or there is a possibility to read or write
 //at the end of a file, we can read - so it gives back control : (
                           
 if (FD_ISSET(p[0],&watchedpipes_read)) 
 //after select, only that filedesc are available
 //which are ready for operation
 //others will disappear from set
 { 
   printf("Now we can read from the pipe \n");
   int data;
   read(p[0],&data,sizeof(data));   

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
 printf("select will wait for read,  - after 5 seconds it can read a random number\n");
 
 pid_t child=fork();
 if (child>0){ //parent process
   sleep(5);
   int i=rand()%100,status;
   write(p[1],&i,sizeof(i)); //writes to the pipe
   wait(&status);   //waits for the child
 }else{
  watchedpipes_read=copy_watchedpipes_read;
  
  result=select(p[0]+1,&watchedpipes_read,NULL,NULL,NULL);
  if (FD_ISSET(p[0],&watchedpipes_read)) 
  { 
   printf("Now we can read from the pipe \n");
   int data;
   read(p[0],&data,sizeof(data));   

   printf("The data is %i\n",data);                          
  } 
 }
 close(p[0]);
 close(p[1]);
 return 0; 
}
