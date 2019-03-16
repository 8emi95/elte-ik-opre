#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>


void handler(int signumber){
  //printf("Signal with number %i has arrived\n",signumber);
}

int main(int argc, char *argv[]){
  // szignal
  signal(SIGTERM,handler); 
  int status;
  // pipe
  int pipefd[2];
  pipe(pipefd);
  char sz[100];


  pid_t child=fork();
  // szulo folyamat
  if (child>0)
  { 
    printf("Taxistársaság: várok egy utasra\n");
    // varunk a gyerek szignalra
    pause();
    // megjott a szignal
    read(pipefd[0],sz,sizeof(sz));
    sleep(1);
    printf("Taxitársaság: Megkaptam a lakcímet: %s\n", sz);
    kill(getppid(), SIGTERM);
  }
  else 
  {
    // kuldjuk a signalt a szulonek h eltudjon kezdeni mukodni
    sleep(1);
    printf("Utas: küldöm a lakcímet\n",SIGTERM);    
    kill(getppid(),SIGTERM); 
    sleep(1);
    write(pipefd[1], argv[1],sizeof(argv[1]));
    sleep(1);
    kill(child, SIGTERM);
  }
  return 0;
}

