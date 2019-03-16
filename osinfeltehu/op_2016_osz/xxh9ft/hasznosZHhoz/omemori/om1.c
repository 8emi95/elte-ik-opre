/**
 * (c) FARKAS M  2007. tavasz
 * FAMIAAT.ELTE fmate14@gmail.com
 * Operi Rendszerek gyakorlat
 *
 *
 * 16. feladat
 *
 * Mint az elozo feladat, csak osztott memoria terulettel 
*/
#include <errno.h>   /* hibak kezelesere */
#include <stdio.h>   /* mindenfele kiiras, beolvasas */
#include <stdlib.h>  /* standard konyvtar: stdout, NULL, stb. */
#include <unistd.h>  /* fork, getpid, ilyenek */
#include <string.h>  /* strcpy, strtok, stb. */
#include <wait.h>    /* waitpid() es tarsai */
#include <sys/shm.h> /* Osztott teruletek */
#include <sys/ipc.h> /* Osztott teruletek */
#include <sys/stat.h> /* uzenetsorok */
#define USE(x) if ((x)==(x)){}
        
int main()
{
  int shmid;
  pid_t gyerek;
  if ( (shmid = shmget(IPC_PRIVATE, 1024, IPC_CREAT|S_IWUSR|S_IRUSR)) < 0 ) 
  {
    perror("shmget()");
    exit(1);
  };

  if((gyerek = fork()) < 0) 
  {
    perror("fork()");
    exit(1);
  };

  if( gyerek == 0 ) // child
  {
    char * shmcim = shmat(shmid, NULL, 0);
    if ((int)shmcim == -1) 
    {
      perror("shmat()");
      exit(1);
      printf("A kuldott uzenet: \"Hello!\"\n");
      strcpy(shmcim,"Hello!");
      shmdt(shmcim);
    }
  }
  else 
  {
    char * shmcim = shmat(shmid, NULL, 0);
    if ((int)shmcim == -1) 
    {
      perror("shmat()");
      exit(1);
    };
    usleep(1);
    /* Olvasas a socket-bol */
    printf("A kapott uzenet: \%s \n", shmcim);
    shmdt(shmcim);
    if (waitpid(gyerek,NULL,0) < 0) 
    {
      perror("waitpid()");
      exit(1);
    }
    if (shmctl(shmid,IPC_RMID,NULL) < 0) 
    {
      perror("shmctl()");
      exit(1);
    };
  };
  return 0;
}
                                                                                                                                                                               