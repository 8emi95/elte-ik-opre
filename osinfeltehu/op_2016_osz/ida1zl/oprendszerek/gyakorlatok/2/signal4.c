#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

int i = 0;
//ha érkezik egy SIGTERM jelzés, ne álljon le, futtassa ezt a függvényt
void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  i++;
}
//végtelen ciklus lesz
int main()
{ 
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  
  pid_t child=fork();
  if (child>0)
  { 
	kill(child, SIGTERM);
    //pause(); //waits till a signal arrive, vár amíg jelzést nem kap
    printf("Signal arrived\n",SIGTERM);
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {
    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
    //sleep(3);
    //kill(getppid(),SIGTERM); //getppid: szülő pidszámának meghatározása
	//pause();
	while(i == 0) {usleep(100);}
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    printf("Child process ended\n");  
  }
  return 0;
}