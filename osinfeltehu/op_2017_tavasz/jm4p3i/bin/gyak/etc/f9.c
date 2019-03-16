/********************************************
Szignálkezelok visszatartasa, szignálcsoportok

Üres csoport letrehozása:
int sigemptyset(sigset_t* csoport)

minden szignált tartalmazó csoport létrehozása:
int sigfillset(sigset_t* csoport)

Egy csoporthoz "újabb" szignál hozzáadása:
int sigaddset(sigset_t* csoport, int szignál)

Egy scoportból egy szignál törlése:
int sigdelset(sigset_t* csoport, int szignál)

A szignál visszatartásának a szabályozása/lekérdezése
A 2. paraméter a megadott csoport, a harmadik paraméter a változtatás elõtti,
visszatartott szignálik listája
int sigprocmask( int parancs, const sigset_t* csoport, sigset_t* régicsoport)
Parancsok:
SIG_BLOCK a csoport hozza lesz adva a visszatartott szignálokhoz
SIG_UNBLOCK a csoport ki lesz véve a visszatartott szignálokból
SIG_SETMASK pontosan azok lesznek visszatartva amelyeket a 2. paraméterben megadunk.


Példa: A program írja ki másodpercenként a természetes számokat 20-ig, 
de CTRL+C leütésére mindig kezdje újra a számolást.



Visszaugrás függvénybõl setjmp-longjmp:

Elmenti a pillanatnyi környezetet a környezet változóba, ide lehet majd visszaugrani.
int setjmp( jmp_buf környezet);
A visszatérési értéke: 
0, ha még csak mentünk és nem visszaugrásból jövünk
nem 0, ha visszaugrásból jövünk

Visszaugrás:
void longjmp( jmp_buf környezet, int vissz_érték );
A környezet változó tartalmaza, hova ugorjunk vissza, a vissza értékkel
lehet megadni, hogy a visszaugrás után a setjmp milyen értékkel térjen vissza,
ez nem lehet 0, különben 1-re lesz cserélve.
A függvény soha nem tér vissza!
********************************************/

#include<stdio.h>
#include<signal.h>
#include<setjmp.h>


jmp_buf  ide_ugrik;

void kezelo( int szignal ){
	sigset_t  szignalok;
	
	sigemptyset( &szignalok );
	sigaddset( &szignalok, SIGINT );
	sigprocmask( SIG_UNBLOCK, &szignalok, NULL );

	longjmp( ide_ugrik, 1 );
}



int main(){
	
	int i;

	signal( SIGINT, kezelo );

	if( setjmp( ide_ugrik ) != 0  ) 
		puts("Ujra kezdem");
	
	for( i=1; i<21; ++i ){
		printf("%d\n",i);
		fflush(NULL);
		sleep(1);
	}
	return 0;
}