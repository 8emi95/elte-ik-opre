/**************************************************************************
Szign�lkezel�s 4.

A feladat ugyan az, mint az el�z�n�l, csak most a gyerek v�rjon SIGTERM szign�lt
(ezt szokt�k arra haszn�lni, hogy jelezz�k a folyamatnak j� lenne, ha megsz�nne),
a sz�l�, pedig k�ldj�n 5 alkalommal SIGTERM szign�lt 5 ms-es id�k�z�nk�nt.

V�rjuk: A m�g a gyerek m�k�dik, minden alkalommal fogadja a szign�lt.
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
   int status
   pid_t child = fork ();

   if ( child > 0 ) {
      int j;
      for(j=0; j<5; ++j){
       sleep(5);
       puts("Szulo: kuldom a szignalt a gyereknek.");
       kill ( child, SIGTERM );
      }
      wait(child, &status);
   } else if ( child == 0 ) {
      signal( SIGTERM, kezelo );
      int i;
      for ( i=0; i<20; ++i ) {
         printf("%d\n",i);
	 sleep(1);
      }
   } else {
      perror ( "HIBA a fork-nal" );
      exit(1);
   }

   return 0;
}
