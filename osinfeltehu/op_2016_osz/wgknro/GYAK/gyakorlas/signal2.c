#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);

}


int main(){

  const int N = 2;
  pid_t pids_a, pids_b;
  int i;
  int parent_pid;
  int db = 0;
  parent_pid = getpid();
    
//  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  
   pids_a = fork();
  
    signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 



  	printf("PID_A: %i\n",pids_a);

  if (pids_a > 0)
  { 
	printf("SZÜLŐ KEZD: %i\n",getpid());
	if (pids_b <= 0)
	{	
		printf("SZÜLŐ generál B gyerek\n");
		pids_b = fork();
		printf("PID_B: %i\n",pids_b);
	}
	
	if (pids_b > 0)
	{ 

//    for (i = 0; i < N; i++)
//	{
	while(db < 2){
		pause(); //waits till a signal arrive 
		printf("Signal arrived\n",SIGTERM);
		db = db +1;
		printf("Signal db: %i\n",db);
		int status;
	    wait(&status);
//        wait(NULL);
			
//	}
	}
    printf("Parent process ended\n");
	}
  }
  else 
  {

	if (pids_b == 0)
	{
		printf("CHILD a KEZD: %i\n",getpid());

		printf("Waits 2 seconds, then send a SIGTERM %i signal\n",getpid());
		sleep(2);
		kill(getppid(),SIGTERM); 
		//1. parameter the pid number of process, we send the signal
		// 		if -1, then eacho of the processes of the same uid get the signal
		// 		we kill our bash as well! The connection will close
		//2. parameter the name or number of signal
		printf("Child process ended\n");  
	} else {
			
		printf("CHILD a KEZD: %i\n",getpid());

		printf("Waits 2 seconds, then send a SIGTERM %i signal\n",getpid());
		sleep(2);
		kill(getppid(),SIGTERM); 
		//1. parameter the pid number of process, we send the signal
		// 		if -1, then eacho of the processes of the same uid get the signal
		// 		we kill our bash as well! The connection will close
		//2. parameter the name or number of signal
		printf("Child process ended\n");  
	
	}
	
	}
  return 0;
}