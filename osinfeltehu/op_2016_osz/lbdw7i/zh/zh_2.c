#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <wait.h>
#include <time.h>

#define MEMSIZE 1024
#define SIGERTED SIGRTMIN + 1

struct message { 
    long mtype;
    int number;
    int max;
    char mtext[1024]; 
};
 
int msgqueue;

void handler(int signumber, siginfo_t* info, void* nonused)
{
  printf("Signal with number %i has arrived\n",signumber);
  if (info->si_code == SI_QUEUE) 
    printf("Most mar erted? %s\n", info->si_value.sival_int ? "igen" : "nem");
}

int main(int argc, char* argv[])
{
  pid_t child1;
  key_t key;
  struct sigaction sigact;

  sigact.sa_sigaction = handler;
  sigact.sa_flags=SA_SIGINFO;
  sigemptyset(&sigact.sa_mask);
  sigaction(SIGERTED, &sigact, NULL);

  key = ftok(argv[0], 1);
  msgqueue = msgget(key, 0600 | IPC_CREAT);
  if (msgqueue < 0)
  {
    perror("msgget");
    exit(1);
  }

  child1 = fork();
  if (child1 < 0)
  {
    perror("The fork calling was not succesful\n");
    exit(1);
  }
  /* CHILD 1 */
  if (child1 == 0)
  {
    printf("CHILD| START\n");

    struct message msg;
    int id=0;

    if (msgrcv(msgqueue, &msg, sizeof(struct message), id, 0) < 0)
    {
      perror("msgsnd");
      exit(1);
    }

    printf("My number is %d, and max is %d\n", msg.number, msg.max);
    int after = msg.max-msg.number;
    printf("After me %d\n", after);

    if((rand() % 10) == 0 )
    {
        printf("FAILED\n");
        strcpy(msg.mtext, "FAILED");
    }
    else
    {
        printf("SUCCESS\n");
        strcpy(msg.mtext, "SUCCESS");
    }

    kill(getppid(), SIGERTED);

    if (msgsnd(msgqueue, &msg, sizeof(struct message), 0) < 0)
    {
      perror("msgsnd");
      exit(1);
    }

    printf("CHILD| END\n");

    exit(0);
  }
   /* PARENT */
  else
  {
      printf("PARENT| START\n");

      sigset_t sigset;
      sigfillset(&sigset);
      sigdelset(&sigset,SIGUSR1);

      int max = atoi(argv[1]);
      printf("PARENT| max = %d\n", max);
      srand (time(NULL));
      int childNum = (rand() % max) + 1;

      struct message msg = { 1, childNum, max};

      if (msgsnd(msgqueue, &msg, sizeof(struct message), 0) < 0)
      {
        perror("msgsnd");
        exit(1);
      }

      //fogadjuk a jelzést a gyerektől
      pause();
    
      if (msgrcv(msgqueue, &msg, sizeof(struct message), 0, 0) < 0)
      {
        perror("msgsnd");
        exit(1);
      }

      printf("PARENT| %d. metro %s\n", msg.number, msg.mtext);

      waitpid(child1, 0, 0);

      if (msgctl(msgqueue, IPC_RMID, NULL) < 0)
      {
        perror("msgctl");
        exit(1);
      }

      printf("PARENT| END\n");
  }

  return 0;
}
