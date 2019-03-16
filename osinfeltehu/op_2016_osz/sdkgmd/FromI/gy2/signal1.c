#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(){

    
  signal(SIGUSR1,handler); //handler = SIG_IGN - ignore the signal  
                           //handler = SIG_DFL - back to default behavior 
			// SIGKILL, SIGSTOP nem kezelheto!!!!
  signal(SIGRTMIN,handler); // mas szignalt is kezelhet uaz a handler
  pid_t child=fork();
  if (child>0)
  { 
    pause(); //waits till a signal arrive, that has a handler!!!!! 
    printf("Signal arrived, pause ended!\n");
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {
    //printf("Waits a minute, then send a SIGUSR1(%i),or other signal\n",SIGUSR1);
    //sleep(1);
    //kill(getppid(),SIGUSR1); // SIGRTMIN eseten megy,celba er mindket jelzes
    //sleep(1);  // ha varunk, akkor a masodik signal is celba er
    kill(getppid(),SIGUSR1);  // ha nem varunk, akkor a masodik SIGUSR1
    // blokkolodik, mivel az elso feldolgozasa nem fejezodott be
    // Tehat RT stignalt nem, de a normal szignalt blokkolja a handler
    // 1. parameter the pid number of process, we send the signal
    // if -1, then each of the processes of the same uid get the signal
    // 	so we could kill (SIGTERM) our bash as well! The connection will close
    //2. parameter the name or number of signal
    printf("Child process ended\n");  
  }
  return 0;
}
