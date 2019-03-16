#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

//feladat: a szulo kuldjon egy jelzest a gyerekfolyamatnak, a gyerek pedig kuldjon vissza egy jelzest a szulonek es a szulo jelenitsen emg egy uzenetet hogy megkapta azt a jelzest

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(){

  signal(SIGTERM,handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP), 
                           //handler = SIG_DFL - back to default behavior 
  
  
  pid_t child=fork();
  if (child>0)
  {
	
	pause(); 
	sleep(1);
	kill(child,SIGTERM);
	int(&status);
    wait(&status);
    
    printf("Parent process ended\n");
  }
   else 
  {
	  
    printf("Waits 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
    sleep(3);
	kill(getppid(),SIGTERM); 
    printf("Child process ended\n");  
  }
  return 0;
}