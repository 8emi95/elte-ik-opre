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

struct message { 
     long mtype;
     char mtext[1024]; 
};
 
int msgqueue;
int pipefd[2][2];
int sh_mem_id, sem_id;
char *s;

int create_semaphore(const char* pathname,int semaphore_value){
    int semid;
    key_t key;
    
    key = ftok(pathname,1);    
    if ((semid = semget(key, 1, IPC_CREAT | S_IRUSR | S_IWUSR )) < 0)
	    perror("semget");
    if (semctl(semid, 0, SETVAL, semaphore_value) < 0)
      perror("semctl");
       
    return semid;
}

void semaphore_operation(int semid, int op){
    struct sembuf sem_op;
    
    sem_op.sem_num = 0;
    sem_op.sem_op  = op; /* op=1 up, op=-1 down */ 
    sem_op.sem_flg = 0;
    
    if (semop(semid,&sem_op,1) < 0) /* 1 number of sem. operations */
      perror("semop");	    
}

void delete_semaphore(int semid){
      semctl(semid,0,IPC_RMID);
}

void child_proc(int);

int main(int argc, char* argv[])
{
  pid_t child1;
  key_t key;
  srand (time(NULL));

  key = ftok(argv[0], 1);
  msgqueue = msgget(key, 0600 | IPC_CREAT);
  if (msgqueue < 0)
  {
    perror("msgget");
    exit(1);
  }

  if (pipe(pipefd[0]) == -1 || pipe(pipefd[1]) == -1)
  {
    perror("Error at opening pipe");
    exit(EXIT_FAILURE);
  }

  sh_mem_id = shmget(key, MEMSIZE, IPC_CREAT | S_IRUSR | S_IWUSR);
  s = shmat(sh_mem_id, NULL, 0);

  sem_id = create_semaphore(argv[0], 0);
  strcpy(s, "");
  semaphore_operation(sem_id, 1);

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
      int pipe_msg;
      struct message msg = { 1, "Hany eves a hajoskapitany?" };

      printf("I am the parent\n");
      close(pipefd[0][1]);
      close(pipefd[1][1]);

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

      read(pipefd[0][0], &pipe_msg, sizeof(pipe_msg));
      printf("Answer from child 1: %d\n", pipe_msg);
      read(pipefd[1][0], &pipe_msg, sizeof(pipe_msg));
      printf("Answer from child 2: %d\n", pipe_msg);

      semaphore_operation(sem_id, -1);
      if (!strcmp(s, "1") || !strcmp(s, "2"))
        printf("%s-es gyerek nem erti\n", s);
      semaphore_operation(sem_id, 1);

      waitpid(child1, 0, 0);
      waitpid(child2, 0, 0);

      if (msgctl(msgqueue, IPC_RMID, NULL) < 0)
      {
        perror("msgctl");
        exit(1);
      }

      delete_semaphore(sem_id);
	    shmctl(sh_mem_id, IPC_RMID, NULL);
    }
  }
  return 0;
}

void child_proc(int id)
{
  struct message msg;
  int pipe_msg;

  srand (time(NULL) ^ (getpid()<<16));

  close(pipefd[id-1][0]);

  if (msgrcv(msgqueue, &msg, sizeof(struct message), id, 0) < 0)
  {
    perror("msgsnd");
    exit(1);
  }

  printf("Received msg with code(%ld) and text(%s)\n", msg.mtype, msg.mtext);

  pipe_msg = rand() % 4 + 1;
  write(pipefd[id-1][1], &pipe_msg, sizeof(pipe_msg));

  if (rand() % 2)
  {
    semaphore_operation(sem_id, -1);
    sprintf(s, "%d", id);
    semaphore_operation(sem_id, 1);
    shmdt(s);
  }
}
