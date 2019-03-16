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
  printf("\nEllenor vagyok  \n"); 
}
void pecset(){
  kill(getppid(),SIGUSR1); //munka ksz vagyok 
  printf("\nPecsetelo vagyok  \n"); 
}

void elnok(){
 printf("Elnok vagyok \n");
 //mindenki kesz volt munkara, csobe a genera adatokat 

}
volatile szamlalo=0;
void handler(int sn){
  szamlalo++;
}
int main(int argc,char** argv)
{
  signal(SIGUSR1,handler);
  if (argc!=2) {perror("egy argumentum...\n") ; exit(1);}
  szavazokszama=atoi(argv[1]);

  if (pipe(fd)<0){perror("Hibas a cso letrehozas\n");exit(EXIT_FAILURE);}
   
  pid_t ellen=fork();
  if (ellen>0)
  {
      pid_t pecsetelo=fork();
      if (pecsetelo>0){
       while(szamlalo!=2){;}  //pause helyett
       elnok();
       wait(NULL); wait(NULL);
      }
      else {
       pecset();
      }      
  }
  else {
    ellenorzo();
  }   
  return 0;
}
