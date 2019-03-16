#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>

//#include "STRUCTS/EVENT.h"

void handler( int sigid ) { ; }
void childTask();
void readEventsFromFile( void );
void skimEventsFromFile( FILE* file );
void openTheFile( FILE** file, char* fname, char* mode );
void makeEventStringToConsole( const int eventCounter );
void makeEventStringToFile( const int eventCounter );
int numberOfLinesInFile( char* fname );
int countGuestsForThisEventId( char* id );
void readEventsFromFile( void );
void readGuestsFromFile( void );
void memoryAllocForEvents();
void readGuestsFromFile();
void skimGuestsFromFile( FILE* file );
void makeGuestStringToConsole( const int guestCounter );
void makeGuestStringToFile( const int guestCounter );
void memoryAllocForGuests();




int myPipe[2];
int myPipeBack[2];
int numberOfEvents;
int numberOfGuests;
struct Event* events;
struct Guest* guests;

struct Event
{
    char* id;
    char* title;
    char* date;

    char* stringToFile;
    char* stringToConsole;
};

struct Guest
{
    char* place;
    char* name;
    char* email;
    char* event_id;
    char* date;

    char* stringToFile;
    char* stringToConsole;
};



int main( int argc, char** argv )
{
	numberOfEvents = numberOfLinesInFile( "DATABASE/events.txt" );	
	memoryAllocForEvents();
	readEventsFromFile();
	//printf( "numberOfEvents: %d\n", numberOfEvents );

	numberOfGuests = numberOfLinesInFile( "DATABASE/guests.txt" );
	memoryAllocForGuests();
	readGuestsFromFile();
	//printf( "numberOfGuests: %d\n", numberOfGuests );

	signal( SIGUSR1, handler );
	srand( time( NULL ) );

	if( pipe( myPipe ) < 0 ) { exit(1); }
	if( pipe( myPipeBack ) < 0 ) { exit(1); }

	int child = fork();

	if( child < 0 )       exit(2); 
	else if( child == 0 ) childTask();

	pause();
	printf( "KingOfStands - Küldöm a rendezvenyek cimeit egymas utan!\n\n" );

	close( myPipe[0] );
	close( myPipeBack[1] );
	int i = 0;
	for( i; i < numberOfEvents; ++i )
	{
		int numGuest = countGuestsForThisEventId( events[i].id );
		int l = strlen( events[i].title );

		printf( "KingOfStands - Küldöm a(z) %d. rendezveny cimet! %s\n", i + 1, events[i].title );
		write( myPipe[1], &l, sizeof( int ) );
		//printf( "KingOfStands - Küldöm a(z) %d. rendezveny azonositojat!\n" );
		write( myPipe[1], &events[i].title, sizeof( char ) * l );
		//printf( "KingOfStands - Küldöm a varhato vendegek letszamat!\n" );
		write( myPipe[1], &numGuest, sizeof( int ) );


		//printf( "KingOfStands - Küldöm a varhato vendegek neveit egymas utan!\n" );
		int j = 0;
		for( j; j < numberOfGuests; ++j )
		{

//printf( "guest event_id: %s,\tevent_id: %s\n", guests[j].event_id, events[i].id );
//getchar();

			if( strcmp( guests[j].event_id, events[i].id ) == 0 )
			{
//printf( "azonosak\n" );
				int l = strlen(guests[j].name);
				write( myPipe[1], &l, sizeof( int ) );				
				write( myPipe[1], &guests[j].name, sizeof( char ) * l );
							
				int eljott;
				read( myPipeBack[0], &eljott, sizeof( int ) );
				if( !eljott )
					printf( "KingOfStands - Az ugynok szerint a(z) %s nevu vendeg nem jott el!\n", guests[j].name );
			}
//			else
//printf( "NEM azonosak\n" );

		}
		//pause();

		int siker = 0;
		read( myPipeBack[0], &siker, sizeof( int ) );
		printf( "KingOfStands - Az ugynok szerint ennyire volt sikeres ez a rendezveny: %d\n\n", siker );

	}
	close( myPipe[1] );
	close( myPipeBack[0] );

	wait( NULL );

	return 0;
} 


void childTask()
{
	signal( SIGUSR1, handler );

	printf( "\nUgynok - Feladatra jelentkezem!\n\n" );
	kill( getppid(), SIGUSR1 );

	close( myPipe[1] );
	close( myPipeBack[0] );
	
	char* id;
	int i = 0;
	for( i; i < numberOfEvents; ++i )
	{
		int numGuest = 0;
		int numOfNotAttend = 0;
		int l = 0;
		read( myPipe[0], &l, sizeof( int ) );		
		read( myPipe[0], &id, sizeof( char ) * l );		
		read( myPipe[0], &numGuest, sizeof( int ) );		

		printf( "Ugynok - A(z) %s cimu helyszinen vagyok!\n", id );
		printf( "Ugynok - Erre a rendezvenyre %d fo hivatalos!\n", numGuest );

		int j = 0;
		for( j; j < numGuest; ++j )
		{
			int l;
			char* name;
			read( myPipe[0], &l, sizeof( int ) );		
			read( myPipe[0], &name, sizeof( char ) * l );		
			printf( "Ugynok - %s\n", name );

			int eljott = (rand() % 100 > 10);
			//if( !eljott ) 
			//	printf("Ugynok - A(z) %s nevu vendeg nem jott el!\n", name );
			write( myPipeBack[1], &eljott, sizeof( int ) );
		}
		
		int siker = rand() % 10 + 1;
		write( myPipeBack[1], &siker, sizeof( int ) );
	}
	
	close( myPipe[0] );
	close( myPipeBack[1] );
	exit( 0 );
}




int countGuestsForThisEventId( char* id )
{
	int counter = 0;
	int i = 0;
	for ( ; i < numberOfGuests; ++i)
	{
		if( strcmp( guests[i].event_id, id ) == 0 )
			counter++;
	}
	return counter;
}

int numberOfLinesInFile( char* fname )
{
    FILE* file;
    openTheFile( &file, fname, "r" );
    int counter = 0;
    char ch;
    while( ( ch = fgetc( file ) ) != EOF )
        if( ch == '\n' ) ++counter;
    fclose( file );
    return counter;
}

void openTheFile( FILE** file, char* fname, char* mode )
{
    *file = fopen( fname, mode );
    if( *file == NULL ) { /*printf("sikertelen.\n");*/ exit( 3 ); }
}













void memoryAllocForEvents()
{
    free( events );
    events = malloc( numberOfEvents * sizeof( struct Event ) );
}


void readEventsFromFile( void )
{
    FILE* file;
    openTheFile( &file, "DATABASE/events.txt", "r" );
    skimEventsFromFile( file );
    fclose( file );
}

void skimEventsFromFile( FILE* file )
{
    char  ch            =  fgetc( file );
    int   eventCounter  =  0;
    while( ch != EOF )
    {
        int dataConter = 0;
        while(     ch != EOF
                    && ch != '\n'
                  )
        {
            int chConter = 0;
            char buffer[ 1000 ];
            while(     ch != EOF
                   && ch != '\n'
                   && ch != ';'
                      )
            {
                buffer[ chConter++ ] = ch;
                ch = fgetc( file );
            }
            buffer[ chConter ] = '\0';
            char* str = buffer;
            switch( dataConter++ )
            {
                case 0:    events[ eventCounter ].id = malloc( chConter );
                        strcpy( events[ eventCounter ].id, str );
                        break;
                case 1: events[ eventCounter ].title = malloc( chConter );
                        strcpy( events[ eventCounter ].title, str );
                        break;
                case 2:  events[ eventCounter ].date = malloc( chConter );
                        strcpy( events[ eventCounter ].date, str );
                        break;
                default:break;
            }
            if( ch != EOF && ch != '\n' ) ch = fgetc( file );
        }
        if( ch != EOF ) ch = fgetc( file );
        if( dataConter < 3 )
        {
            numberOfEvents = 0;
            return;
        }
        makeEventStringToConsole( eventCounter   );
        makeEventStringToFile   ( eventCounter++ );
    }
}

void makeEventStringToConsole( const int eventCounter )
{
    events[ eventCounter ].stringToConsole = malloc(      strlen( events[ eventCounter ].id )
                                                     + strlen( events[ eventCounter ].title )
                                                     + strlen( events[ eventCounter ].date )
                                                     + ( 15 ) );
    strcpy( events[ eventCounter ].stringToConsole, events[ eventCounter ].id );
    strcat( events[ eventCounter ].stringToConsole, "\t" );
    strcat( events[ eventCounter ].stringToConsole, events[ eventCounter ].title );
    strcat( events[ eventCounter ].stringToConsole, "\t" );
    strcat( events[ eventCounter ].stringToConsole, events[ eventCounter ].date );
    strcat( events[ eventCounter ].stringToConsole, "\0" );
}

void makeEventStringToFile( const int eventCounter )
{
    events[ eventCounter ].stringToFile = malloc(      strlen( events[ eventCounter ].id )
                                                  + strlen( events[ eventCounter ].title )
                                                  + strlen( events[ eventCounter ].date )
                                                  + 8 );
    strcpy( events[ eventCounter ].stringToFile, events[ eventCounter ].id ) ;
    strcat( events[ eventCounter ].stringToFile, ";" );
    strcat( events[ eventCounter ].stringToFile, events[ eventCounter ].title );
    strcat( events[ eventCounter ].stringToFile, ";" );
    strcat( events[ eventCounter ].stringToFile, events[ eventCounter ].date );
    strcat( events[ eventCounter ].stringToFile, "\n\0" );
}







void readGuestsFromFile()
{
    FILE* file;
    openTheFile( &file, "DATABASE/guests.txt", "r" );
    skimGuestsFromFile( file );
    fclose( file );
}

void skimGuestsFromFile( FILE* file )
{
    char  ch            =  fgetc( file );
    int   guestCounter  =  0;
    while( ch != EOF )
    {
        int dataConter = 0;
        while(     ch != EOF
                    && ch != '\n'
                  )
        {
            int chConter = 0;
            char buffer[ 1000 ];
            while(     ch != EOF
                        && ch != '\n'
                        && ch != ';'
                      )
            {
                buffer[ chConter++ ] = ch;
                ch = fgetc( file );
            }
            buffer[ chConter ] = '\0';
            char* str = buffer;
            switch( dataConter++ )
            {
                case 0:    guests[ guestCounter ].place = malloc( chConter );
                        strcpy( guests[ guestCounter ].place, str );
                        break;
                case 1: guests[ guestCounter ].name = malloc( chConter );
                        strcpy( guests[ guestCounter ].name, str );
                        break;
                case 2: guests[ guestCounter ].email = malloc( chConter );
                        strcpy( guests[ guestCounter ].email, str );
                        break;
                case 3: guests[ guestCounter ].event_id = malloc( chConter );
                        strcpy( guests[ guestCounter ].event_id, str );
                        break;
                case 4:  guests[ guestCounter ].date = malloc( chConter );
                        strcpy( guests[ guestCounter ].date, str );
                        break;
                default:break;
            }
            if( ch != EOF && ch != '\n' ) ch = fgetc( file );
        }
        if( ch != EOF ) ch = fgetc( file );
        if( dataConter < 5 )
        {
            numberOfGuests = 0;
            return;
        }
        makeGuestStringToConsole( guestCounter );
        makeGuestStringToFile( guestCounter++ );
    }
}

void makeGuestStringToConsole( const int guestCounter )
{
    guests[ guestCounter ].stringToConsole = malloc(      strlen( guests[ guestCounter ].place )
                                                     + strlen( guests[ guestCounter ].name )
                                                     + strlen( guests[ guestCounter ].email )
                                                     + strlen( guests[ guestCounter ].event_id )
                                                     + strlen( guests[ guestCounter ].date )
                                                     + ( 25 ) );
    strcpy( guests[ guestCounter ].stringToConsole, guests[ guestCounter ].place ) ;
    strcat( guests[ guestCounter ].stringToConsole, "\t" );
    strcat( guests[ guestCounter ].stringToConsole, guests[ guestCounter ].name );
    strcat( guests[ guestCounter ].stringToConsole, "\t" );
    strcat( guests[ guestCounter ].stringToConsole, guests[ guestCounter ].email );
    strcat( guests[ guestCounter ].stringToConsole, "\t" );
    strcat( guests[ guestCounter ].stringToConsole, guests[ guestCounter ].event_id );
    strcat( guests[ guestCounter ].stringToConsole, "\t" );
    char* ptr;
    long time_l = strtol( guests[ guestCounter ].date, &ptr, 10 );
    char buffer [ 256 ];
    struct tm* timeinfo;
    timeinfo = localtime( &time_l );
    strftime( buffer, 256, "%Y/%m/%d", timeinfo );
    strcat( guests[ guestCounter ].stringToConsole, buffer );
    strcat( guests[ guestCounter ].stringToConsole, "\0" );
}

void makeGuestStringToFile( const int guestCounter )
{
    guests[ guestCounter ].stringToFile = malloc(      strlen( guests[ guestCounter ].place )
                                                  + strlen( guests[ guestCounter ].name )
                                                  + strlen( guests[ guestCounter ].email )
                                                  + strlen( guests[ guestCounter ].event_id )
                                                  + strlen( guests[ guestCounter ].date )
                                                  + ( 10 ) );
    strcpy( guests[ guestCounter ].stringToFile, guests[ guestCounter ].place ) ;
    strcat( guests[ guestCounter ].stringToFile, ";" );
    strcat( guests[ guestCounter ].stringToFile, guests[ guestCounter ].name );
    strcat( guests[ guestCounter ].stringToFile, ";" );
    strcat( guests[ guestCounter ].stringToFile, guests[ guestCounter ].email );
    strcat( guests[ guestCounter ].stringToFile, ";" );
    strcat( guests[ guestCounter ].stringToFile, guests[ guestCounter ].event_id );
    strcat( guests[ guestCounter ].stringToFile, ";" );
    strcat( guests[ guestCounter ].stringToFile, guests[ guestCounter ].date );
    strcat( guests[ guestCounter ].stringToFile, "\n\0" );
}

void memoryAllocForGuests()
{
    free( guests );
    guests = malloc( numberOfGuests * sizeof( struct Guest ) );
}
