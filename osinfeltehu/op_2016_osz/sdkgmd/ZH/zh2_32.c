#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>

void trainerDolgozik();
void versenyzoDolga();
void handler();
void printResultForVesrenyzo( int );
void printResultForTrainer( int );

int p[2];
int p2[2];


int main( int argc, char** argv )
{
	if( argc < 2 ) return -1;
	//if( pipe( p ) < 0 ) return -2;
	//if( pipe( p2 ) < 0 ) return -3;

	printf( "number of arguments: %d\n", argc );

	srand( time( NULL ) );

	signal( SIGUSR1, handler );

	trainerDolgozik( argc, argv );

	return 0;
}


void trainerDolgozik( int argc, char** argv )
{
	int bukottak[argc];
	int hanyanbuktak = 0;

	int i = 1;
	for( ; i < argc; ++i )
	{		
		if( pipe( p ) < 0 ) exit(-2);
		if( pipe( p2 ) < 0 ) exit(-2);

		int versenyzo = fork();

		if( versenyzo < 0 ) exit(-3);
		else if( versenyzo == 0 ) versenyzoDolga( i );

		pause();
		printf( "TRAINER - %d. Versenyzo kesz!\n", i );

		int task = rand() % 10;
		int size = strlen( argv[i] );
		int num = i;
		int siker;

		close( p[0] );
		close( p2[1] );

		write( p[1], &task, sizeof( int ) );	
		write( p[1], &size, sizeof( int ) );	
		write( p[1], &argv[i], sizeof( char ) * strlen( argv[i ] ) );
		write( p[1], &num, sizeof( int ) );	

		pause();
		read( p2[0], &siker, sizeof( int ) );

		printf( "Szulonel a siker: %d\n", siker );
		printResultForTrainer( siker );
		if(!siker)
		{
			bukottak[hanyanbuktak] = i;
			++hanyanbuktak;
		}

		close( p2[1] );
		close( p[1] );

		wait( NULL );
	}

	printf( "\n-------------------\nUJRAPROBA:\n-------------------\n" );
	i = 0;
	for( ; i < hanyanbuktak; ++i )
	{
		if( pipe( p ) < 0 ) exit(-2);
		if( pipe( p2 ) < 0 ) exit(-2);

		int versenyzo = fork();

		if( versenyzo < 0 ) exit(-3);
		else if( versenyzo == 0 ) versenyzoDolga( bukottak[i] );

		pause();
		printf( "TRAINER - %d. Versenyzo kesz!\n", ( bukottak[i] + 1 ) );

		int task = rand() % 10;
		int size = strlen( argv[bukottak[i]] );
		int num = i;
		int siker;

		close( p[0] );
		close( p2[1] );

		write( p[1], &task, sizeof( int ) );	
		write( p[1], &size, sizeof( int ) );	
		write( p[1], &argv[bukottak[i]], sizeof( char ) * strlen( argv[bukottak[i]] ) );
		write( p[1], &num, sizeof( int ) );	

		pause();
		read( p2[0], &siker, sizeof( int ) );

		printf( "Szulonel a siker: %d\n", siker );
		printResultForTrainer( siker );

		close( p2[1] );
		close( p[1] );

		wait( NULL );		
	}


	exit( 1 );
}

void versenyzoDolga( int i )
{
	kill( getppid(), SIGUSR1 );
	printf( "%d. VERSENYZO - Feladatra jelentkezik!\n", i );

	int task;
	char* name;
	int size;
	int num;

	close( p[1] );
	close( p2[0] );

	read( p[0], &task, sizeof( int ) );
	read( p[0], &size, sizeof( int ) );
	read( p[0], &name, sizeof( char ) * size );
	read( p[0], &num, sizeof( int ) );
	printf( "VERSENYZO - Megkaptam az adatokat!\nFeladat: %d\nNev: %s\nSorszam: %d\n", task, name, num );

	int siker = ( rand() % 100 > 80 );
	printf( "Vesenyzonel a siker: %d\n", siker );

	printResultForVesrenyzo( siker );

	write( p2[1], &siker, sizeof( int ) );
	kill( getppid(), SIGUSR1 );

	close( p2[1] );
	close( p[0] );

	exit( 2 );
}

void handler()
{
	;
}

void printResultForVesrenyzo( int siker )
{
	if( siker ) 
	{
		printf( "VERSENYZO - A feladatot sikeresen megoldottam!\n" );
	}
	else
	{
		printf( "VERSENYZO - A feladatot nem sikerult megoldanom!\n" );
	}		
}

void printResultForTrainer( int siker )
{
	if( siker ) 
	{
		printf( "TRAINER - A versenyzo a feladatot sikeresen megoldotta!\n\n" );
	}
	else
	{
		printf( "TRAINER - A versenyzonek a feladatot nem sikerult megoldani!\n\n" );
	}		
}