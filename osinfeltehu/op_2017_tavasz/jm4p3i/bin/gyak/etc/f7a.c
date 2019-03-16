/**************************************************************************
Szign�lkezel�s 7/a.

Ez a folyamat 0-99-ig ki�rja a sz�mokat 1 ms-enk�nt.
Ezt a folyamatot fogjuk elind�tani egy m�sikb�l,
majd megpr�b�ljuk kil�ni.
**************************************************************************/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void kezelo(){
    puts("f7a: lelottek!");
    exit(0);
}


int main () {
      int i;
      signal ( SIGTERM, kezelo );
      for ( i=0; i<100; ++i ) {
         printf("%d\n",i);
	 sleep(1);
      }

   return 0;
}
