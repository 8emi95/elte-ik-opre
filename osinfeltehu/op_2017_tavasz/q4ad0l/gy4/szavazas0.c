#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 

int fd[2];
int szamlalo=0;
void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
  szamlalo++;
}

void elnok(int szavazokszama)
{
  srand(time(NULL));
  int* azon=(int*)malloc(szavazokszama*sizeof(int));
  int i;
  for(i=0;i<szavazokszama;i++)
  {
    azon[i] = rand();
  }
  write(fd[1],azon,szavazokszama*sizeof(int));
  close(fd[1]);
  close(fd[0]);
}

void pecset()
{
  kill(getppid(),SIGUSR1);
  printf("Pecsetelo kesz\n");
}

void ellenorzo(int szavazokszama)
{
  kill(getppid(),SIGUSR1);
  printf("Ellenorzo kesz\n");
  int * azon;
  read(fd[0],azon,szavazokszama*sizeof(int));
  int i;
  for(i=0;i<szavazokszama;i++)
  {
     printf("%i\n", azon[i]);
  }

}

int main(int argc,char ** argv)
{
  signal(SIGUSR1,handler);
  if (argc!=2) {perror("egy argumentum...\n"); exit(1);}
  int szavazokszama=atoi(argv[1]);

  if(pipe(fd)<0){perror("Hibas a cso letrehozas\n"); exit(1);}
  
  pid_t ellen=fork();
  if(ellen>0)
  {
    pid_t pecsetelo=fork();
    if(pecsetelo>0){
      while(szamlalo!=2){usleep(100);} //pause helyett
      elnok(szavazokszama);
      wait(NULL);wait(NULL);
    }
    else{
      pecset();
    }
  }
  else{
    ellenorzo(szavazokszama);
  }
  return 0;
}
