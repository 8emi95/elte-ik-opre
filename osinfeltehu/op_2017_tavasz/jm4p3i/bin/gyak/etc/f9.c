/********************************************
Szign�lkezelok visszatartasa, szign�lcsoportok

�res csoport letrehoz�sa:
int sigemptyset(sigset_t* csoport)

minden szign�lt tartalmaz� csoport l�trehoz�sa:
int sigfillset(sigset_t* csoport)

Egy csoporthoz "�jabb" szign�l hozz�ad�sa:
int sigaddset(sigset_t* csoport, int szign�l)

Egy scoportb�l egy szign�l t�rl�se:
int sigdelset(sigset_t* csoport, int szign�l)

A szign�l visszatart�s�nak a szab�lyoz�sa/lek�rdez�se
A 2. param�ter a megadott csoport, a harmadik param�ter a v�ltoztat�s el�tti,
visszatartott szign�lik list�ja
int sigprocmask( int parancs, const sigset_t* csoport, sigset_t* r�gicsoport)
Parancsok:
SIG_BLOCK a csoport hozza lesz adva a visszatartott szign�lokhoz
SIG_UNBLOCK a csoport ki lesz v�ve a visszatartott szign�lokb�l
SIG_SETMASK pontosan azok lesznek visszatartva amelyeket a 2. param�terben megadunk.


P�lda: A program �rja ki m�sodpercenk�nt a term�szetes sz�mokat 20-ig, 
de CTRL+C le�t�s�re mindig kezdje �jra a sz�mol�st.



Visszaugr�s f�ggv�nyb�l setjmp-longjmp:

Elmenti a pillanatnyi k�rnyezetet a k�rnyezet v�ltoz�ba, ide lehet majd visszaugrani.
int setjmp( jmp_buf k�rnyezet);
A visszat�r�si �rt�ke: 
0, ha m�g csak ment�nk �s nem visszaugr�sb�l j�v�nk
nem 0, ha visszaugr�sb�l j�v�nk

Visszaugr�s:
void longjmp( jmp_buf k�rnyezet, int vissz_�rt�k );
A k�rnyezet v�ltoz� tartalmaza, hova ugorjunk vissza, a vissza �rt�kkel
lehet megadni, hogy a visszaugr�s ut�n a setjmp milyen �rt�kkel t�rjen vissza,
ez nem lehet 0, k�l�nben 1-re lesz cser�lve.
A f�ggv�ny soha nem t�r vissza!
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