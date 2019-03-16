#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "struktura.h"                    /* a fenti fejlécállomány */

typedef struct elem
{
  int szam;
  int pid;
} azon;

int main(int argc, char *argv[])          /* a számot a parancssorban adjuk meg */
{
  int fd, fd1;                            /* kliens- és szerverfifo */
  char s[15];
  azon t;

  if (argc != 2)                          /* nincs megadva argumentum, hiba */
  {
    printf("hasznalat: kliens <szam>\n");
    exit(1);
  }

  sprintf(s, "fifo_%d", getpid());        /* meghat. a fifonevet a pid segítségével */
  mkfifo(s, S_IFIFO|0666);                /* létrehoz egy kliensfifot */
  fd = open("szerverfifo", O_WRONLY);
  t.pid = getpid();                       /* a küldendő adatok */
  t.szam = atoi(argv[1]);                 /* string átalakítása számmá */
  write(fd, &t, sizeof(t));               /* küldi a szervernek */
  fd1 = open(s, O_RDONLY);
  read(fd1, &t, sizeof(t));               /* a válasz */
  close(fd1);
  unlink(fd1);                            /* kliensfifo törlése */
  printf("a negyzete: %d\n", t.szam);
  exit(0);
}
