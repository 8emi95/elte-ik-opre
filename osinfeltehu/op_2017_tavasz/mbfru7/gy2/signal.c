/*
	ps - process list
	kill -SIGTERM PID // PID=4779 -küld egy jelzést a folyamatnak
			-SIGUSR1 / USR2 -szabadon hsználható jelzések, defaultból megállítja a folyamatot == SIGKILL
	signal(SIGTERM, SIG_DFL) - visszaállítja a SIGTERM-et a defaultra
	raise(SIGTERM) - kiváltja magánaz az eseményt
	kill(PID - melyik folyamat, melyik signal)
	pause - jelzésre vér, wait - gyerekfolyamat befejezésére vár
	
	sleep helyett
	int x = 0;
	handler -> x = 1;
	while (x == 0) {
		usleep (10); --10 ezredmp
		--other stuff
	}
*/
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(){

    
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  
  pid_t child=fork();
  if (child>0)
  { 
    pause(); //megvárja a gyerek jelzését
	kill(child, SIGUSR1);
    //printf("Signal arrived\n",SIGTERM);
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {
    kill(getppid(),SIGUSR2); 
	pause(); //megvárja a szülő jelzését
    //printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
    //sleep(3);
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    printf("Child process ended\n");  
  }
  return 0;
}