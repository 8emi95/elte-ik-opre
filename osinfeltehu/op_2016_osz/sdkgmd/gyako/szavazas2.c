#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>


void ellenorDolga();
void pecseteloDolga();
void elnokDolga();
void handler(int);

int p[2];
int p2[2];
int resztvevokSzama = 0;

struct szavazat
{
	int azon;
	int ervenyes;
};

int main( int argc, char** argv )
{
	if( argc < 2 ) return -1;
	if( pipe( p ) < 0 ) return -2;
	resztvevokSzama = atoi( argv[1] );
	if( resztvevokSzama == 0 ) return -3;

	srand( time( NULL ) );
	signal( SIGUSR1, handler );

	int ellenor = fork();
	if( ellenor < 0 ) return -4;
	else if( ellenor == 0 ) ellenorDolga();

	int pecsetelo = fork();
	if( pecsetelo < 0 ) return -5;
	else if( pecsetelo == 0 ) pecseteloDolga();

	elnokDolga();

	return 0;
}

void elnokDolga()
{
	pause();
	pause();
	printf( "Elnok - Mindenki kesz!\n" );

	printf( "Elnok - Most azonositokat gyartok! %d\n", resztvevokSzama );
	close( p[0] );
	int i = 0;
	for( ; i < resztvevokSzama; ++i )
	{
		int azon = rand();
		printf( "Elnok - Kuldom a %d azonositot!\n", azon );
		write( p[1], &azon, sizeof( int ) ); 
	}
	close( p[1] );

	wait( NULL );
	wait( NULL );
}

void ellenorDolga()
{
	sleep( 1 );
	kill( getppid(), SIGUSR1 );
	printf( "Ellenor - Keszen allok!\n" );

	close( p[1] );	
	close( p2[0] );	
	int i = 0;
	for( ; i < resztvevokSzama; ++i )
	{
		struct szavazat sz;
		read( p[0], &sz.azon, sizeof( int ) );
		printf( "Ellenor - Megkaptam a %d azonositot!\n", sz.azon );
		sz.ervenyes = ( rand() % 100 > 80 );
		printf( "Ellenor - Kuldom a %d azonositoju szavazatot!\n", sz.azon );
		write( p2[1], &sz, sizeof( struct szavazat ) );
	}
	close( p2[1] );	
	close( p[0] );	

	exit(1);
}

void pecseteloDolga()
{
	kill( getppid(), SIGUSR1 );
	printf( "Pecsetelo - Keszen allok!\n" );

	close( p2[1] );
	int i = 0;
	for( ; i < resztvevokSzama; ++i )
	{
		struct szavazat sz;
		read( p[0], &sz, sizeof( struct szavazat ) );
		printf( "Pecsetelo - A %d szavazatot megkaptam, ervenyessege: %d", sz.azon, sz.ervenyes );
	}
	close( p2[0] );

	exit(2);
}

void handler( int sigId )
{
	;
}
