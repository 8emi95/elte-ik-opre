#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}

int main(){

    
                           //handler = SIG_DFL - back to default behavior 
			// SIGKILL, SIGSTOP nem kezelheto!!!!
  signal(SIGUSR1,handler); // mas szignalt is kezelhet uaz a handler
  pid_t child=fork();
  if (child>0)
  {
//sleep(1); 
   printf("Szulo rogton jelzest kuld a gyereknek!\n");
   kill(child,SIGUSR1);
    //pause(); //waits till a signal arrive, that has a handler!!!!! 
    printf("Signal elkuldve!!\n");
    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {
  //signal(SIGUSR1,handler); //handler = SIG_IGN - ignore the signal  
    printf("Gyerek: Varunk szignalra:(%i)\n",SIGUSR1);
    //sleep(1);
    //kill(getppid(),SIGUSR1); // SIGRTMIN eseten megy,celba er mindket jelzes
    //sleep(1);  // ha varunk, akkor a masodik signal is celba er
//	pause();
    //kill(getppid(),SIGUSR1);  // ha nem varunk, akkor a masodik SIGUSR1
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
