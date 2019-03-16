#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <sys/wait.h> 
#include <errno.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>


void handler(int signumber){
	pid_t my_id = getpid();
	printf("I arrived to the city (%i,%i).\n",my_id,signumber);
}


int main(){
	 int pipefd[2]; // unnamed pipe file descriptor array
     pid_t pid;
     char sz[100];
	 
	  if (pipe(pipefd) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
	
	
	      pid = fork();	// creating parent-child processes
           if (pid == -1) 
	   {
               perror("Fork hiba");
               exit(EXIT_FAILURE);
           }
		   
		   if(pid==0)
		   {
			  close(pipefd[1]);
			  read(pipefd[0],sz,sizeof(sz));
			  printf("Gyerek olvasta uzenet, itt kerul lebonyolitasra: %s",sz);
	          printf("\n");
			  close(pipefd[0]);
			   
			   
		   }
	       else
		   {
			   close(pipefd[0]); 
			   write(pipefd[1], "Hajra Fradi!",13);
			   close(pipefd[1]);
			   fflush(NULL);
               //wait();				  	   
		   }

return 0;	
}