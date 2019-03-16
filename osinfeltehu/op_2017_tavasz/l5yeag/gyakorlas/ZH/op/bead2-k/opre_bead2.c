/*
	A "Nyusziment� akci�tervet" fel�gyel� miniszteri biztos (sz�l� program) megfigyel� exped�ci�kat (gyerek program) szervez! 
	1. A sz�l� program param�terben kapja meg, hogy h�ny exped�ci�t kell szerveznie �s mennyi ideig tart a nyuszifigyel� akci�. A gyerek programok a helysz�nre �rve egy-egy jelz�ssel tudatj�k a meg�rkez�s�k t�ny�t! 
	2. Miut�n mindenki a helysz�nre �rt �s visszajelzett a sz�l�nek, hogy indulhat a munka, a sz�l� egy-egy sz�veges azonos�t�t k�ld jelz�s vagy cs� seg�ts�g�vel az exped�ci�knak. (pl. medve, farkas, oroszl�n) Az exped�ci� ki�rja, hogy megkapta az azonos�t�t, majd visszajelz�sben tudatja, hogy adott azonos�t�j� exped�ci� megfigyel�sre k�sz! 
	3. Az azonos�t�s ut�n minden exped�ci� kezdje el a megfigyel�seit �s mindh�rom nyuszikoroszt�lyt figyelje meg. Minden exped�ci� egy id� m�lva (v�letlenszer�en megv�lasztva 2,3,4 vagy 5 m�sodperc) �zenetsoron kereszt�l k�ldje vissza mindh�rom megfigyel�si adatot �s a megfigyel� azonos�t�t! A sz�l� menti az adatb�zisba a kapott megfigyel�si adatokat.
	4. A sz�l� a "f�radt" exped�ci�kat a projekt v�ge el�tt visszah�vhatja (v�letlenszer�en), m�g a marad�k exped�ci�k csak a projekt v�g�n fejezik be tev�kenys�g�ket!
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void parent( int nexp );
pid_t createChild();
void handler( int signumber );
void handler2( int signumber, siginfo_t* info, void* nonused );
char* genId( char** id );

int main( int argc, char** argv )
{
	int nexp = atoi( argv[1] );
	// int exp_duration = atoi( argv[2] );
	signal( SIGTERM, handler );
	
	struct sigaction sigact;
    sigact.sa_sigaction = handler2;
    sigemptyset( &sigact.sa_mask );
    sigact.sa_flags = SA_SIGINFO;
    sigaction( SIGUSR1, &sigact, NULL );
	
	parent( nexp );
	
	return 0;
}

void parent( int nexp )
{
	int status;
	pid_t child = -1;
	
	char id[] = "id00";
	char* ptrid = &id[0];
	genId( &ptrid );
	// printf( "expedition pid %i id %s\n", child, genId( &ptrid ) );
	
	if( nexp > 0 )
	{
		child = createChild();
	}
	
	if( child > 0 )
	{
		pid_t ppid = getpid();
		
		pause();
		
		// char id[] = "id00";
		// char* ptrid = &id[0];
		printf( "expedition pid %i id %s\n", child, ptrid );
		
		union sigval s_value_ptr;
        s_value_ptr.sival_ptr = &ptrid;
		sigqueue( child, SIGUSR1, s_value_ptr );
		
		parent( nexp - 1 );
        
		wait( &status );
		
		if( nexp == 2 && ppid == getpid() )
		{
			sleep( 1 );
			printf( "Parent process ended\n" );
		}
	}
	else if( child == 0 )
	{
		sleep( 1 );
        kill( getppid(), SIGTERM );
		pause();
		sleep(1);
		printf( "pid %i\n", getpid() );
	}
}

pid_t createChild()
{
	pid_t child = fork();
    if( child < 0 )
	{
		perror( "The fork calling was not succesful\n" );
		exit( 1 );
	}
	
	return child;
}

void handler( int signum )
{
    printf( "parent--> Arrived: " );
}

void handler2( int signumber, siginfo_t* info, void* nonused )
{
	char** str = ( char** )info->si_value.sival_ptr;
	printf( "child--> Expedition id %s ", *str );
}

char* genId( char** id )
{
	static int num = 10;
	sprintf(&(( *id )[2]), "%d", num / 10);
	sprintf(&(( *id )[3]), "%d", num % 10);
	++num;
	return *id;
}

