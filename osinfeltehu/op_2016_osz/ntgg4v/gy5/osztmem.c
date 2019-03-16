#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

struct valami{
	char* s;
	int b;
};

int main(int argc, char * argv[]) {

  int pid;
  key_t kulcs;
  int oszt_mem_id;
  char * s;
  int * p;
  struct valami * v;
  // a parancs nevevel es az 1 verzio szammal kulcs generalas
  kulcs = ftok(argv[0], 1);
  // osztott memoria letrehozasa, irasra olvasasra, 500 bajt mrettel
  oszt_mem_id = shmget(kulcs, 500, IPC_CREAT | S_IRUSR | S_IWUSR);
  // csatlakozunk az osztott memoriahoz, 
  // a 2. parameter akkor kell, ha sajat cimhez akarjuk illeszteni
  // a 3. paraméter lehet SHM_RDONLY, ekkor csak read van   
  s = shmat(oszt_mem_id, NULL, 0);
  v =(struct valami*) s;
  //
  pid = fork();
  if (pid > 0) {

    (*v).s = argv[2];
    // beirunk a memoriaba 
    //strcpy(s, buffer);
    // elengedjuk az osztott memoriat
	(*v).b=atoi(argv[1]);
    shmdt(s);
    //	s[0]='B';  //ez segmentation fault hibat ad
    wait(NULL);
    // IPC_RMID- torolni akarjuk a memoriat, ekkor nem kell 3. parameter
    // IPC_STAT- osztott memoria adatlekerdezes a 3. parameterbe,
    //  ami shmid_ds struct tipusu mutato
    shmctl(oszt_mem_id, IPC_RMID, NULL);
  } else
  if (pid == 0) {
    sleep(1);
    printf("A gyerek ezt olvasta az osztott memoriabol: %d\n", (*v).b);
	printf("A gyerek ezt olvasta az osztott memoriabol: %s\n", (*v).s);
    // gyerek is elengedi
    shmdt(s);
  }

  return 0;
}
