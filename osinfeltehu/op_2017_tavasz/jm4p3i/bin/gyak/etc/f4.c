/**************************************************************************
Szignálkezelés 4.

A feladat ugyan az, mint az elõzõnél, csak most a gyerek várjon SIGTERM szignált
(ezt szokták arra használni, hogy jelezzük a folyamatnak jó lenne, ha megszûnne),
a szûlõ, pedig küldjön 5 alkalommal SIGTERM szignált 5 ms-es idõközönként.

Várjuk: A míg a gyerek mûködik, minden alkalommal fogadja a szignált.
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
