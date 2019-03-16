/**************************************************************************
Szign�lkezel�s 1.

Szign�l a v�ratlan kapcsolatteremt�s egyik m�dja.
Pl.: egy folyamat nyomtatni, majd meggonduljuk magunkat �s le kellene
�ll�tani a nyomtat�st. Ekkor k�ld�nk egy szign�lt a nyomtat� folyamatnak,
amelynek hat�s�ra � le�ll�thatja a nyomtat�st. A szign�lok alkalmasak, hardver �ltal felder�tett csapd�k jelz�s�re: �rv�nytelen utas�t�s, vagy lebeg�pontos t�lcsordul�s stb.
Id�intervallum lej�rt�nak a jelz�s�re is haszn�ljuk.

Szign�l k�ld�s:   kill ( kinek_pid, signal_tipus )

Ha egy olyan folyamatnak k�ld�nk szign�lt, amely nem jelezte, hogy hajland� azt fogadni,
a folyamat megsz�nik. N�zz�nk erre egy p�ld�t:

Hozzunk l�tre egy gyerek folyamatot, amely m�sodpercenk�nt ki�rja a sz�mokat 0-19-ig.
A sz�l� folyamat v�rjon 5 ms-et majd k�ldj�n egy szign�lt a gyereknek. (pl.: SIGINT
, amely a DEL le�t�sekor szokott keletkezni)

V�rjuk: a gyerek termin�l, azaz nem tudja ki�rna a sz�mokat 19-ig (4-5 k�rny�k�n
megszakad)
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
      puts("Szulo most fogja kuldeni a szignalt a gyereknek");
      kill ( child, SIGINT );
   } else if ( child == 0 ) {
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
