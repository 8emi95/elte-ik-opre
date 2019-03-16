#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>

#define CHILDREN 2

struct message
{
  long mtype;
  char mtext[80];
};

static void handler(int signo)
{
  printf("%d jelzés küldése megtörtént! \n", signo);
  signal(signo, handler);
}

void sendMsg( char* adr, int readF, int writeF, int parent)
{
  printf("%s Kuldom a címet! %s\n", adr);
  //char buffer[20];

  write(writeF, adr, strlen(adr) + 1);
  kill(parent, SIGUSR1);
}

int main(int argc, char** argv)
{
  //srand(time(NULL));
  signal(SIGCONT, handler);
  signal(SIGUSR1, handler);
  int firstChild, secondChild, parent;
  int pipeMsg[2];
  char* adr = argv[1];
  printf("Cim: %s\n", adr);
  int i = 0;
  
  if (pipe(pipeMsg) == -1)
  {
    perror("Hiba a pipe létrehozásakor.");
    exit(EXIT_FAILURE);
  }
  
  parent = getpid();
  firstChild = fork();
  if (firstChild == 0)
  {
    sleep(1);
    printf("Első utas! \n");
    kill(parent, SIGUSR1);

    printf("%s Kuldom a címet! %s\n", adr);


    write(pipeMsg[1], adr, strlen(adr) + 1);
    kill(parent, SIGUSR1);

  }
  else
  {
    secondChild = fork();
    if (secondChild == 0)
    {
      sleep(2);
      printf("Elindult a taxi! \n");
      kill(parent, SIGUSR1);
      

    }
    else
    {
      pid_t pid;
      printf("Kozpont Elindult es var\n");
      pause();
      pause();
      printf("Kozpont fogadja a cimet\n");
      close(pipeMsg[1]);
      char buff[50];
      read(pipeMsg[0],buff, sizeof(buff));
      printf("kiolvasott cím: %s\n", buff);

      int i = 0;
      int status;
      while( i < CHILDREN){
        pid = wait(&status);
        printf("Egy gyerek vegzett! %ld\n", (long)pid);
        i++;
      }
    }
  }
  return 0;
}
