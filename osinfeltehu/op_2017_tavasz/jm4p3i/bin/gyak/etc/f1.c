/**************************************************************************
Szignálkezelés 1.

Szignál a váratlan kapcsolatteremtés egyik módja.
Pl.: egy folyamat nyomtatni, majd meggonduljuk magunkat és le kellene
állítani a nyomtatást. Ekkor küldünk egy szignált a nyomtató folyamatnak,
amelynek hatására õ leállíthatja a nyomtatást. A szignálok alkalmasak, hardver által felderített csapdák jelzésére: érvénytelen utasítás, vagy lebegõpontos túlcsordulás stb.
Idõintervallum lejártának a jelzésére is használjuk.

Szignál küldés:   kill ( kinek_pid, signal_tipus )

Ha egy olyan folyamatnak küldünk szignált, amely nem jelezte, hogy hajlandó azt fogadni,
a folyamat megszûnik. Nézzünk erre egy példát:

Hozzunk létre egy gyerek folyamatot, amely másodpercenként kiírja a számokat 0-19-ig.
A szülõ folyamat várjon 5 ms-et majd küldjön egy szignált a gyereknek. (pl.: SIGINT
, amely a DEL leütésekor szokott keletkezni)

Várjuk: a gyerek terminál, azaz nem tudja kiírna a számokat 19-ig (4-5 környékén
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
