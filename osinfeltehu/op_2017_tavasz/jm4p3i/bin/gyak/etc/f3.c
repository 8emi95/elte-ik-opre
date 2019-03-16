/**************************************************************************
Szign�lkezel�s 3.

Ha olyan szign�lt k�ld�nk a gyereknek, amelyet nem v�r, akkor megsz�nik a gyerek.
A feladat ugyan az, mint az el�z�n�l, csak most a gyerek v�rjon SIGTERM szign�lt
(ezt szokt�k arra haszn�lni, hogy jelezz�k a folyamatnak j� lenne, ha megsz�nne),
a sz�l�, pedig k�ldj�n SIGINT szign�lt.

V�rjuk: a gyerek folyamat megsz�nik a sz�l� szign�l hat�s�ra.
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
         sleep(5);
         puts("Szulo: kuldom a szignalt a gyereknek");
         kill ( child, SIGINT );
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
