#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

void handler();
void versenyzoKuld();
void trenerKuld();
void handler();
void feladatSikeresegUzenet(int);

int cso[2];
int cso2[2];

int main (int argc, char** argv) {
	if( argc != 2 ) return -1;
	if( pipe( cso ) < 0 ) return -2;

	signal( SIGUSR1, handler );

	int versenyzokSzama = fork();

	if( versenyzokSzama < 0 ) return -3;
	else if( versenyzokSzama == 0 ) versenyzoKuld();

	trenerKuld( argv[1] );

	return 0;
}

void handler(int signumber) {
	;
}

void versenyzoKuld()
{
	kill( getppid(), SIGUSR1 );
	
	
	int feladat;
	char* nev;
	int meret;
	int sorszam;
	
	close( cso[1] );
	close( cso2[0] );
	
	read( cso[0], &feladat, sizeof( int ) );
	read( cso[0], &meret, sizeof( int ) );
	read( cso[0], &nev, sizeof( char ) * meret );
	read( cso[0], &sorszam, sizeof( int ) );
	
	int sikeresE = rand() % 2;
	
	
	feladatSikeresegUzenet(sikeresE);
	
	printf( "VERSENYZO adatok! Feladat: %d  Nev: %s  Sorszam: %d \n", feladat, nev, sorszam );

	write( cso2[1], &sikeresE, sizeof( int ) );
	kill( getppid(), SIGUSR1 );

	close( cso2[1] );
	close( cso[0] );

	exit( 2 );
	
	
}

void trenerKuld( char* nev )
{	
	int sorszam = 1;
	pause();
	printf( "TRAINER - Versenyzo elkezdte a jatekot!\n" );

	
	int feladat = rand();
	int meret = strlen( nev );
	int sorszam = 1;
	int sikeresE;

	close( cso[0] );
	close( cso2[1] );

	write( cso[1], &feladat, sizeof( int ) );	
	write( cso[1], &meret, sizeof( int ) );	
	write( cso[1], &nev, sizeof( char ) * strlen( nev ) );
	write( cso[1], &sorszam, sizeof( int ) );	

	pause();
	read( cso2[0], &sikeresE, sizeof( int ) );

	feladatSikeresegUzenet(sikeresE);

	close( cso2[1] );
	close( cso[1] );


	wait( NULL );

	exit( 1 );
}

void feladatSikeresegUzenet(int sikeresMegoldas)
{
	if(sikeresMegoldas){
		printf("A feladat sikeresen meg lett oldva!!\n");
	}else{
		printf("A feladot nem sikerult megoldani!\n");
	}
}




