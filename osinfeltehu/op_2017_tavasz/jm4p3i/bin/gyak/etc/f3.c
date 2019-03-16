/**************************************************************************
Szignálkezelés 3.

Ha olyan szignált küldünk a gyereknek, amelyet nem vár, akkor megszünik a gyerek.
A feladat ugyan az, mint az elõzõnél, csak most a gyerek várjon SIGTERM szignált
(ezt szokták arra használni, hogy jelezzük a folyamatnak jó lenne, ha megszûnne),
a szûlõ, pedig küldjön SIGINT szignált.

Várjuk: a gyerek folyamat megszûnik a szülõ szignál hatására.
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
