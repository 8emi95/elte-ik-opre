/**************************************************************************
Szign�lkezel�s 8.

Valamilyen tev�kenys�g egy adott id� mulva val�� megszak�t�sa: alarm() rendszerh�v�ssal

Az alarm(masodperc) rendszerhivas ut�n a megadott masodperc mulva az oprendszer
egy SIGALRM szign�lt k�ld.

A feladat: folyamatosan �rjunk ki sz�mokat k�sleltet�s n�lk�l v�gtelen sok�ig,
de 3 ms-nyi sz�m ki�r�s ut�n �lljunk meg 2 ms pihen�re
(itt �rjuk ki, hogy "Na v�gre egy kis pihen�").
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
