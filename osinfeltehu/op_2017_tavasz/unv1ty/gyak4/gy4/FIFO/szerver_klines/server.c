#include <sys/types.h>
#include <sys/stat.h>
#include "struktura.h"                    /* a fent megadott fejléc */


typedef struct elem
{
  int szam;
  int pid;
} azon;



int main(void)
{
  int fd, fd1;                            /* szerver- és kliensfifo */
  char s[15];                             /* kliensfifo neve; pl. fifo_143 */
  azon t;                                 /* küldendő "csomag" */

  mkfifo("szerverfifo", S_IFIFO|0666);    /* a szerver létrehozza a saját fifo-ját */
  fd = open("szerverfifo", O_RDONLY);     /* megnyitja olvasásra; jöhetnek a számok */

  do                                      /* addig megy, míg 0-t nem küld egy kliens */
  {
    read(fd, &t, sizeof(t));              /* szám kiolvasása */
    t.szam = t.szam * t.szam;
    sprintf(s, "fifo_%d", t.pid);         /* a pid segítségével meghat. a kliensfifo nevét */
    fd1 = open(s, O_WRONLY);              /* kliensfifo megnyitása írásra */
    write(fd1, &t, sizeof(t));
    close(fd1);                           /* adatok elküldve, kliensfifo vége */
  } while (!t.szam);
  close(fd);                              /* szerverfifo vége */
  unlink("szerverfifo");                  /* törli a szerverfifot, hiszen ő hozta létre */
  exit(0);
}
