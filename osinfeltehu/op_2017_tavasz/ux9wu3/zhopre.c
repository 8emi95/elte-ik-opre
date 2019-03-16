#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/sem.h>

struct uzenet
{
  long mtype;
  pid_t telefon;
};

void handler(int s)
{
  printf("Fogadta az ugyfel a hivast.\n");
}

void handler2(int s)
{
  printf("Fonok ertesitve lett.\n");
}

void szemafor_muvelet(int semid, int op)
{
  struct sembuf muv;
  muv.sem_num=0;
  muv.sem_op = op;
  muv.sem_flg=0;
  semop(semid, &muv, 1);
}

int main(int argc, char ** argv)
{
  if (argc != 3)
  {
    return 0;
  }
  
  //veletlen
  srand(time(NULL));;

  //signal blokkolásának beállítása
  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler2);
  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGUSR1);
  sigaddset(&sigset, SIGUSR2);
  sigprocmask(SIG_BLOCK, &sigset, NULL);

  //cso letrehozasa
  int toAgent[2];
  int toBuyer[2];
  pipe(toAgent);
  pipe(toBuyer);

  pid_t child1 = fork();
  if (child1 > 0)
  {
    //uzenetsoros dolgok
    int uzenetsor;
    key_t mkulcs = ftok("mux9wu3",1);
    uzenetsor = msgget(mkulcs, 0600|IPC_CREAT);
    pid_t child2 = fork();

    //osztottmemorias dolgok
    int oszt_mem;
    char* s;
    key_t skulcs = ftok("sux9wu3",1);
    oszt_mem = shmget(skulcs, 500, IPC_CREAT|S_IRUSR|S_IWUSR);
    s = shmat(oszt_mem, NULL, 0);
    
    //szemforos dolgok
    int my_sem;
    key_t semkulcs = ftok("semux9wu3", 1);
    my_sem = semget(semkulcs, 1, IPC_CREAT | S_IRUSR | S_IWUSR);
    semctl(my_sem, 0, SETVAL, 0);
    szemafor_muvelet(my_sem, 1);

    if (child2>0) //vallalat
    {
      //nem hasznalt pipe-ok bezárása
      close(toAgent[0]);
      close(toAgent[1]);
      close(toBuyer[0]);
      close(toBuyer[1]);
      
      //A feladat
      struct uzenet uz = {1, child1};
      msgsnd(uzenetsor, &uz, sizeof(uz.telefon), 0);
      
      //D feladat
      sigemptyset(&sigset);
      sigsuspend(&sigset);
      int ar;
      szemafor_muvelet(my_sem, -1);
      memcpy(&ar, s, sizeof(ar));
      szemafor_muvelet(my_sem, 1);
      printf("Vegso ar (fonokhoz eljuttatva): %d\n",ar);


      shmdt(s);
      wait(NULL);
      wait(NULL);
      shmctl(oszt_mem, IPC_RMID, NULL);
      msgctl(uzenetsor, IPC_RMID, NULL);
      semctl(my_sem, 0, IPC_RMID);
    } else //ugynok
    {
      //pipe végek bezárása
      close(toBuyer[0]);
      close(toAgent[1]);

      //A feladat
      struct uzenet uz;
      msgrcv(uzenetsor, &uz, 1024, 1, 0);
      kill(uz.telefon, SIGUSR1);
      
      //B feladat és C feladat
      int valasz = 0;
      int kezdetiar = atoi(argv[2]);
      int ar = atoi(argv[2]);
      while (valasz == 0)
      {
        printf("Termek ara: %d\n", ar);
        write(toBuyer[1], &ar, sizeof(ar));
        read(toAgent[0], &valasz, sizeof(valasz));
        if (valasz == 0)
        {
          printf("Ugyfel nem fogadta el.\n");
          if (ar > kezdetiar/2)
          {
            ar = ar - 1;
          }
        } else
        {
          printf("Ugyfel el fogadta.\n");
        }
      }

      //D feladat
      szemafor_muvelet(my_sem, -1); //down
      memcpy(s, &ar, sizeof(int));
      szemafor_muvelet(my_sem, 1); //up
      kill(getppid(), SIGUSR2);

      //már használt pipe-ok bezárása
      close(toBuyer[1]);
      close(toAgent[0]);
      
      shmdt(s);
    }
  } else //vasarlo
  {
    //pipe végek bezárása
    close(toBuyer[1]);
    close(toAgent[0]);

    //A feladat
    sigemptyset(&sigset);
    sigsuspend(&sigset);

    //B feladat
    int valasz = 0;
    int ar = 1;
    while (valasz == 0)
    {
      read(toBuyer[0], &ar, sizeof(ar));
      printf("Termek ara ugyfelnel: %d\n", ar);
      valasz = rand()%2;
      write(toAgent[1], &valasz, sizeof(valasz));
    }
    //már használt pipe-ok bezárása
    close(toBuyer[0]);
    close(toAgent[1]);
  }
 
  return 0;
}
