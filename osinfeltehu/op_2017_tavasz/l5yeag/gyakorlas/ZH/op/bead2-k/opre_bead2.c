/*
	A "Nyuszimentõ akciótervet" felügyelõ miniszteri biztos (szülõ program) megfigyelõ expedíciókat (gyerek program) szervez! 
	1. A szülõ program paraméterben kapja meg, hogy hány expedíciót kell szerveznie és mennyi ideig tart a nyuszifigyelõ akció. A gyerek programok a helyszínre érve egy-egy jelzéssel tudatják a megérkezésük tényét! 
	2. Miután mindenki a helyszínre ért és visszajelzett a szülõnek, hogy indulhat a munka, a szülõ egy-egy szöveges azonosítót küld jelzés vagy csõ segítségével az expedícióknak. (pl. medve, farkas, oroszlán) Az expedíció kiírja, hogy megkapta az azonosítót, majd visszajelzésben tudatja, hogy adott azonosítójú expedíció megfigyelésre kész! 
	3. Az azonosítás után minden expedíció kezdje el a megfigyeléseit és mindhárom nyuszikorosztályt figyelje meg. Minden expedíció egy idõ múlva (véletlenszerûen megválasztva 2,3,4 vagy 5 másodperc) üzenetsoron keresztül küldje vissza mindhárom megfigyelési adatot és a megfigyelõ azonosítót! A szülõ menti az adatbázisba a kapott megfigyelési adatokat.
	4. A szülõ a "fáradt" expedíciókat a projekt vége elõtt visszahívhatja (véletlenszerûen), míg a maradék expedíciók csak a projekt végén fejezik be tevékenységüket!
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

