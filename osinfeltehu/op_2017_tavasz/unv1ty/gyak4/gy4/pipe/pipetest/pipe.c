//#include "hdr.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1024

//küldjünk el egy szöveget az apától a fiúnak pipe vezeték segítségével. A gyerek írja ki, hogy hány bájtot olvasott, és jelenítse meg az üzenet szövegét.

int main(void)
{ 
  int pfd[2], n;                              /* pipe változó, olvasott bájtok száma */
  pid_t pid;

  char buf[MAXLINE];                          /* ebbe fogja kiolvasni a gyerek az inf. */
  char test[] = "pipe teszt\n";               /* ezt küldi a gyerek */

  if (pipe(pfd) < 0)                          /* pipe létrehozása */
    printf("pipe hiba");

  if ((pid = fork()) < 0)                     /* gyerekfolyamat létrehozása */
    printf("fork hiba");
  else
    if (pid == 0)                             /* gyerek */
    {
      close(pfd[1]);                          /* fölösleges vezeték bezárása */
      n = read(pfd[0], buf, MAXLINE);         /* információ kiolvasása */
      printf("beolvastam %d bajtot: \n", n);
      fflush(stdout);
      write(1, buf, n);							// 1 = stdout  (0 = stdin)
    }
    else                                      /* szülő */
    {
      close(pfd[0]);                          /* fölösleges vezeték bezárása */
      write(pfd[1], test, sizeof(test));      /* információ beírása a pipe-ba */
    }
  exit(0);
}
