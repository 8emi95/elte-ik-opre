#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <sys/types.h> //waitpid
#include <errno.h> //perror 
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

int fd[2];
int f;
int szavazokszama;


void ellenorzo(){
  kill(getppid(),SIGUSR1); //munka ksz vagyok 
  printf("Ellenor vagyok - kesz a munkara\n");
  int azonosito;
  close(fd[1]);
  
  while (read(fd[0],&azonosito,sizeof(int))){
    printf("Ellenor: %i\n",azonosito);
    fflush(stdout);   
  }
  close(fd[0]);
  printf("\nEllenor vagyok csobol olvastam, irtam \n"); 
}
void elnok(){
 printf("Elnok vagyok - mindenki kesz\n");
 //mindenki kesz volt munkara, csobe a genera adatokat 
 int i,azonosito;
 printf("Elnok vagyok...azonositokat gyartottam\n");
 close(fd[0]);
 for (i=0;i<szavazokszama;i++){
   azonosito=rand()%10000;
   write(fd[1],&azonosito,sizeof(int));
   printf("%i ,\n",azonosito);
  }
 printf("Elnok vagyok, beirtam az azonositokat a csobe\n");
 close(fd[1]);
}
volatile szamlalo=0;
void handler(int sn){
  szamlalo++;
;
}
int main(int argc,char** argv)
{
  signal(SIGUSR1,handler);
  szavazokszama=atoi(argv[1]);
  if (pipe(fd)<0){perror("Hibas a cso letrehozas\n");exit(EXIT_FAILURE);}
   
  pid_t ellen=fork();
  if (ellen>0)
  {
      while(szamlalo!=1){;}  //pause helyett
      elnok();
      wait(NULL);
  }
  else {
    ellenorzo();
  }

   
  return 0;
}
