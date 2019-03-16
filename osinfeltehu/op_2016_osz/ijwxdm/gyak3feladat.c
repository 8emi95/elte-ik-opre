#include <stdio.h>
#include <string.h>

struct struktura {
  char  nev[20];
   int  szam;
};

int main( ) {

   struct struktura s;
   strcpy( s.nev, "NEV");
   s.szam = 2;	
	printf("%c","a");

}