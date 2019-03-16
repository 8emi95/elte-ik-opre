#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>


//: x=0;
void handler(int signumber){
  //printf("Signal with number %i has arrived\n",signumber);
  //signal(SIGTERM,SIG_DFL);
  //raise(SIGTERM);
  //x++;
}

int main(){

  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
                           //felülírja a default működést, melyik jlezést akarom, ahelyett hogy leállna
                           //futtassa le a handler függvényt
  pid_t child=fork();
  if (child>0)
  { 
    //while(x==0) {usleep(5);}
    //sleep(3);
    pause(); //waits till a signal arrive --jelzésre vár
             //wait: folyamat befejeződésére vár, az adott folyamat gyerek folyamatának befejezésére
    printf("Signal arrived\n",SIGTERM);
    kill(child,SIGTERM);
    int status;
    wait(&status);        //gyerek folyamat befejeződését várja
    printf("Parent process ended\n");
  }
  else 
  {
    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
    sleep(3);
    kill(getppid(),SIGTERM);
    pause(); 
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    printf("Child process ended\n");  
  }
  return 0;
}