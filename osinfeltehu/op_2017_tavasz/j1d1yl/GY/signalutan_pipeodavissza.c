#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber) {
	printf("Signal with number %i has arrived\n",signumber);
}

int main(){	
    signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
    int pipefd[2]; // unnamed pipe file descriptor array
	int pipefd2[2]; // unnamed pipe file descriptor array
    pid_t pid;
    char sz[100];  // char array for reading from pipe

    if (pipe(pipefd) == -1) {		
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefd2) == -1) {
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	
	pid_t child=fork();
	if (child>0) {	  	  
	  pause(); //waits till a signal arrive 
	  printf("Signal arrived\n",SIGTERM);
	  close(pipefd[0]);
	  close(pipefd2[1]);
	  write(pipefd[1], "Hajra Fradi!",13);
	  close(pipefd[1]); // Closing write descriptor 
	  printf("Szulo beirta az adatokat a csobe!\n");
	  read(pipefd2[0],sz,sizeof(sz)); // reading max 100 chars
	  close(pipefd2[0]);
	  printf("Szulo olvasta uzenet: %s",sz);	
	  int status;
	  wait(&status);
	  printf("Parent process ended\n");
	}
	else {				
	    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
	    //sleep(3);
	    close(pipefd2[0]);
	    kill(getppid(),SIGTERM); 
	    close(pipefd[1]);
	    read(pipefd[0],sz,sizeof(sz)); // reading max 100 chars
	    close(pipefd[0]);
	    printf("Gyerek olvasta uzenet: %s",sz,"\n");	
	    write(pipefd2[1], "Hajra Frodo!",13);
	    close(pipefd2[1]); // Closing write descriptor 
	    printf("Gyerek beirta az adatokat a csobe!\n");
        
	    //1. parameter the pid number of process, we send the signal
	    // 		if -1, then eacho of the processes of the same uid get the signal
	    // 		we kill our bash as well! The connection will close
	    //2. parameter the name or number of signal
	    printf("Child process ended\n");  
	}
	return 0;
}