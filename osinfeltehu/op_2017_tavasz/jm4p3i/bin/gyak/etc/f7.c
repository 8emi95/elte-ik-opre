/**************************************************************************
Szign�lkezel�s 7.

A feladat: elind�tjuk az "f7a" folyamatot, majd 5-ms mulva szign�lt k�ld�nk:
   - SIGTERM szign�lt
   - SIGKILL szign�lt
**************************************************************************/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>


int main () {
   pid_t child = fork ();

   if ( child > 0 ) {
       sleep(5);
       puts("Szulo: kilovom a gyereket.");
       kill ( child, SIGTERM );
       //kill ( child, SIGKILL );
   } else if ( child == 0 ) {
       execve ( "f7a", NULL, NULL );
       perror ( "HIBA az f7a inditasanal" );
   } else {
      perror ( "HIBA a fork-nal" );
      exit(1);
   }

   return 0;
}
