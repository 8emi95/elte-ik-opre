#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

void trainerDolgozik();
void versenyzoDolga();
void handler();
void printResultForVesrenyzo( int );
void printResultForTrainer( int );


int p[2];
int p2[2];


int main( int argc, char** argv )
{
	if( argc != 2 ) return -1;
	if( pipe( p ) < 0 ) return -2;

	signal( SIGUSR1, handler );

	int versenyzo = fork();

	if( versenyzo < 0 ) return -3;
	else if( versenyzo == 0 ) versenyzoDolga();

	trainerDolgozik( argv[1] );

	return 0;
}


void trainerDolgozik( char* name )
{
	pause();
	printf( "TRAINER - Versenyzo kesz!\n" );

	int task = rand() % 10;
	int size = strlen( name );
	int num = 1;
	int siker;

	close( p[0] );
	close( p2[1] );

	write( p[1], &task, sizeof( int ) );	
	write( p[1], &size, sizeof( int ) );	
	write( p[1], &name, sizeof( char ) * strlen( name ) );
	write( p[1], &num, sizeof( int ) );	

	pause();
	read( p2[0], &siker, sizeof( int ) );

	printResultForTrainer( siker );

	close( p2[1] );
	close( p[1] );


	wait( NULL );

	exit( 1 );
}

void versenyzoDolga()
{
	kill( getppid(), SIGUSR1 );
	printf( "VERSENYZO - Feladatra jelentkezik!\n" );

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

	int siker = ( rand() % 100 > 50 );

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
		printf( "VERSENYZO - A feladatot nem sikerult megoldani!\n" );
	}		
}

void printResultForTrainer( int siker )
{
	if( siker ) 
	{
		printf( "TRAINER - A versenyzo a feladatot sikeresen megoldotta!\n" );
	}
	else
	{
		printf( "TRAINER - A versenyzonek a feladatot nem sikerult megoldani!\n" );
	}		
}