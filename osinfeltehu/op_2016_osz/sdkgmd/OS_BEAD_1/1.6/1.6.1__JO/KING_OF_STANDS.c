#include "INCLUDES/INCLUDES.h"

// 1.6.1

INT main()
{
    initSystem();
    AS_LONG_AS( userIsNotQuitting() )
        programIsRunning();
    closeProgram();
    RETURN ZERO;
}


// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ---------------------SYSTEM FUNCTIONS-----------------
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------

VOID initSystem( VOID )
{
    cursor              SHOULD_BE   STARTING_POSITION;
    currentFooterInfo   SHOULD_BE   EMPTY_STRING;
    menu                SHOULD_BE   IN_MAIN_MENU;
    input               SHOULD_BE   EMPTY_INPUT;
    menuK1              SHOULD_BE   menu;
    wasInInfo           SHOULD_BE   FALSE;
    wasInEventModify    SHOULD_BE   FALSE;
    wasInEventCreate    SHOULD_BE   FALSE;
    wasInEventDelete    SHOULD_BE   FALSE;
    wasInGuestModify    SHOULD_BE   FALSE;
    wasInGuestCreate    SHOULD_BE   FALSE;
    wasInGuestDelete    SHOULD_BE   FALSE;
    system( CURSOR_OFF );
    countEvents();
    countGuests();
    mainCalc();
    mainRender();
}

BOOL userIsNotQuitting( VOID )
{
    RETURN_TRUE_IF menu IS_NOT ON_EXIT_MENU;
}

VOID programIsRunning( VOID )
{
    getUserInput();
    IF( inputIsNotRelevant() ) RETURN;
    mainCalc();
    mainRender();
    menuK1 SHOULD_BE menu;
}

VOID getUserInput( VOID )
{
    input SHOULD_BE getch();
}

BOOL inputIsNotRelevant( VOID )
{
    RETURN (     input IS_NOT KEY_w AND input IS_NOT KEY_W
             AND input IS_NOT KEY_s AND input IS_NOT KEY_S
             AND input IS_NOT KEY_t AND input IS_NOT KEY_T
             AND input IS_NOT ENTER AND input IS_NOT ESC
           );
}

VOID mainCalc( VOID )
{
    IF( commingBackFromInfoPage() ) RETURN;
    ACCORDING_TO( input )
    {
        IF_PRESSED KEY_w AS IF_PRESSED KEY_W    THEN moveCursor( BY_MINUS_ONE ); AND_STOP_HERE;
        IF_PRESSED KEY_s AS IF_PRESSED KEY_S    THEN moveCursor( BY_ONE );       AND_STOP_HERE;
        IF_PRESSED KEY_t AS IF_PRESSED KEY_T    THEN deleteRecord();             AND_STOP_HERE;
        IF_PRESSED                  ENTER       THEN enterWasPressed();          AND_STOP_HERE;
        IF_PRESSED                  ESC         THEN escWasPressed();            AND_STOP_HERE;
        DO_NOTHING BY_DEFAULT;
    }
    calcInfoString();
}

BOOL commingBackFromInfoPage( VOID )
{
    IF( wasInInfo )
    {
        goBackToMainMenu();
        wasInInfo SHOULD_BE FALSE;
        RETURN TRUE;
    }
    RETURN FALSE;
}

VOID calcInfoString( VOID )
{
    ACCORDING_TO( menu )
    {
        IF_WE_ARE IN_MAIN_MENU   THEN  calcInfoStringInMainMenu();   AND_STOP_HERE;
        IF_WE_ARE IN_EVENT_MENU  THEN  calcInfoStringInEventMenu();  AND_STOP_HERE;
        IF_WE_ARE IN_GUEST_MENU  THEN  calcInfoStringInGuestMenu();  AND_STOP_HERE;
        DO_NOTHING BY_DEFAULT;
    }
}

VOID calcInfoStringInMainMenu( VOID )
{
    ACCORDING_TO( cursor )
    {
        IF_IT_IS ON_EVENTS_MENU  THEN  currentFooterInfo SHOULD_BE MAIN_INFO_FOR_EVENTS_STRING; AND_STOP_HERE;
        IF_IT_IS ON_GUESTS_MENU  THEN  currentFooterInfo SHOULD_BE MAIN_INFO_FOR_GUESTS_STRING; AND_STOP_HERE;
        IF_IT_IS ON_INFO_MENU    THEN  currentFooterInfo SHOULD_BE MAIN_INFO_FOR_INFO_STRING;   AND_STOP_HERE;
        IF_IT_IS ON_EXIT_MENU    THEN  currentFooterInfo SHOULD_BE MAIN_INFO_FOR_EXIT_STRING;   AND_STOP_HERE;
        DO_NOTHING BY_DEFAULT;
    }
}

VOID calcInfoStringInEventMenu( VOID )
{
    CONST INT END_POSITION SHOULD_BE numberOfEvents PLUS_ONE;
    IF     ( cursor IS STARTING_POSITION )
        currentFooterInfo SHOULD_BE EVENT_INFO_FOR_NEW_EVENT_BUTTON_STRING;
    ELSE IF( cursor IS END_POSITION )
        currentFooterInfo SHOULD_BE INFO_FOR_BACK_BUTTON_STRING;
    ELSE
        currentFooterInfo SHOULD_BE EVENT_INFO_FOR_EVENT_STRING;
}


VOID calcInfoStringInGuestMenu( VOID )
{
    CONST INT END_POSITION SHOULD_BE numberOfGuests PLUS_ONE;
    IF     ( cursor IS STARTING_POSITION )
        currentFooterInfo SHOULD_BE GUEST_INFO_FOR_NEW_GUEST_BUTTON_STRING;
    ELSE IF( cursor IS END_POSITION )
        currentFooterInfo SHOULD_BE INFO_FOR_BACK_BUTTON_STRING;
    ELSE
        currentFooterInfo SHOULD_BE GUEST_INFO_FOR_GUEST_STRING;
}

VOID moveCursor( INT step )
{
    cursor INCREASE_BY step;
    CONST INT MAX SHOULD_BE getMaxCursorValueByMenu();
    IF     ( cursor IS_GREATER_THEN MAX  ) cursor SHOULD_BE STARTING_POSITION;
    ELSE IF( cursor IS_LESS_THEN    ZERO ) cursor SHOULD_BE MAX;
}

INT getMaxCursorValueByMenu( VOID )
{
    IN_WHICH( menu )
    {
        IF_WE_ARE IN_MAIN_MENU  THEN  RETURN NUMBER_OF_MENU_ITEMS MINUS_ONE;
        IF_WE_ARE IN_EVENT_MENU THEN  RETURN numberOfEvents PLUS_ONE;
        IF_WE_ARE IN_GUEST_MENU THEN  RETURN numberOfGuests PLUS_ONE;
        DO_NOTHING BY_DEFAULT;
    }
    RETURN WELLDONE;
}

VOID deleteRecord( VOID )
{
    IF( userWantsToCancelDelete() ) RETURN;
    IN_WHICH( menu )
    {
        IF_WE_ARE IN_EVENT_MENU THEN deleteEvent(); AND_STOP_HERE;
        IF_WE_ARE IN_GUEST_MENU THEN deleteGuest(); AND_STOP_HERE;
        DO_NOTHING BY_DEFAULT;
    }
}

BOOL userWantsToCancelDelete( VOID )
{
    RETURN FALSE;
}

VOID enterWasPressed( VOID )
{
    IN_WHICH( menu )
    {
        ENTER_WAS_PRESSED_IN IN_MAIN_MENU  THEN  enterWasPressedInMainMenu();   AND_STOP_HERE;
        ENTER_WAS_PRESSED_IN IN_EVENT_MENU THEN  enterWasPressedInEventMenu();  AND_STOP_HERE;
        ENTER_WAS_PRESSED_IN IN_GUEST_MENU THEN  enterWasPressedInGuestMenu();  AND_STOP_HERE;
        DO_NOTHING BY_DEFAULT;
    }
}

VOID enterWasPressedInMainMenu( VOID )
{
    WHEN_POSITION_OF_THE( cursor )
    {
        RIGHT ON_EVENTS_MENU THEN  enterEventsMenu();  AND_STOP_HERE;
        RIGHT ON_GUESTS_MENU THEN  enterGuestsMenu();  AND_STOP_HERE;
        RIGHT ON_INFO_MENU   THEN  enterInfoPage();    AND_STOP_HERE;
        RIGHT ON_EXIT_MENU   THEN  enterExitPage();    AND_STOP_HERE;
        DO_NOTHING BY_DEFAULT;
    }
}

VOID enterWasPressedInEventMenu( VOID )
{
    CONST BOOL CURSOR_IS_ON_NEW_EVENT_BUTTON    SHOULD_BE_TRUE_IF ( cursor IS STARTING_POSITION       );
    CONST BOOL ON_BACK_TO_MENU_BUTTON           SHOULD_BE_TRUE_IF ( cursor IS numberOfEvents PLUS_ONE );
    IF     ( CURSOR_IS_ON_NEW_EVENT_BUTTON )
        goToCreateNewEventMenu();
    ELSE IF( ON_BACK_TO_MENU_BUTTON )
        goBackToMainMenu();
    ELSE
        goToModifyEventMenu();
}

VOID enterWasPressedInGuestMenu( VOID )
{
    CONST BOOL CURSOR_IS_ON_NEW_GUEST_BUTTON    SHOULD_BE_TRUE_IF ( cursor IS STARTING_POSITION       );
    CONST BOOL ON_BACK_TO_MENU_BUTTON           SHOULD_BE_TRUE_IF ( cursor IS numberOfGuests PLUS_ONE );
    IF     ( CURSOR_IS_ON_NEW_GUEST_BUTTON )
        goToCreateNewGuestMenu();
    ELSE IF( ON_BACK_TO_MENU_BUTTON )
        goBackToMainMenu();
    ELSE
        goToModifyGuestMenu();
}

VOID escWasPressed( VOID )
{
    IN_WHICH( menu )
    {
        IF_WE_ARE IN_MAIN_MENU THEN  enterExitPage();     AND_STOP_HERE;
        DEFAULT:                     goBackToMainMenu();  AND_STOP_HERE;
    }
}

VOID goToCreateNewEventMenu( VOID )
{
    menu    SHOULD_BE  ON_NEW_EVENT_MENU;
    cursor  SHOULD_BE  STARTING_POSITION;
}

VOID goToCreateNewGuestMenu( VOID )
{
    menu    SHOULD_BE  ON_NEW_GUEST_MENU;
    cursor  SHOULD_BE  STARTING_POSITION;
}

VOID goToModifyEventMenu( VOID )
{
    menu   SHOULD_BE ON_EVENT_MODIFY_MENU;
}

VOID goToModifyGuestMenu( VOID )
{
    menu   SHOULD_BE ON_GUEST_MODIFY_MENU;
}

VOID goBackToPreviousMenu( VOID )
{
    menu   SHOULD_BE menuK1;
    cursor SHOULD_BE STARTING_POSITION;
}

VOID goBackToMainMenu( VOID )
{
    menu    SHOULD_BE IN_MAIN_MENU;
    cursor  SHOULD_BE STARTING_POSITION;
}

VOID enterEventsMenu( VOID )
{
    refreshEvents();
    menu   SHOULD_BE IN_EVENT_MENU;
    cursor SHOULD_BE STARTING_POSITION;
}

VOID enterGuestsMenu( VOID )
{
    refreshGuests();
    menu SHOULD_BE IN_GUEST_MENU;
    cursor SHOULD_BE STARTING_POSITION;
}

VOID enterInfoPage( VOID )
{
    menu SHOULD_BE ON_INFO_MENU;
    cursor SHOULD_BE STARTING_POSITION;
}

VOID enterExitPage( VOID )
{
    menu   SHOULD_BE ON_EXIT_MENU;
    cursor SHOULD_BE STARTING_POSITION;
}

VOID clearConsole( VOID )
{
    system( CLEAR );
    system( CLEAR );
    system( CLEAR );
}

VOID changeColor( STRING color )
{
    printf( COLOR_CHANGING_STRING, color );
}

INT getch( VOID )
{
    STRUCT termios oldattr;
    STRUCT termios newattr;
    INT ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr SHOULD_BE oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch SHOULD_BE getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    RETURN ch;
}

INT getche( VOID )
{
    STRUCT termios oldattr;
    STRUCT termios newattr;
    INT ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr SHOULD_BE oldattr;
    newattr.c_lflag &= ~( ICANON );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch SHOULD_BE getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    RETURN ch;
}



STRING Itoa( INT value, STRING str, INT radix ) {
    STATIC CHAR dig[] SHOULD_BE
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz";
    INT n   SHOULD_BE ZERO;
    INT neg SHOULD_BE ZERO;
    UNSIGNED INT v;
    CHAR* p, *q;
    CHAR c;
    IF( radix IS TEN AND value IS_LESS_THEN ZERO ) {
        value SHOULD_BE -value;
        neg   SHOULD_BE ONE;
    }
    v SHOULD_BE value;
    DO
    {
        str[ n++ ] SHOULD_BE dig[ v % radix ];
        v MULTI_DECREASE_BY radix;
    } AS_LONG_AS ( v );
    IF( neg )
        str[ n++ ] SHOULD_BE '-';
    str[ n ] SHOULD_BE TERMINATING_CHAR;
    FOR( p SHOULD_BE str, q SHOULD_BE p PLUS n/2; p IS_NOT_EQUAL_TO q; ++p, --q )
         c SHOULD_BE *p, *p SHOULD_BE *q, *q SHOULD_BE c;
    RETURN str;
}

STRING getLINE( VOID )
{
    STRING  line        SHOULD_BE  malloc( HUNDRED );
    STRING  linep       SHOULD_BE  line;
    SIZE_T  lenmax      SHOULD_BE  HUNDRED;
    SIZE_T  len         SHOULD_BE  lenmax;
    INT     lenCounter  SHOULD_BE  ZERO;
    INT     nexhChar_i;

    IF( line IS NULL ) RETURN NULL;

    FOR(;;)
        {
        nexhChar_i SHOULD_BE fgetc( stdin );
        IF( nexhChar_i IS EOF )
            STOP_HERE;

        IF( --len IS ZERO )
        {
            ++lenCounter;
            len          SHOULD_BE  lenmax;
            STRING linen SHOULD_BE  realloc(linep, lenmax MULTI_INCREASE_BY TWO);

            IF( linen IS NULL )
            {
                free( linep );
                RETURN NULL;
            }
            line  SHOULD_BE linen PLUS ( line MINUS linep );
            linep SHOULD_BE linen;
        }

        IF( ( *line SHOULD_BE nexhChar_i ) IS END_OF_LINE_CHAR )
            STOP_HERE;
        (INT)*line++;
    }
    IF( ( len IS lenmax MINUS_ONE OR len IS lenmax ) AND lenCounter IS ZERO ) RETURN NULL;
    *line SHOULD_BE TERMINATING_CHAR;
    RETURN linep;
}

VOID closeProgram( VOID )
{
    getUserInput();
    changeColor( NORMAL );
    clearConsole();
}




// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ---------------------RENDER FUNCTIONS-----------------
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------

VOID mainRender( VOID )
{
    clearConsole();
    renderHeader();
    renderContent();
    renderFooter();
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

VOID renderFooter( VOID )
{
    changeColor( CYAN );
    printf( currentFooterInfo );
    changeColor( RED );
    system( FOOTER_FILE );
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
    renderEventSuccessIfNeeded();
}

VOID renderEventSuccessIfNeeded( VOID )
{
    STRING success SHOULD_BE EMPTY_STRING;
    IF     ( wasInEventModify )
        success SHOULD_BE SUCCESSFUL_EVENT_MODIFICATION_STRING;
    ELSE IF( wasInEventCreate )
        success SHOULD_BE SUCCESSFUL_EVENT_CREATE_STRING;
    ELSE IF( wasInEventDelete )
        success SHOULD_BE SUCCESSFUL_EVENT_DELETE_STRING;

    changeColor( RED );
    printf( SUCCESSFUL_EVENT_MODIFICATION_STRING );
    wasInEventModify SHOULD_BE FALSE;
}

VOID renderGuestSuccessIfNeeded( VOID )
{
    IF     ( wasInGuestModify )
    {
        changeColor( RED );
        printf( SUCCESSFUL_GUEST_MODIFICATION_STRING );
        wasInGuestModify SHOULD_BE FALSE;
    }
    ELSE IF( wasInGuestCreate )
    {
        changeColor( RED );
        printf( SUCCESSFUL_GUEST_CREATE_STRING );
        wasInGuestCreate SHOULD_BE FALSE;
    }
    ELSE IF( wasInGuestDelete )
    {
        changeColor( RED );
        printf( SUCCESSFUL_GUEST_DELETE_STRING );
        wasInGuestDelete SHOULD_BE FALSE;
    }
}

VOID renderGuestMenu( VOID )
{
    renderNewGuestButton();
    renderGuests();
    renderBackButton();
    renderGuestSuccessIfNeeded();
}

VOID renderInfoPage( VOID )
{
    changeColor( CYAN );
    system( INFO_PAGE_FILE_1 );
    getch();
    system( INFO_PAGE_FILE_2 );
    getch();
    system( INFO_PAGE_FILE_3 );
    wasInInfo SHOULD_BE TRUE;
}

VOID renderExitPage( VOID )
{
    clearConsole();
    changeColor( RED );
    system( EXIT_PAGE_FILE );
    changeColor( NORMAL );
}

VOID renderNewEventButton( VOID )
{
    IF( cursor IS_ON STARTING_POSITION )    changeColor( YELLOW );
    ELSE                                    changeColor( GREEN );
    printf( NEW_EVENT_BUTTON_STRING );
    changeColor( NORMAL );
    printf( MENU_SEPARATOR_STRING );
}

VOID renderNewGuestButton( VOID )
{
    IF( cursor IS_ON STARTING_POSITION )    changeColor( YELLOW );
    ELSE                                    changeColor( GREEN );
    printf( NEW_GUEST_BUTTON_STRING );
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

VOID renderBackButton( VOID )
{
    INT MAX SHOULD_BE getMaxCursorValueByMenu();
    changeColor( NORMAL );
    printf( MENU_SEPARATOR_STRING );
    IF( cursor IS_EQUAL_TO MAX )    changeColor( YELLOW );
    ELSE                            changeColor( GREEN );
    printf( BACK_BUTTON_STRING );
}



// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ---------------------FILE_IO FUNCTIONS----------------
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------


VOID openTheFile( FILE_POINTER* file, STRING fname, MODE mode )
{
    *file SHOULD_BE fopen( fname, mode );
    IF( *file IS NULL ) exit( FILE_CANNOT_BE_OPENNED );
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


// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ---------------------EVENT FUNCTIONS----------------
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------

VOID createNewEvent( VOID )
{
    clearConsole();
    STRUCT Event newEvent;
    getNewEventDataFromUser( &newEvent );
    appendNewEventToEndOfEventFile( &newEvent );
    refreshEvents();
    wasInEventCreate SHOULD_BE TRUE;
    goBackToPreviousMenu();
    clearConsole();
    renderHeader();
    renderEventMenu();
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
    wasInEventModify SHOULD_BE TRUE;
    goBackToPreviousMenu();
    clearConsole();
    renderHeader();
    renderEventMenu();
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
    wasInEventDelete SHOULD_BE TRUE;
}

VOID countEvents( VOID )
{
    numberOfEvents SHOULD_BE numberOfLinesInFile( EVENTS_FILE );
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

// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ---------------------GUEST FUNCTIONS------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------


VOID createNewGuest( VOID )
{
    STRUCT Guest newGuest;
    getNewGuestDataFromUser( &newGuest );
    appendNewGuestToEndOfGuestFile( newGuest );
    refreshGuests();
    wasInGuestCreate SHOULD_BE TRUE;
    goBackToPreviousMenu();
    clearConsole();
    renderHeader();
    renderGuestMenu();
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
    wasInGuestModify SHOULD_BE TRUE;
    goBackToPreviousMenu();
    clearConsole();
    renderHeader();
    renderGuestMenu();
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
    wasInGuestDelete SHOULD_BE TRUE;
}

VOID countGuests( VOID )
{
    numberOfGuests SHOULD_BE numberOfLinesInFile( GUESTS_FILE );
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

VOID decreaseGuestPlaceByOne( STRUCT Guest* guest )
{
    STRING place  SHOULD_BE  malloc( BYTE );
    CHAR buffer[ BYTE ];
    place         SHOULD_BE  Itoa( ( atoi( guest->place) MINUS_ONE ), buffer, TENTH_NUMBER_SYSTEM );
    free( guest->place );
    guest->place  SHOULD_BE malloc( strlen( place ) );
    strcpy( guest->place, place );
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



// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// -----------------------END OF STORY-------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
