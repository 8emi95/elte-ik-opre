/**************************************************************************
Szign�lkezel�s 6.

A KILL szign�lt nem lehet fogadni vagy lenyelni.

A feladat: a gyerek folyamat �rja ki sz�mokat 0-19-ig �s
  - fogadjon SIGKILL szign�lt.
  - nyelje le a SIGKILL szign�lt
A sz�l�, pedig k�ldj�n 5 ms eltelt�vel egy SIGKILL szign�lt
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
       puts("Szulo: kuldom a SIGKILL szignalt a gyereknek.");
       kill ( child, SIGKILL );
   } else if ( child == 0 ) {
      //signal( SIGKILL, kezelo );
      signal( SIGKILL, SIG_IGN );
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
