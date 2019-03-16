#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

int fd[2];
int f;
int szavazokszama;

void ellenorzo() {
  printf("Ellemorzo fugveny vagyok es elkeztem dolgozni\n");
  kill(getppid(),SIGUSR1);
  printf("Ellemorzo fugveny vagyok es befejeztem a munka\n");
}

void pecset(){
    printf("Pecset fugveny vagyok es elkeztem dolgozni\n");
    kill(getppid(),SIGUSR1);
    printf("Pecset fugveny vagyok es befejeztem a munka\n");
}

void elnok(){
  printf("Elnok folyamat vagyok és elkeztem a mukodesem es be is fejeztem\n");
}

volatile szamlalo=0;
void handler(int sn){
	szamlalo++;
}

int main(int argc,char** argv)
{
  signal(SIGUSR1,handler);
  if(argc!=2){perror("Nem megfelelo argumentum szam\n"); exit(1);}
  szavazokszama=atoi(argv[1]);

  if(pipe(fd)<0){
    perror("Hibas a cso letrehozasa\n");
    exit(1);}

  pid_t ellen=fork();
  if(ellen>0){
    pid_t pecsetelo=fork();
    if(pecsetelo>0){
      while(szamlalo!=2){
        usleep(100);//pause helyett
      }
      elnok();
      int i;
      for(i=0;i<2;++i){
        wait(NULL);
      }
    }else{
      pecset();
    }
  }else{
ellenorzo();
}
return 0;
}