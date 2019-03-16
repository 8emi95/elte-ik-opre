#include "INCLUDES/INCLUDES.h"

// 1.6.2

extern  VOID    initSystem( VOID );
extern  BOOL    userIsNotQuitting( VOID );
extern  VOID    programIsRunning( VOID );
extern  VOID    changeColor( STRING );


INT main()
{
    initSystem();
    AS_LONG_AS( userIsNotQuitting() )
        programIsRunning();
        changeColor( NORMAL );
    RETURN ZERO;
}

VOID countEvents( VOID )
{
    numberOfEvents SHOULD_BE numberOfLinesInFile( EVENTS_FILE );
}

VOID countGuests( VOID )
{
    numberOfGuests SHOULD_BE numberOfLinesInFile( GUESTS_FILE );
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
    place         SHOULD_BE  Itoa( ( atoi( guest->place) MINUS_ONE ), buffer, TENTH_NUMBER_SYSTEM );
    free( guest->place );
    guest->place  SHOULD_BE malloc( strlen( place ) );
    strcpy( guest->place, place );
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
    strcpy( events[ eventCounter ].stringToConsole, events[ eventCounter ].id );
    strcat( events[ eventCounter ].stringToConsole, TABULATOR_STRING );
    strcat( events[ eventCounter ].stringToConsole, events[ eventCounter ].title );
    strcat( events[ eventCounter ].stringToConsole, TABULATOR_STRING );
    strcat( events[ eventCounter ].stringToConsole, events[ eventCounter ].date );
    strcat( events[ eventCounter ].stringToConsole, TERMINATING_STRING /*END_LINE_CHAR_PLUS_TERMINATING_CHAR_STRING*/ );
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

VOID createNewGuest( VOID )
{
    STRUCT Guest newGuest;
    getNewGuestDataFromUser( &newGuest );
    appendNewGuestToEndOfGuestFile( newGuest );
    refreshGuests();
    goBackToPreviousMenu();
    changeColor( GREEN );
    system( SUCCESSFUL_GUEST_CREATE );
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
    strcat( guests[ guestCounter ].stringToConsole, TERMINATING_STRING /*END_LINE_CHAR_PLUS_TERMINATING_CHAR_STRING*/ );
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

VOID getNewGuestDataFromUser( STRUCT Guest* newGuest )
{
    changeColor( RED );
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
    nextPlace         SHOULD_BE  Itoa( ( numberOfGuests PLUS_ONE ), buffer, TENTH_NUMBER_SYSTEM );
    guest->place = malloc( strlen( nextPlace ) );
    strcpy( guest->place, nextPlace );
}

VOID getGuestName( STRUCT Guest* guest )
{
    guest->name SHOULD_BE NULL;
    changeColor( CYAN );
    printf( ASK_FOR_GUEST_NAME );
    changeColor( NORMAL );
    AS_LONG_AS( guest->name IS NULL )
        guest->name  SHOULD_BE getLINE();
}

VOID getGuestEmail( STRUCT Guest* guest )
{
    guest->email SHOULD_BE NULL;
    changeColor( CYAN );
    printf( ASK_FOR_GUEST_EMAIL );
    changeColor( NORMAL );
    AS_LONG_AS( guest->email IS NULL )
        guest->email  SHOULD_BE getLINE();
}

VOID getGuestEventId( STRUCT Guest* guest )
{
    guest->event_id SHOULD_BE NULL;
    changeColor( CYAN );
    printf( ASK_FOR_EVENT_ID );
    changeColor( NORMAL );
    AS_LONG_AS( guest->event_id IS NULL )
        guest->event_id  SHOULD_BE getLINE();
}

VOID getGuestDate( STRUCT Guest* guest )
{
    TIME_T* rawtime SHOULD_BE (TIME_T*)malloc( SIZEOF( TIME_T ) );
    time( rawtime );
    STRING str SHOULD_BE malloc( BYTE * SIZEOF( CHAR ) );
    sprintf( str, LONG_DECIMAL_STRING, (LONG)*rawtime );
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

VOID renderHeader( VOID )
{
    changeColor( RED );
    system( CAT_HEADER );
}

VOID renderContent( VOID )
{
    IN_WHICH( menu )
    {
        IF_WE_ARE IN_MAIN_MENU:          renderMainMenu();  AND_STOP_HERE;
        IF_WE_ARE IN_EVENT_MENU:         renderEventMenu(); AND_STOP_HERE;
        IF_WE_ARE IN_GUEST_MENU:         renderGuestMenu(); AND_STOP_HERE;
        IF_WE_ARE ON_INFO_PAGE:          renderInfoPage();  AND_STOP_HERE;
        IF_WE_ARE ON_EXIT_PAGE:          renderExitPage();  AND_STOP_HERE;
        IF_WE_ARE ON_NEW_EVENT_MENU:     createNewEvent();  AND_STOP_HERE;
        IF_WE_ARE ON_NEW_GUEST_MENU:     createNewGuest();  AND_STOP_HERE;
        IF_WE_ARE ON_EVENT_MODIFY_MENU:  modifyEvent();     AND_STOP_HERE;
        IF_WE_ARE ON_GUEST_MODIFY_MENU:  modifyGuest();     AND_STOP_HERE;
        DO_NOTHING BY_DEFAULT;
    }
}

VOID renderMainMenu( VOID )
{
    INT i;
    FOR( i SHOULD_BE ZERO; i IS_LESS_THEN NUMBER_OF_MENU_ITEMS; i INCREASE_BY ONE )
    {
        IF( cursor IS i )   changeColor( YELLOW );
        ELSE                changeColor( GREEN );
        printf( menuLabels[ i ] );
    }
}

VOID renderEventMenu( VOID )
{
    renderNewEventButton();
    renderEvents();
    renderBackButton();
}

VOID renderNewEventButton( VOID )
{
    IF( cursor IS_ON STARTING_POSITION )    changeColor( YELLOW );
    ELSE                                    changeColor( GREEN );
    printf( NEW_EVENT_BUTTON_STRING );
    changeColor( NORMAL );
    printf( MENU_SEPARATOR_STRING );
}

VOID renderEvents( VOID )
{
    INT i;
    FOR( i SHOULD_BE ZERO; i IS_LESS_THEN numberOfEvents; ++i )
    {
        IF( cursor IS_EQUAL_TO i PLUS_ONE )     changeColor( YELLOW );
        ELSE                                    changeColor( GREEN );
        printf( events[ i ].stringToConsole );
    }
}

VOID renderBackButton( VOID )
{
    INT MAX SHOULD_BE getMaxCursorValueByMenu();
    changeColor( NORMAL );
    printf( MENU_SEPARATOR_STRING );
    IF( cursor IS_EQUAL_TO MAX )    changeColor( YELLOW );
    ELSE                            changeColor( GREEN );
    printf( BACK_BUTTON_STRING );
}

VOID renderGuestMenu( VOID )
{
    renderNewGuestButton();
    renderGuests();
    renderBackButton();
}

VOID renderNewGuestButton( VOID )
{
    IF( cursor IS_ON STARTING_POSITION )    changeColor( YELLOW );
    ELSE                                    changeColor( GREEN );
    printf( NEW_GUEST_BUTTON_STRING );
    changeColor( NORMAL );
    printf( MENU_SEPARATOR_STRING );
}

VOID renderGuests( VOID )
{
    INT i;
    FOR( i SHOULD_BE ZERO; i IS_LESS_THEN numberOfGuests; ++i )
    {
        IF( cursor IS_EQUAL_TO i PLUS_ONE )     changeColor( YELLOW );
        ELSE                                    changeColor( GREEN );
        printf( guests[ i ].stringToConsole );
    }
}

VOID renderInfoPage( VOID )
{
    changeColor( CYAN );
    system( INFO_PAGE_FILE );
    goBackToMainMenu();
}

VOID renderExitPage( VOID )
{
    clearConsole();
    changeColor( RED );
    system( EXIT_PAGE_FILE );
    changeColor( NORMAL );
}





VOID modifyEvent( VOID )
{
    changeColor( MAGENTA );
    printf( MODIFICATION_OPTIONS_FOR_EVENT );
    CHAR ch SHOULD_BE EMPTY_CHAR;
    AS_LONG_AS(     ch IS_NOT MOD_OPT_EVE_1
                AND ch IS_NOT MOD_OPT_EVE_2
                AND ch IS_NOT MOD_OPT_EVE_3
              )             ch SHOULD_BE getch();
    ACCORDING_TO( ch )
    {
        IF_IT_IS MOD_OPT_EVE_1  THEN    modifyIdOnEvent();     AND_STOP_HERE;
        IF_IT_IS MOD_OPT_EVE_2  THEN    modifyTitleOnEvent();  AND_STOP_HERE;
        IF_IT_IS MOD_OPT_EVE_3  THEN    modifyDateOnEvent();   AND_STOP_HERE;
        DO_NOTHING BY_DEFAULT;
    }
    saveEvents();
    refreshEvents();
    system( SUCCESSFUL_EVENT_MODIFICATION );
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

VOID modifyGuest( VOID )
{
    changeColor( MAGENTA );
    printf( MODIFICATION_OPTIONS_FOR_GUEST );
    CHAR ch SHOULD_BE EMPTY_CHAR;
    AS_LONG_AS(     ch IS_NOT MOD_OPT_GUE_1
                AND ch IS_NOT MOD_OPT_GUE_2
                AND ch IS_NOT MOD_OPT_GUE_3
         )          ch SHOULD_BE getch();
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
    system( SUCCESSFUL_GUEST_MODIFICATION );
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

VOID renderFooter( VOID )
{
    changeColor( RED );
    system( FOOTER_FILE );
}

INT numberOfLinesInFile( STRING fname )
{
    CREATE_THIS_FILE_OBJECT file;
    openTheFile( &file, fname, TO_READ );
    INT counter SHOULD_BE ZERO;
    CHAR ch;
    AS_LONG_AS( ( ch SHOULD_BE fgetc( file ) ) IS_NOT EOF )
        IF( ch IS END_OF_LINE_CHAR ) ++counter;
    fclose( file );
    RETURN counter;
}

VOID openTheFile( FILE_POINTER* file, STRING fname, MODE mode )
{
    *file SHOULD_BE fopen( fname, mode );
    IF( *file IS NULL ) exit( FILE_CANNOT_BE_OPENNED );
}



VOID createNewEvent( VOID )
{
    clearConsole();
    STRUCT Event newEvent;
    getNewEventDataFromUser( &newEvent );
    appendNewEventToEndOfEventFile( &newEvent );
    refreshEvents();
    system( SUCCESSFUL_EVENT_CREATE );
    goBackToPreviousMenu();
}

VOID getNewEventDataFromUser( STRUCT Event* newEvent )
{
    changeColor( RED );
    printf( CREATE_NEW_EVENT_STRING );
    getEventID   ( newEvent );
    getEventTitle( newEvent );
    getEventDate ( newEvent );
}

VOID getEventID( STRUCT Event* event )
{
    event->id SHOULD_BE NULL;
    changeColor( CYAN );
    printf( ASK_FOR_EVENT_ID );
    changeColor( NORMAL );
    AS_LONG_AS( event->id IS NULL )
        event->id  SHOULD_BE getLINE();
}

VOID getEventTitle( STRUCT Event* event )
{
    event->title SHOULD_BE NULL;
    changeColor( CYAN );
    printf( ASK_FOR_EVENT_TITLE );
    changeColor( NORMAL );
    AS_LONG_AS( event->title IS NULL )
        event->title  SHOULD_BE getLINE();
}

VOID getEventDate( STRUCT Event* event )
{
    event->date SHOULD_BE NULL;
    changeColor( CYAN );
    printf( ASK_FOR_EVENT_DATE );
    changeColor( NORMAL );
    AS_LONG_AS( event->date IS NULL )
        event->date  SHOULD_BE getLINE();
}

VOID appendNewEventToEndOfEventFile( STRUCT Event* event )
{
    CREATE_THIS_FILE_OBJECT file;
    openTheFile( &file, EVENTS_FILE, TO_APPEND );
    STRING str SHOULD_BE malloc(   strlen( event->id )
                                 + strlen( event->title )
                                 + strlen( event->date )
                                 + NUMBER_OF_EVENT_DATA );
    strcpy( str, event->id );
    strcat( str, SEPARATOR_STRING );
    strcat( str, event->title );
    strcat( str, SEPARATOR_STRING );
    strcat( str, event->date );
    strcat( str, TERMINATING_STRING );
    str[ strlen( str ) ] SHOULD_BE TERMINATING_CHAR;
    fprintf( file, STRING_PLUS_NEW_LINE, str );
    fclose( file );
}












/*
*/
