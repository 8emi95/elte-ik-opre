#include "FUNCTIONS.h"


VOID initSystem( VOID )
{
    input               SHOULD_BE   EMPTY_CHAR;
    menu                SHOULD_BE   MINUS_ONE;
    menuK1              SHOULD_BE   menu;
    cursor              SHOULD_BE   ZERO;
    //currentFooterInfo   SHOULD_BE   EMPTY_STRING;
    countEvents();
    countGuests();
    system( CURSOR_OFF );
    mainRender();
}

BOOL userIsNotQuitting( VOID )
{
    RETURN_TRUE_IF menu IS_NOT ON_EXIT_MENU;
}

VOID programIsRunning( VOID )
{
    menuK1 SHOULD_BE menu;
    getUserInput();
    IF( inputIsNotRelevant() ) RETURN;
    mainCalc();
    mainRender();
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
    IF(    menu IS ON_NEW_EVENT_MENU     OR  menu IS ON_NEW_GUEST_MENU
        OR menu IS ON_EVENT_MODIFY_MENU  OR  menu IS ON_GUEST_MODIFY_MENU
       )                        RETURN;
    ACCORDING_TO( input )
    {
        IF_PRESSED KEY_w AS IF_PRESSED KEY_W /*AS IF_PRESSED  UP*/  THEN moveCursor( BY_MINUS_ONE ); AND_STOP_HERE;
        IF_PRESSED KEY_s AS IF_PRESSED KEY_S /*AS IF_PRESSED DOWN*/ THEN moveCursor( BY_ONE );       AND_STOP_HERE;
        IF_PRESSED KEY_t AS IF_PRESSED KEY_T                    THEN deleteRecord();             AND_STOP_HERE;
        IF_PRESSED                  ENTER                       THEN enterWasPressed();          AND_STOP_HERE;
        IF_PRESSED                  ESC                         THEN escWasPressed();            AND_STOP_HERE;
        DO_NOTHING BY_DEFAULT;
    }
}

VOID mainRender( VOID )
{
    clearConsole();
    renderHeader();
    renderContent();
    renderFooter();
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
    IN_WHICH( menu )
    {
        IF_WE_ARE IN_EVENT_MENU THEN deleteEvent(); AND_STOP_HERE;
        IF_WE_ARE IN_GUEST_MENU THEN deleteGuest(); AND_STOP_HERE;
        DO_NOTHING BY_DEFAULT;
    }
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

/*
*/
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

