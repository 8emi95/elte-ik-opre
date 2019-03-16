#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <wait.h> 

struct message { 
     long mtype;
     char mtext[1024]; 
};

int msgqueue;

void child_proc(int queue_id);

int main(int argc, char* argv[])
{
  pid_t child1;
  key_t key;

  key = ftok(argv[0], 1);
  msgqueue = msgget(key, 0600 | IPC_CREAT);
  if (msgqueue < 0)
  {
    perror("msgget");
    exit(1);
  }

  printf("E-lection started\n");

  child1 = fork();
  if (child1 < 0)
  {
    perror("The fork calling was not succesful\n");
    exit(1);
  }
  /* CHILD 1 */
  if (child1 == 0)
  {
    printf("I am the first child\n");
    child_proc(1);
    sleep(3);
  }
  else
  {
    pid_t child2 = fork();

    if (child2 < 0)
    {
      perror("The fork calling was not succesful\n");
      exit(1);
    }
    /* CHILD 2 */
    if (child2 == 0)
    {
      printf("I am the second child\n");
      child_proc(2);
      sleep(3);
    }
    /* PARENT */
    else
    {
      struct message msg = { 1, "Hany eves a hajoskapitany?" };

      printf("I am the parent\n");

      if (msgsnd(msgqueue, &msg, sizeof(struct message), 0) < 0)
      {
        perror("msgsnd");
        exit(1);
      }

      msg.mtype = 2;

      if (msgsnd(msgqueue, &msg, sizeof(struct message), 0) < 0)
      {
        perror("msgsnd");
        exit(1);
      }

      waitpid(child1, 0, 0);
      waitpid(child2, 0, 0);

      if (msgctl(msgqueue, IPC_RMID, NULL) < 0)
      {
        perror("msgctl");
        exit(1);
      }
    }
  }
  return 0;
}

void child_proc(int queue_id)
{
  struct message msg;

  if (msgrcv(msgqueue, &msg, sizeof(struct message), queue_id, 0) < 0)
  {
    perror("msgsnd");
    exit(1);
  }

  printf("Received msg with code(%ld) and text(%s)\n", msg.mtype, msg.mtext);
}
