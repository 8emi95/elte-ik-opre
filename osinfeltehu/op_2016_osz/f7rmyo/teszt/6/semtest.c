#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "pv.c"                    /* a fent defini�lt P �s V elj�r�sok */


#define SEMPERM 0600               /* hozz�f�r�si jogok */

void rut_sem(int semid);
int initsem(key_t semkey);

void main(void)
{
  key_t semkey = 0x200;            /* kulcs */
  int semid, i;
  semid = initsem(semkey);         /* szemafor l�trehoz�sa */

  for (i=0; i<3; i++)              /* 3 darab gyerekfolyamat l�trehoz�sa */
     if (fork() == 0)
      rut_sem(semid);              /* er�forr�si k�relmek a gyerekfolyamatokt�l */
}

void rut_sem(int semid)            /* szemaforok megval�s�t�sa */
{
  pid_t pid;
  pid = getpid();                  /* gyerekfolyamat PID-je */

  P(semid);                        /* bel�p�s a kritikus szakaszba */
  printf("a %d folyamat kritikus szakaszban van\n", pid);

  sleep(random()%5);               /* kritikus szakasz; v�rakoz�s */

  printf("a %d folyamat elhagyja a kritikus szakaszt\n", pid);
  V(semid);                        /* kil�p�s a kritikus szakaszb�l */

  exit(0);                         /* gyerekfolyamat v�ge */
}

int initsem(key_t semkey)
{
  int semid;                       /* szemafor l�trehoz�sa */

  semid = semget(semkey, 1, SEMPERM | IPC_CREAT);

  if (semctl(semid, 0, SETVAL, 1) < 0)
    //err_sys("semctl hiba");        /* szemaforok sz�ma = 1 */
	printf("semctl hiba");        /* szemaforok sz�ma = 1 */

  return semid;                    /* szemafor ID-j�nak visszat�r�t�se */
}