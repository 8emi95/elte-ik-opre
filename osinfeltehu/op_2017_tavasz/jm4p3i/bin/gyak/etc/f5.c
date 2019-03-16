/**************************************************************************
Szignálkezelés 5.

A szignál "lenyelése".

A feladat: a gyerek folyamat írja ki számokat 0-19-ig, azonban, a [7..15] közötti
számok kiírása közben ne fogadjon SIGTERM szignált.
A szûlõ, pedig küldjön 7 alkalommal SIGTERM szignált 3 ms-es idõközönként.

A signal(...) második paramétereként megadható: SIG_IGN konstans hatására
lenyeli az adott típusú szignált.
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
