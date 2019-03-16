#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>

#define PARTOKSZAMA 6

void handler( int sigId ) { ; }
void elnok();
void ellenor();
void pecsetelo();
void deleteSemafor( char* );
sem_t* createSemafor( char* , int );
int wannaGoOut();
void tryToGoOut( char* name );


int elnokEllenorPipe[2];
int ellenorPecseteloPipe[2];
int pecseteloElnokPipe[2];
int szavazokSzama;

int partok[ PARTOKSZAMA ];

sem_t* mutex;

struct valaszto
{
	int azonosito;
	int ervenyes;
	int szavazasa;
};
 
struct validity
{
	int valid;
	int invalid;	
};

int main( int argc, char** argv )
{
	deleteSemafor( "/mymutex1" );	
	signal( SIGUSR1, handler );
	srand( time( NULL ) );

	mutex = createSemafor( "/mymutex1", 1 );	

	int i = 0;
	for( ; i < PARTOKSZAMA; ++i )
		partok[ i ] = 0;

	if( argc < 2 ) { exit( 3 ); }
	szavazokSzama = atoi( argv[1] );	
	if( szavazokSzama == 0 ) { exit( 3 ); }

	if( pipe( elnokEllenorPipe ) < 0 ) { exit( 42 ); }
	if( pipe( ellenorPecseteloPipe ) < 0 ) { exit( 42 ); }
	if( pipe( pecseteloElnokPipe ) < 0 ) { exit( 42 ); }

	int ellen = fork();
	if( ellen < 0 ) { exit( 3 ); } 
	else if( ellen == 0 ) { ellenor(); exit( 1 ); }

	int pecset = fork();
	if( pecset < 0 ) { exit( 3 ); } 
	else if( pecset == 0 ) { pecsetelo(); exit( 1 ); }

	elnok();	

	deleteSemafor( "/mymutex1" );	

	return 0;
}


void elnok()
{
	pause();
	pause();
	printf( "Elnok - Mindenki kész!\n" );
	printf( "Elnok - Most azonositokat gyartok!\n" );

	int i = 0;
	close( elnokEllenorPipe[0] );
	for( ; i < szavazokSzama; ++i )
	{
		if( wannaGoOut() ) tryToGoOut( "Elnok" );
		int azon = rand();
		write( elnokEllenorPipe[1], &azon, sizeof( int ) );
	}

	close( elnokEllenorPipe[1] );

	close( pecseteloElnokPipe[1] );
	struct valaszto valaszt;
	i = 0;
	for ( ; i < szavazokSzama; ++i)
	{
		if( wannaGoOut() ) tryToGoOut( "Elnok" );
		read( pecseteloElnokPipe[0], &valaszt, sizeof( struct valaszto ) );
		if( valaszt.ervenyes )
			partok[ valaszt.szavazasa ]++;
	}

	printf( "Elnok - Az egyes partok ennyi szavazatot kaptak:\n" );
	i = 0;
	for ( ; i < PARTOKSZAMA; ++i)
	{
		if( wannaGoOut() ) tryToGoOut( "Elnok" );
		printf("%i. part ennyit kapott: %d\n", i, partok[i] );
	}

	struct validity val;
	read( pecseteloElnokPipe[0], &val, sizeof( struct validity ) );
	close( pecseteloElnokPipe[0] );
	printf( "\nElnok - validity adatok:\nervenyes: %i\nervenytelen: %i \n\n", val.valid, val.invalid );

	float valRate =  (float)val.valid / (float)szavazokSzama;	
	float invalRate = (float)val.invalid / (float)szavazokSzama;	

	FILE* file = fopen( "jegyzokonyv.txt", "w" );	
	if( file == NULL ) { exit(4); }
	fprintf( file, "%f - %f", valRate, invalRate );
	fclose( file );

	wait( NULL );
	wait( NULL );
}

void ellenor()
{
	kill( getppid(), SIGUSR1 );
	printf( "Ellenor - Ellenor kesz!\n" );

	close( elnokEllenorPipe[1] );
	close( ellenorPecseteloPipe[0] );
	int azon;
	//while( read( elnokEllenorPipe[0], &azon, sizeof( int ) ) )
	int i = 0;
	for( ; i < szavazokSzama; ++i)
	{
		if( wannaGoOut() ) tryToGoOut( "Ellenor" );
		read( elnokEllenorPipe[0], &azon, sizeof( int ) );
		printf( "Ellenor - A kovetkezo azonositot olvastam: %i \n", azon );
		struct valaszto v;
		v.azonosito = azon;
		v.ervenyes = !( rand() % 100 < 20 );
		write( ellenorPecseteloPipe[1], &v, sizeof( struct valaszto ) ); 		
		//printf("ellenor for-ban\n");
	}

	//printf("itt vagyok ellenor vegen\n");

	close( elnokEllenorPipe[0] );
	close( ellenorPecseteloPipe[1] );
}

void pecsetelo()
{
	sleep( 1 );
	kill( getppid(), SIGUSR1 );
	printf( "Pecsetelo - Pecsetelo kesz!\n" );

	close( ellenorPecseteloPipe[1] );
	close( pecseteloElnokPipe[0] );
	struct valaszto v;
	//while( read( ellenorPecseteloPipe[0], &v, sizeof( struct valaszto ) ) )
	int i = 0;
	int valid = 0;
	int invalid = 0;
	for( ; i < szavazokSzama; ++i)
	{
		if( wannaGoOut() ) tryToGoOut( "Pecsetelo" );
		read( ellenorPecseteloPipe[0], &v, sizeof( struct valaszto ) );
		if( v.ervenyes ) valid++;
		else invalid++;
		v.szavazasa = rand() % PARTOKSZAMA;
		printf( "Pecsetelo - Valaszto: %i, ervenyes: %i, szavazott: %i\n", v.azonosito, v.ervenyes, v.szavazasa );
		write( pecseteloElnokPipe[1], &v, sizeof( struct valaszto ) ); 
	}		
	close( ellenorPecseteloPipe[0] );
	printf( "Pecsetelo - ervenyes: %i, ervenytelen: %i\n", valid, invalid );
	
	//close( pecseteloElnokPipe[0] );
	struct validity val;
	val.valid = valid;
	val.invalid = invalid;
	write( pecseteloElnokPipe[1], &val, sizeof( struct validity ) );
	close( pecseteloElnokPipe[1] );	
}

sem_t* createSemafor( char* nev, int ertek )
{
	sem_t* semid = sem_open( nev, O_CREAT, S_IRUSR, ertek );
	if( semid == SEM_FAILED )
		perror( "sem_open" );
	return semid;
}

void deleteSemafor( char* nev )
{
	sem_unlink( nev );
}

int wannaGoOut()
{
	return (rand() % 100 < 20);
}

void tryToGoOut( char* name )
{
	sem_wait( mutex );
	printf( "%s - Kimentem!\n", name );
	sleep(2);
	printf( "%s - Visszajottem!\n", name );
	sem_post( mutex );
}
