#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SEGSIZE 100                                 /* a tolandszeg max. mete */

void writeshm(int shmid, char *segptr, char *text);
void readshm(int shmid, char *segptr);
void removeshm(int shmid);                          /* fgvyek deklara */
void changemode(int shmid, char *mode);
void usage(void);

int main(int argc, char *argv[])                    /* parancssorb a paramerek */
{
  key_t key;                                        /* kulcs */
  int shmid;                                        /* osztott memia ID-ja */
  char *segptr;                                     /* osztott memia ce */
  if (argc == 1) usage();                           /* hiyos paramerlista */
  key = ftok(".", 'S');                             /* egyedi kulcs lrehoza */
                                                    /* megnyit, szs eset lrehoz */
  printf("A kulcs %d\n", key);
  if ((shmid = shmget(key, SEGSIZE, IPC_CREAT|IPC_EXCL|0666)) == -1)
  {
     printf("az osztott memoriazona letezik - megnyitom\n");
     if ((shmid = shmget(key, SEGSIZE, 0)) == -1)
     {
       perror("shmget hiba");
       exit(1);
     }
  }
  else  printf("letrehozok egy uj osztott memoriazonat\n");
                                                     /* memiac hozzendele */
  if ((segptr = shmat(shmid, 0, 0)) == (void *) -1)
  {
    perror("shmat hiba");
    exit(1);
  }
  switch (tolower(argv[1][0]))                      /* a param. alapj elvzi a m. */
  {
    case 'w':                                       /* megadott szeg a */
      writeshm(shmid, segptr, argv[2]);
      break;
    case 'r':                                       /* osztott memiaza kiolvasa */
      readshm(shmid, segptr);
      break;
    case 'd':                                       /* torlesl */
      removeshm(shmid);
      break;
    case 'm':                                       /* jogok modositasa */
      changemode(shmid, argv[2]);
      break;
   default:                                        /* hib opci*/
      usage();
  }
}
void writeshm(int shmid, char *segptr, char *text)
{
  strcpy(segptr, text);                             /* szoveeg beirasaa a memoriaba */
  printf("kesz...\n");
}
void readshm(int shmid, char *segptr)
{
  printf("segptr: %s\n", segptr);                   /* mem. tartalmak kiolvasa */
}
void removeshm(int shmid)
{
  shmctl(shmid, IPC_RMID, 0);                       /* osztott memia tle */
  printf("torolve\n");
}
void changemode(int shmid, char *mode)
{
  struct shmid_ds myshmds;
  shmctl(shmid, IPC_STAT, &myshmds);               /* aktuis jogok lekdeze */
  printf("a regi jogok: %o\n", myshmds.shm_perm.mode);
  sscanf(mode, "%o", &myshmds.shm_perm.mode);
  shmctl(shmid, IPC_SET, &myshmds);                /*  uj jogok beitasaala */
  printf("az uj jogok: %o\n", myshmds.shm_perm.mode);
}
void usage(void)                                   /* hasznati mutat*/
{
  printf("shmtool - osztott memoria menedzselo rendszer\n\n");
  printf("HASZNALAT: shmtool (w)rite <szoveg>\n");
  printf("                   (r)ead\n");
  printf("                   (d)elete\n");
  printf("                   (m)ode change <oktalis_mod>\n");
  exit(1);
}
/*A fenti pda tesztelekor a ketkezeredmyre jutottunk:
  $ shmtool w teszt
      letrehozok egy uj osztott memoriazonat
      kesz...
  $ shmtool r
      az osztott memoriazona letezik - megnyitom
      segptr: teszt
  $ shmtool w szasz
      letrehozok egy uj osztott memoriazonat
      kesz...
  $ shmtool r
      az osztott memoriazona letezik - megnyitom
      segptr: szasz
  $ shmtool d
      az osztott memoriazona letezik - megnyitom
      torolve
  $ shmtool m 660
      letrehozok egy uj osztott memoriazonat
      a regi jogok: 666
      az uj jogok: 660
  $
*/                                                                                                                                                                                                                                                                                              
                                                                                                                                                                                                                                                                                               