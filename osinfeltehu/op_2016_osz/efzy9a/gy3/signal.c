#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){

  printf("Signal with number %i has arrived\n",signumber);
  //signal("SIGTERM, SIG_DFL");//SIGIGN -sigignore
  //raise(signumber);
  ++i;
}

int i = 0;
int main(){
    
  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  // ne a gyerek folyamat elejere, mert ekkor nem biztositjuk hogy kesobb
  // fusson le biztosan a gyerekfolyamat
  
  
  pid_t child=fork();
  if (child>0)
  { 
    pause(); //waits till a signal arrive 
    printf("Signal arrived\n",SIGTERM);
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {
    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
	// Ha előbb ér a végére a szülőfolyamat, ezért kell egy delay,
	// hogy biztosan érjen el odáig: konkurrens processek
    sleep(3);
	// Kikuszobolese: ne a pause() -t hasznaljuk. helyette(1*)
	while(i == 0){
		usleep(100);
	}
	
    kill(getppid(),SIGTERM); 
    //1. parameter the pid number of process, we send the signal
    // 		if -1, then eacho of the processes of the same uid get the signal
    // 		we kill our bash as well! The connection will close
    //2. parameter the name or number of signal
    printf("Child process ended\n");  
  }
  return 0;
}