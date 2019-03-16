#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>


void trainerDolgozik();
void versenyzoDolga(  int, int );
void handler();
void printResultForVesrenyzo( int );
void printResultForTrainer( int );

int p[2];
int p2[2];
char *s;
key_t kulcs;
int oszt_mem_id;


int main( int argc, char** argv )
{
	if( argc < 2 ) return -1;
	//if( pipe( p ) < 0 ) return -2;
	//if( pipe( p2 ) < 0 ) return -3;


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
	    kulcs=ftok(argv[0],1);
	    oszt_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);
		s = shmat(oszt_mem_id,NULL,0);

		if( pipe( p ) < 0 ) exit(-2);
		if( pipe( p2 ) < 0 ) exit(-2);

		int versenyzo = fork();

		if( versenyzo < 0 ) exit(-3);
		else if( versenyzo == 0 ) versenyzoDolga( i, 1 );

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
		else
		{
			pause();
		    printf(" TRAINER - Ezt olvasta az osztott memoriabol: %s\n",s);
		}

		shmdt(s);
		close( p2[1] );
		close( p[1] );


		wait( NULL );
	    shmctl(oszt_mem_id,IPC_RMID,NULL);
	}

	printf( "\n-------------------\nUJRAPROBA:\n-------------------\n" );
	i = 0;
	for( ; i < hanyanbuktak; ++i )
	{
	    kulcs=ftok(argv[0],1);
	    oszt_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);
		s = shmat(oszt_mem_id,NULL,0);

		if( pipe( p ) < 0 ) exit(-2);
		if( pipe( p2 ) < 0 ) exit(-2);

		int versenyzo = fork();

		if( versenyzo < 0 ) exit(-3);
		else if( versenyzo == 0 ) versenyzoDolga( bukottak[i], 2 );

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

		pause();
	    printf(" TRAINER - Ezt olvasta az osztott memoriabol: %s\n",s);


		shmdt(s);
		close( p2[1] );
		close( p[1] );

		wait( NULL );		
	    shmctl(oszt_mem_id,IPC_RMID,NULL);
	}


	exit( 1 );
}

void versenyzoDolga( int i, int psz )
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
	if(siker && psz == 1)
	{

       // beirunk a memoriaba 
       strcpy(s,name);       
       strcpy(s,"; 1\n");
       // elengedjuk az osztott memoriat
       shmdt(s);       		
	}
	else if(psz == 2)
	{
       // beirunk a memoriaba 
       strcpy(s,name);       
       strcpy(s,"; 2\n");
       // elengedjuk az osztott memoriat
       shmdt(s);       		
	}
	else if( !siker )
	{
       // beirunk a memoriaba 
       strcpy(s,name);       
       strcpy(s,"; Szeretem, ezert maradok!\n");
       // elengedjuk az osztott memoriat
       shmdt(s);       		
	}
	kill( getppid(), SIGUSR1 );

	write( p2[1], &siker, sizeof( int ) );
	sleep( 1 );
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