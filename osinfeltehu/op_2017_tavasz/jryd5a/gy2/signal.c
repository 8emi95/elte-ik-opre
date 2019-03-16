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
    pause(); 
    printf("Signal arrived\n",SIGTERM);

    kill(child, SIGTERM);

    int status;
    wait(&status);
    printf("Parent process ended\n");
  }
  else 
  {
    kill(getppid(),SIGTERM);;
    pause();
    printf("Child process ended\n");
  }

  return 0;
}
