#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main()
{
     int    i;
     //char   buf[BUF_SIZE];
        pid_t  pid;
     pid = fork();
   
   if (pid==0){
          printf( "This line is from pid %d, this is the cild (=0)\n", pid, i);
   }else{
	   printf( "This line is from pid %d,  this is the parent  (!=0)\n", pid, i);
	   
	   
   } 
     
}