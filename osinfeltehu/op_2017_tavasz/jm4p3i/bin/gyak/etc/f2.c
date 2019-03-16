/**************************************************************************
Szign�lkezel�s 2.

Szign�l fogad�sa:
 signal ( signal_tipus, signalkezelo_eljaras_neve )

Hozzunk l�tre egy gyerek folyamatot, amelyet b�ll�tunk, hogy fogadja a SIGINT
szign�lt �s �rjunk egy szign�lkezel� elj�r�st, amely ki�rja "Gyerek: szign�lt kaptam".
A gyerek tev�kenys�ge: m�sodpercenk�nt �rja ki a sz�mokat 0-19-ig.
A sz�l� folyamat v�rjon 5 ms-et majd k�ldj�n egy SIGINT szign�lt a gyereknek.

V�rjuk: a gyerek fogadja a SIGINT szign�lt, lefut a kezelo elj�r�s,
azaz ki�rja "Gyerek: szign�lt kaptam", mindezt a 0-19 szmok ki�r�sa k�zben teszi.
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
