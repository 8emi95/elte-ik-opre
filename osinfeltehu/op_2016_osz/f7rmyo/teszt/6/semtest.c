#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "pv.c"                    /* a fent definiált P és V eljárások */


#define SEMPERM 0600               /* hozzáférési jogok */

void rut_sem(int semid);
int initsem(key_t semkey);

void main(void)
{
  key_t semkey = 0x200;            /* kulcs */
  int semid, i;
  semid = initsem(semkey);         /* szemafor létrehozása */

  for (i=0; i<3; i++)              /* 3 darab gyerekfolyamat létrehozása */
     if (fork() == 0)
      rut_sem(semid);              /* erõforrási kérelmek a gyerekfolyamatoktól */
}

void rut_sem(int semid)            /* szemaforok megvalósítása */
{
  pid_t pid;
  pid = getpid();                  /* gyerekfolyamat PID-je */

  P(semid);                        /* belépés a kritikus szakaszba */
  printf("a %d folyamat kritikus szakaszban van\n", pid);

  sleep(random()%5);               /* kritikus szakasz; várakozás */

  printf("a %d folyamat elhagyja a kritikus szakaszt\n", pid);
  V(semid);                        /* kilépés a kritikus szakaszból */

  exit(0);                         /* gyerekfolyamat vége */
}

int initsem(key_t semkey)
{
  int semid;                       /* szemafor létrehozása */

  semid = semget(semkey, 1, SEMPERM | IPC_CREAT);

  if (semctl(semid, 0, SETVAL, 1) < 0)
    //err_sys("semctl hiba");        /* szemaforok száma = 1 */
	printf("semctl hiba");        /* szemaforok száma = 1 */

  return semid;                    /* szemafor ID-jának visszatérítése */
}