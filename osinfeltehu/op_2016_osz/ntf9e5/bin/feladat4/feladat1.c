#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h> 

int childPid = 0;

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
 /* sleep(3);
  printf("Child ready to read again\n");
  sleep(3);*/
 /* printf("My name is legend\n");
  sleep(3);
//  kill(childPid, 10);*/
}

/*void handler_second(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  sleep(3);
  printf("My name is legend 2\n");
  sleep(3);

}*/

int main(){

  // signal lekezelése  
  signal(SIGUSR1,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
	//signal(SIGUSR2, handler_second);
	
  int pipefd[2];
  int senderfd[2];
  char sz[100];
  char send[100];
 
  DIR           *d;
  struct dirent *dir;
 
 if (pipe(pipefd) == -1) 
 {
   perror("Hiba a pipe nyitaskor!");
   exit(EXIT_FAILURE);
 }
 if (pipe(senderfd) == -1) 
 {
   perror("Hiba a pipe nyitaskor!");
   exit(EXIT_FAILURE);
 }
  pid_t child=fork();
  // szülő

  if (child>0)
  {
	printf("Parent started now\n");
	printf("Listing directories:\n");
	
	char event[50];
	
while(1){	
	 d = opendir("1");
	  if (d)
	  {
		while ((dir = readdir(d)) != NULL)
		{
		  printf("%s\n", dir->d_name);
		}

		closedir(d);
	  }
	  printf("Pick a number: ");
	  scanf("%s", event);
	  break;
}
	close(pipefd[0]);
	close(senderfd[0]);
	// sending place to go to
	write(pipefd[1], event, sizeof(event));
	close(pipefd[1]);
    
	childPid = child;
    pause(); //waits till a signal arrive 
    printf("Signal arrived\n",SIGUSR1); 
	printf("Writing to pipe\n");
	
	sleep(1);
	// after opening file, sending names
	write(senderfd[1], "writing now again from this", 30);
	close(senderfd[1]);

	//printf("Write to pipe done \n");
	
	fflush(NULL);
	// második signal várás
//	pause();
	wait();
/*	int status;
    wait(&status);
	  printf("SIGNAL: %i \n", status);
	/*
	pause();
    printf("Signal arrived\n",SIGUSR2);
    //int status;
    wait(&status);
	  printf("SIGNAL: %i \n", status);
	*/
    printf("Parent process ended\n");
  }
  else 
  // gyerek
  {
    printf("Child is waiting to go to a place\n");
	sleep(3);
	
	close(pipefd[1]);
	close(senderfd[1]);
	read(pipefd[0], sz, sizeof(sz));
	printf("Child is going to: %s\n", sz);
	close(pipefd[0]);
//	printf("Child arrived, sending signal \n");
	
	kill(getppid(), SIGUSR1);
	/*
	close(pipefd[0]);
	open(pipefd[0]);
	*/
	//sleep(5);
	
	read(senderfd[0], send, sizeof(send));
	
	printf("Child is reading: \n");
	printf("%s\n", send);
	close(senderfd[0]);
	
//	kill(getppid(), SIGUSR2);
	/*
	printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGUSR1);
    sleep(3);
    kill(getppid(),SIGUSR1); 
	*/
	
	//printf("First signal has been made\n");
  //  sleep(3);
//	kill(getppid(),SIGUSR2); 
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
 //   sleep(3);
    printf("Child process ended\n");  
  }

  return 0;
}
