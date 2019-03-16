/**************************************************************************
Szignálkezelés 2.

Szignál fogadása:
 signal ( signal_tipus, signalkezelo_eljaras_neve )

Hozzunk létre egy gyerek folyamatot, amelyet bállítunk, hogy fogadja a SIGINT
szignált és írjunk egy szignálkezelõ eljárást, amely kiírja "Gyerek: szignált kaptam".
A gyerek tevékenysége: másodpercenként írja ki a számokat 0-19-ig.
A szülõ folyamat várjon 5 ms-et majd küldjön egy SIGINT szignált a gyereknek.

Várjuk: a gyerek fogadja a SIGINT szignált, lefut a kezelo eljárás,
azaz kiírja "Gyerek: szignált kaptam", mindezt a 0-19 szmok kiírása közben teszi.
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
      signal( SIGINT, kezelo );
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
