#include "EVENT_FUNCTIONS.h"


VOID countEvents( VOID )
{
    numberOfEvents SHOULD_BE numberOfLinesInFile( EVENTS_FILE );
}

VOID deleteEvent( VOID )
{
    CREATE_THIS_FILE_OBJECT file;
    openTheFile( &file, EVENTS_FILE, TO_WRITE );
    fclose( file );
    INT i;
    FOR( i SHOULD_BE ZERO; i IS_LESS_THEN numberOfEvents; ++i )
        IF( cursor MINUS_ONE IS_NOT_EQUAL_TO i )
            appendNewEventToEndOfEventFile( &events[ i ] );
    refreshEvents();
}

VOID refreshEvents( VOID )
{
    countEvents();
    memoryAllocForEvents();
    readEventsFromFile();
}

VOID memoryAllocForEvents( VOID )
{
    free( events );
    events GETS_MEMORY malloc( numberOfEvents TIMES SIZEOF( STRUCT Event ) );
}

VOID readEventsFromFile( VOID )
{
    CREATE_THIS_FILE_OBJECT file;
    openTheFile( &file, EVENTS_FILE, TO_READ );
    skimEventsFromFile( file );
    fclose( file );
}

VOID skimEventsFromFile( FILE_POINTER file )
{
    CHAR  ch            SHOULD_BE  fgetc( file );
    INT   eventCounter  SHOULD_BE  ZERO;
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
                IN_CASE_OF EVENT_ID_DATA:    events[ eventCounter ].id SHOULD_BE malloc( chConter );
                        strcpy( events[ eventCounter ].id, str );
                        AND_STOP_HERE;
                IN_CASE_OF EVENT_TITLE_DATA: events[ eventCounter ].title SHOULD_BE malloc( chConter );
                        strcpy( events[ eventCounter ].title, str );
                        AND_STOP_HERE;
                IN_CASE_OF EVENT_DATE_DATA:  events[ eventCounter ].date SHOULD_BE malloc( chConter );
                        strcpy( events[ eventCounter ].date, str );
                        AND_STOP_HERE;
                DO_NOTHING BY_DEFAULT;
            }
            IF( ch IS_NOT EOF AND ch IS_NOT END_OF_LINE_CHAR ) ch SHOULD_BE fgetc( file );
        }
        IF( ch IS_NOT EOF ) ch SHOULD_BE fgetc( file );
        IF( dataConter IS_LESS_THEN NUMBER_OF_EVENT_DATA )
        {
            numberOfEvents SHOULD_BE ZERO;
            RETURN;
        }
        makeEventStringToConsole( eventCounter   );
        makeEventStringToFile   ( eventCounter++ );
    }
}

VOID makeEventStringToConsole( CONST INT eventCounter )
{
    events[ eventCounter ].stringToConsole = malloc(      strlen( events[ eventCounter ].id )
                                                     PLUS strlen( events[ eventCounter ].title )
                                                     PLUS strlen( events[ eventCounter ].date )
                                                     PLUS ( FIVE TIMES NUMBER_OF_EVENT_DATA ) );
    strcpy( events[ eventCounter ].stringToConsole, events[ eventCounter ].id ) ;
    strcat( events[ eventCounter ].stringToConsole, TABULATOR_STRING );
    strcat( events[ eventCounter ].stringToConsole, events[ eventCounter ].title );
    strcat( events[ eventCounter ].stringToConsole, TABULATOR_STRING );
    strcat( events[ eventCounter ].stringToConsole, events[ eventCounter ].date );
    strcat( events[ eventCounter ].stringToConsole, END_LINE_CHAR_PLUS_TERMINATING_CHAR_STRING );
}

VOID makeEventStringToFile( CONST INT eventCounter )
{
    events[ eventCounter ].stringToFile = malloc(      strlen( events[ eventCounter ].id )
                                                  PLUS strlen( events[ eventCounter ].title )
                                                  PLUS strlen( events[ eventCounter ].date )
                                                  PLUS ( NUMBER_OF_EVENT_DATA PLUS FIVE ) );
    strcpy( events[ eventCounter ].stringToFile, events[ eventCounter ].id ) ;
    strcat( events[ eventCounter ].stringToFile, SEPARATOR_STRING );
    strcat( events[ eventCounter ].stringToFile, events[ eventCounter ].title );
    strcat( events[ eventCounter ].stringToFile, SEPARATOR_STRING );
    strcat( events[ eventCounter ].stringToFile, events[ eventCounter ].date );
    strcat( events[ eventCounter ].stringToFile, END_LINE_CHAR_PLUS_TERMINATING_CHAR_STRING );
}

VOID goToModifyEventMenu( VOID )
{
    menu   SHOULD_BE ON_EVENT_MODIFY_MENU;
}

VOID modifyEvent( VOID )
{
    printf( MODIFICATION_OPTIONS_FOR_EVENT );
    CHAR ch SHOULD_BE EMPTY_CHAR;
    WHILE(     ch IS_NOT MOD_OPT_EVE_1
           AND ch IS_NOT MOD_OPT_EVE_2
           AND ch IS_NOT MOD_OPT_EVE_3
         )      ch SHOULD_BE getch();
    ACCORDING_TO( ch )
    {
        IF_IT_IS MOD_OPT_EVE_1  THEN    modifyIdOnEvent();     AND_STOP_HERE;
        IF_IT_IS MOD_OPT_EVE_2  THEN    modifyTitleOnEvent();  AND_STOP_HERE;
        IF_IT_IS MOD_OPT_EVE_3  THEN    modifyDateOnEvent();   AND_STOP_HERE;
        DO_NOTHING BY_DEFAULT;
    }
    saveEvents();
    refreshEvents();
    catFileToConsole( SUCCESSFUL_EVENT_MODIFICATION );
    goBackToPreviousMenu();
}

VOID modifyIdOnEvent( VOID )
{
    STRUCT Event* e SHOULD_BE &events[ cursor MINUS_ONE ];
    getEventID( e );
}

VOID modifyTitleOnEvent( VOID )
{
    STRUCT Event* e SHOULD_BE &events[ cursor MINUS_ONE ];
    getEventTitle( e );
}

VOID modifyDateOnEvent( VOID )
{
    STRUCT Event* e SHOULD_BE &events[ cursor MINUS_ONE ];
    getEventDate( e );
}

VOID saveEvents( VOID )
{
    CREATE_THIS_FILE_OBJECT file;
    openTheFile( &file, EVENTS_FILE, TO_WRITE );
    fclose( file );
    INT i;
    FOR( i SHOULD_BE ZERO; i IS_LESS_THEN numberOfEvents; ++i )
        appendNewEventToEndOfEventFile( &events[ i ] );
}

VOID goToCreateNewEventMenu( VOID )
{
    menu    SHOULD_BE  ON_NEW_EVENT_MENU;
    cursor  SHOULD_BE  STARTING_POSITION;
}

VOID createNewEvent( VOID )
{
    STRUCT Event newEvent;
    getNewEventDataFromUser( &newEvent );
    appendNewEventToEndOfEventFile( &newEvent );
    refreshEvents();
    catFileToConsole( SUCCESSFUL_EVENT_CREATE );
    goBackToPreviousMenu();
}

VOID getNewEventDataFromUser( STRUCT Event* newEvent )
{
    printf( CREATE_NEW_EVENT_STRING );
    getEventID   ( newEvent );
    getEventTitle( newEvent );
    getEventDate ( newEvent );
/*
    IF( strlen( newEvent->id ) % 8 == 0 )
    {
        STRING str = newEvent->id;
        newEvent->id = realloc( newEvent->id, strlen( newEvent->id ) + 1 );
        newEvent->id = str;
        newEvent->id[ strlen( newEvent->id ) - 1 ] = newEvent->id[ strlen( newEvent->id ) - 2 ];
        newEvent->id[ strlen( newEvent->id ) - 2 ] = ' ';
    }
*/
}

VOID getEventID( STRUCT Event* event )
{
    event->id SHOULD_BE NULL;
    printf( ASK_FOR_EVENT_ID );
    WHILE( event->id IS NULL )
        event->id  SHOULD_BE getLINE();
}

VOID getEventTitle( STRUCT Event* event )
{
    event->title SHOULD_BE NULL;
    printf( ASK_FOR_EVENT_TITLE );
    WHILE( event->title IS NULL )
        event->title  SHOULD_BE getLINE();
}

VOID getEventDate( STRUCT Event* event )
{
    event->date SHOULD_BE NULL;
    printf( ASK_FOR_EVENT_DATE );
    WHILE( event->date IS NULL )
        event->date  SHOULD_BE getLINE();
}

VOID appendNewEventToEndOfEventFile( STRUCT Event* event )
{
    CREATE_THIS_FILE_OBJECT file;
    openTheFile( &file, EVENTS_FILE, TO_APPEND );
    STRING str = malloc(   strlen( event->id )
                         + strlen( event->title )
                         + strlen( event->date )
                         + NUMBER_OF_EVENT_DATA );
    strcpy( str, event->id );
    strcat( str, SEPARATOR_STRING );
    strcat( str, event->title );
    strcat( str, SEPARATOR_STRING );
    strcat( str, event->date );
    //strcat( str, TERMINATING_STRING );
    str[ strlen( str ) ] = '\0';
    fprintf( file, STRING_PLUS_NEW_LINE, str );
    fclose( file );
}
