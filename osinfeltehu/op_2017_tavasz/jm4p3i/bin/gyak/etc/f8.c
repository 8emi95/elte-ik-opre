/**************************************************************************
Szignálkezelés 8.

Valamilyen tevékenység egy adott idõ mulva valõó megszakítása: alarm() rendszerhívással

Az alarm(masodperc) rendszerhivas után a megadott masodperc mulva az oprendszer
egy SIGALRM szignált küld.

A feladat: folyamatosan írjunk ki számokat késleltetés nélkül végtelen sokáig,
de 3 ms-nyi szám kiírás után álljunk meg 2 ms pihenõre
(itt írjuk ki, hogy "Na végre egy kis pihenõ").
**************************************************************************/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void kezelo(){
   puts("\nNa vegre egy kis piheno");
   sleep(2);
   alarm(3);
}


int main () {
   signal( SIGALRM, kezelo );
   alarm(3);
   int i=0;
   while(1)
      printf("%d",++i);

   return 0;
}
