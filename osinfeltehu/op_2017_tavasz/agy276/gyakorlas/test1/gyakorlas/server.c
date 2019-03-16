#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MEMSIZE 100

int main(int arg, char *args[]){
  int shmid;
  key_t key;
  char *shm;
  char *s;

  key = 9876;

  shmid = shmget(key, MEMSIZE, IPC_CREAT | 0666);
  if(shmid < 0){
    perror("shmid");
    exit(1);
  }

  shm = shmat(shmid, NULL, 0);
  if(shm == (char *)-1){
    perror("shmat");
    exit(1);
  }

  memcpy(shm, "Hello Client", 12);

  s = shm;
  s += 12;
  *s = 0;

  while(*shm != '*')
    sleep(1);

  printf("Server finished\n");

return 0;
}
