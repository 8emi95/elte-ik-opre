#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}
int main(int argc, char *argv[]){
	int	secnumber[1];
	 
	int firstNumber=rand()%100+1;
	printf("first player: %i\n",firstNumber);
	
	signal(SIGTERM,handler);

		int pipe_fd[2]; 
          
            
	 if (pipe(pipe_fd) == -1) 
	   {
               perror("Error in pipe opening!");
               exit(EXIT_FAILURE);
        }
		   
	pid_t child=fork();
	
	
	if (child>0)
  { 
    pause(); //waits till a signal arrive 
    printf("Signal arrived\n",SIGTERM);
	
    close(pipe_fd[1]);
	sleep(1);
	read(pipe_fd[0],secnumber,sizeof(secnumber));
	close(pipe_fd[0]);
	
	int status;
    wait(&status);
    
	printf("second number: %i ,%i\n",secnumber[0],firstNumber);
	
	if(secnumber[0]==firstNumber){
		printf("it is ok.\n");
	}else if(secnumber[0]>firstNumber){
		printf("it is bigger.\n");
	}else if(secnumber[0]<firstNumber){
		printf("it is smaller.\n");
	}
	
	
  }
  else 
  {
    
    kill(getppid(),SIGTERM); 
   
	
	int a =rand()%100+1;
	int r1[1];
	 r1[0]=a;
	
	close(pipe_fd[0]);
	write(pipe_fd[1],r1,sizeof(r1));
	close(pipe_fd[1]);
    printf("Child number %i\n",r1[0]);  
  }
	return 0;
}