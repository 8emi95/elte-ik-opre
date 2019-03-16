/**************************************************************************
Szign�lkezel�s 5.

A szign�l "lenyel�se".

A feladat: a gyerek folyamat �rja ki sz�mokat 0-19-ig, azonban, a [7..15] k�z�tti
sz�mok ki�r�sa k�zben ne fogadjon SIGTERM szign�lt.
A sz�l�, pedig k�ldj�n 7 alkalommal SIGTERM szign�lt 3 ms-es id�k�z�nk�nt.

A signal(...) m�sodik param�terek�nt megadhat�: SIG_IGN konstans hat�s�ra
lenyeli az adott t�pus� szign�lt.
**************************************************************************/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void kezelo(){
   puts("Gyerek: szignalt kaptam");
}


int main () {
   pid_t child = fork ();

   if ( child > 0 ) {
      int j;
      for(j=0; j<7; ++j){
       puts("Szulo: kuldom a szignalt a gyereknek.");
       kill ( child, SIGTERM );
       sleep(3);
      }
   } else if ( child == 0 ) {
      signal( SIGTERM, kezelo );
      int i;
      for ( i=0; i<20; ++i ) {
         if(i==7){ signal(SIGTERM,SIG_IGN); }
         printf("%d\n",i);
	 if(i==15){ signal(SIGTERM,kezelo); }
	 sleep(1);
      }
   } else {
      perror ( "HIBA a fork-nal" );
      exit(1);
   }

   return 0;
}
