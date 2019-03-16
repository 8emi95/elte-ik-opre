#include "GUEST_FUNCTIONS.h"


VOID countGuests( VOID )
{
    numberOfGuests SHOULD_BE numberOfLinesInFile( GUESTS_FILE );
}

VOID deleteGuest( VOID )
{
    CREATE_THIS_FILE_OBJECT file;
    openTheFile( &file, GUESTS_FILE, TO_WRITE );
    fclose( file );
    INT i;
    BOOL after SHOULD_BE FALSE;
    FOR( i SHOULD_BE ZERO; i IS_LESS_THEN numberOfGuests; ++i )
    {
        IF( cursor MINUS_ONE IS_NOT_EQUAL_TO i )
        {
            IF( after )
                decreaseGuestPlaceByOne( &guests[i] );
            appendNewGuestToEndOfGuestFile( guests[ i ] );
        }
        ELSE
        {
            after SHOULD_BE TRUE;
        }
    }
    refreshGuests();
}

VOID decreaseGuestPlaceByOne( STRUCT Guest* guest )
{
    STRING place  SHOULD_BE  malloc( BYTE );
    CHAR buffer[ BYTE ];
    place         SHOULD_BE  itoa( ( atoi( guest->place) MINUS_ONE ), buffer, TENTH_NUMBER_SYSTEM );
    free( guest->place );
    guest->place  SHOULD_BE malloc( strlen( place ) );
    strcpy( guest->place, place );
}

VOID refreshGuests( VOID )
{
    countGuests();
    memoryAllocForGuests();
    readGuestsFromFile();
}

VOID memoryAllocForGuests( VOID )
{
    free( guests );
    guests GETS_MEMORY malloc( numberOfGuests TIMES SIZEOF( STRUCT Guest ) );
}

VOID readGuestsFromFile( VOID )
{
    CREATE_THIS_FILE_OBJECT file;
    openTheFile( &file, GUESTS_FILE, TO_READ );
    skimGuestsFromFile( file );
    fclose( file );
}

VOID skimGuestsFromFile( FILE_POINTER file )
{
    CHAR  ch            SHOULD_BE  fgetc( file );
    INT   guestCounter  SHOULD_BE  ZERO;
    AS_LONG_AS( ch IS_NOT EOF )
    {
        INT dataConter SHOULD_BE ZERO;
        AS_LONG_AS(     ch IS_NOT END_OF_FILE
                    AND ch IS_NOT END_OF_LINE_CHAR
                  )
        {
            INT chConter SHOULD_BE ZERO;
            CHAR buffer[ THOUSAND ];
            AS_LONG_AS(     ch IS_NOT END_OF_FILE
                        AND ch IS_NOT END_OF_LINE_CHAR
                        AND ch IS_NOT SEPARATOR_CHAR
                      )
            {
                buffer[ chConter++ ] SHOULD_BE ch;
                ch SHOULD_BE fgetc( file );
            }
            buffer[ chConter ] SHOULD_BE TERMINATING_CHAR;
            STRING str SHOULD_BE buffer;
            ACCORDING_TO( dataConter++ )
            {
                IN_CASE_OF GUEST_PLACE_DATA:    guests[ guestCounter ].place SHOULD_BE malloc( chConter );
                        strcpy( guests[ guestCounter ].place, str );
                        AND_STOP_HERE;
                IN_CASE_OF GUEST_NAME_DATA: guests[ guestCounter ].name SHOULD_BE malloc( chConter );
                        strcpy( guests[ guestCounter ].name, str );
                        AND_STOP_HERE;
                IN_CASE_OF GUEST_EMAIL_DATA: guests[ guestCounter ].email SHOULD_BE malloc( chConter );
                        strcpy( guests[ guestCounter ].email, str );
                        AND_STOP_HERE;
                IN_CASE_OF GUEST_EVENTID_DATA: guests[ guestCounter ].event_id SHOULD_BE malloc( chConter );
                        strcpy( guests[ guestCounter ].event_id, str );
                        AND_STOP_HERE;
                IN_CASE_OF GUEST_DATE_DATA:  guests[ guestCounter ].date SHOULD_BE malloc( chConter );
                        strcpy( guests[ guestCounter ].date, str );
                        AND_STOP_HERE;
                DO_NOTHING BY_DEFAULT;
            }
            IF( ch IS_NOT EOF AND ch IS_NOT END_OF_LINE_CHAR ) ch SHOULD_BE fgetc( file );
        }
        IF( ch IS_NOT EOF ) ch SHOULD_BE fgetc( file );
        IF( dataConter IS_LESS_THEN NUMBER_OF_EVENT_DATA )
        {
            numberOfGuests SHOULD_BE ZERO;
            RETURN;
        }
        makeGuestStringToConsole( guestCounter );
        makeGuestStringToFile( guestCounter++ );
    }
}

VOID makeGuestStringToConsole( CONST INT guestCounter )
{
    guests[ guestCounter ].stringToConsole = malloc(      strlen( guests[ guestCounter ].place )
                                                     PLUS strlen( guests[ guestCounter ].name )
                                                     PLUS strlen( guests[ guestCounter ].email )
                                                     PLUS strlen( guests[ guestCounter ].event_id )
                                                     PLUS strlen( guests[ guestCounter ].date )
                                                     PLUS ( FIVE TIMES NUMBER_OF_GUEST_DATA ) );
    strcpy( guests[ guestCounter ].stringToConsole, guests[ guestCounter ].place ) ;
    strcat( guests[ guestCounter ].stringToConsole, TABULATOR_STRING );
    strcat( guests[ guestCounter ].stringToConsole, guests[ guestCounter ].name );
    strcat( guests[ guestCounter ].stringToConsole, TABULATOR_STRING );
    strcat( guests[ guestCounter ].stringToConsole, guests[ guestCounter ].email );
    strcat( guests[ guestCounter ].stringToConsole, TABULATOR_STRING );
    strcat( guests[ guestCounter ].stringToConsole, guests[ guestCounter ].event_id );
    strcat( guests[ guestCounter ].stringToConsole, TABULATOR_STRING );
    CHAR* ptr;
    LONG time_l SHOULD_BE strtol( guests[ guestCounter ].date, &ptr, TENTH_NUMBER_SYSTEM );
    CHAR buffer [ BYTE ];
    STRUCT tm* timeinfo;
    timeinfo SHOULD_BE localtime( &time_l );
    strftime( buffer, BYTE, DATEFORMAT, timeinfo );
    strcat( guests[ guestCounter ].stringToConsole, buffer );
    strcat( guests[ guestCounter ].stringToConsole, END_LINE_CHAR_PLUS_TERMINATING_CHAR_STRING );
}

VOID makeGuestStringToFile( CONST INT guestCounter )
{
    guests[ guestCounter ].stringToFile = malloc(      strlen( guests[ guestCounter ].place )
                                                  PLUS strlen( guests[ guestCounter ].name )
                                                  PLUS strlen( guests[ guestCounter ].email )
                                                  PLUS strlen( guests[ guestCounter ].event_id )
                                                  PLUS strlen( guests[ guestCounter ].date )
                                                  PLUS ( NUMBER_OF_EVENT_DATA PLUS FIVE ) );
    strcpy( guests[ guestCounter ].stringToFile, guests[ guestCounter ].place ) ;
    strcat( guests[ guestCounter ].stringToFile, SEPARATOR_STRING );
    strcat( guests[ guestCounter ].stringToFile, guests[ guestCounter ].name );
    strcat( guests[ guestCounter ].stringToFile, SEPARATOR_STRING );
    strcat( guests[ guestCounter ].stringToFile, guests[ guestCounter ].email );
    strcat( guests[ guestCounter ].stringToFile, SEPARATOR_STRING );
    strcat( guests[ guestCounter ].stringToFile, guests[ guestCounter ].event_id );
    strcat( guests[ guestCounter ].stringToFile, SEPARATOR_STRING );
    strcat( guests[ guestCounter ].stringToFile, guests[ guestCounter ].date );
    strcat( guests[ guestCounter ].stringToFile, END_LINE_CHAR_PLUS_TERMINATING_CHAR_STRING );
}

VOID goToModifyGuestMenu( VOID )
{
    menu   SHOULD_BE ON_GUEST_MODIFY_MENU;
}

VOID modifyGuest( VOID )
{
    printf( MODIFICATION_OPTIONS_FOR_GUEST );
    CHAR ch SHOULD_BE EMPTY_CHAR;
    WHILE(     ch IS_NOT MOD_OPT_GUE_1
           AND ch IS_NOT MOD_OPT_GUE_2
           AND ch IS_NOT MOD_OPT_GUE_3
         )      ch SHOULD_BE getch();
    ACCORDING_TO( ch )
    {
        IF_IT_IS MOD_OPT_GUE_1  THEN    modifyNameOnGuest();      AND_STOP_HERE;
        IF_IT_IS MOD_OPT_GUE_2  THEN    modifyEmailOnGuest();     AND_STOP_HERE;
        IF_IT_IS MOD_OPT_GUE_3  THEN    modifyEventIdOnGuest();   AND_STOP_HERE;
        DO_NOTHING BY_DEFAULT;
    }
    saveGuests();
    refreshGuests();
    goBackToPreviousMenu();
    catFileToConsole( SUCCESSFUL_GUEST_MODIFICATION );
}

VOID saveGuests( VOID )
{
    CREATE_THIS_FILE_OBJECT file;
    openTheFile( &file, GUESTS_FILE, TO_WRITE );
    fclose( file );
    INT i;
    FOR( i SHOULD_BE ZERO; i IS_LESS_THEN numberOfGuests; ++i )
        appendNewGuestToEndOfGuestFile( guests[ i ] );
}

VOID modifyNameOnGuest( VOID )
{
    STRUCT Guest* g SHOULD_BE &guests[ cursor MINUS_ONE ];
    getGuestName( g );
}

VOID modifyEmailOnGuest( VOID )
{
    STRUCT Guest* g SHOULD_BE &guests[ cursor MINUS_ONE ];
    getGuestEmail( g );
}

VOID modifyEventIdOnGuest( VOID )
{
    STRUCT Guest* g SHOULD_BE &guests[ cursor MINUS_ONE ];
    getGuestEventId( g );
}

VOID goToCreateNewGuestMenu( VOID )
{
    menu    SHOULD_BE  ON_NEW_GUEST_MENU;
    cursor  SHOULD_BE  STARTING_POSITION;
}

VOID createNewGuest( VOID )
{
    STRUCT Guest newGuest;
    getNewGuestDataFromUser( &newGuest );
    appendNewGuestToEndOfGuestFile( newGuest );
    refreshGuests();
    goBackToPreviousMenu();
    catFileToConsole( SUCCESSFUL_GUEST_CREATE );
}

VOID getNewGuestDataFromUser( STRUCT Guest* newGuest )
{
    printf( CREATE_NEW_GUEST_STRING );
    getGuestNextPlace( newGuest );
    getGuestName     ( newGuest );
    getGuestEmail    ( newGuest );
    getGuestEventId  ( newGuest );
    getGuestDate     ( newGuest );
}


VOID getGuestNextPlace( STRUCT Guest* guest )
{
    CHAR buffer[ BYTE ];
    STRING nextPlace  SHOULD_BE  malloc( BYTE );
    nextPlace         SHOULD_BE  itoa( ( numberOfGuests PLUS_ONE ), buffer, TENTH_NUMBER_SYSTEM );
    guest->place = malloc( strlen( nextPlace ) );
    strcpy( guest->place, nextPlace );
}

VOID getGuestName( STRUCT Guest* guest )
{
    guest->name SHOULD_BE NULL;
    printf( ASK_FOR_GUEST_NAME );
    WHILE( guest->name IS NULL )
        guest->name  SHOULD_BE getLINE();
}

VOID getGuestEmail( STRUCT Guest* guest )
{
    guest->email SHOULD_BE NULL;
    printf( ASK_FOR_GUEST_EMAIL );
    WHILE( guest->email IS NULL )
        guest->email  SHOULD_BE getLINE();
}

VOID getGuestEventId( STRUCT Guest* guest )
{
    guest->event_id SHOULD_BE NULL;
    printf( ASK_FOR_EVENT_ID );
    WHILE( guest->event_id IS NULL )
        guest->event_id  SHOULD_BE getLINE();
}

VOID getGuestDate( STRUCT Guest* guest )
{
    time_t* rawtime = (time_t*)malloc( sizeof( time_t ) );
    time( rawtime );
    char* str = malloc( 256 * sizeof(char) );
    sprintf( str, "%ld", (long)*rawtime );
    guest->date = str;
}

VOID appendNewGuestToEndOfGuestFile( STRUCT Guest guest )
{
    CREATE_THIS_FILE_OBJECT file;
    openTheFile( &file, GUESTS_FILE, TO_APPEND );
    STRING str SHOULD_BE malloc(      strlen( guest.place )
                                 PLUS strlen( guest.name )
                                 PLUS strlen( guest.email )
                                 PLUS strlen( guest.event_id )
                                 PLUS strlen( guest.date )
                                 PLUS NUMBER_OF_GUEST_DATA );
    strcpy( str, guest.place );
    strcat( str, SEPARATOR_STRING );
    strcat( str, guest.name );
    strcat( str, SEPARATOR_STRING );
    strcat( str, guest.email );
    strcat( str, SEPARATOR_STRING );
    strcat( str, guest.event_id );
    strcat( str, SEPARATOR_STRING );
    strcat( str, guest.date );
    strcat( str, TERMINATING_STRING );
    fprintf( file, STRING_PLUS_NEW_LINE, str );
    fclose( file );
}
