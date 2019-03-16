#include "SYSTEM_FUNCTIONS.h"


EXTERN STRUCT Guest* guests;
EXTERN STRUCT Event* events;
//EXTERN HANDLE        hConsole;
EXTERN STRING        menuLabels[];
EXTERN INT           numberOfGuests;
EXTERN INT           numberOfEvents;
EXTERN INT           cursor;
EXTERN INT           menuK1;
EXTERN INT           menu;
EXTERN CHAR          input;


VOID initSystem( VOID )
{
//s    hConsole            SHOULD_BE   GetStdHandle( STD_OUTPUT_HANDLE );
    input               SHOULD_BE   EMPTY_CHAR;
    menu                SHOULD_BE   MINUS_ONE;
    menuK1              SHOULD_BE   menu;
    cursor              SHOULD_BE   ZERO;
    currentFooterInfo   SHOULD_BE   EMPTY_STRING;
//    hideCursor();
    countEvents();
    countGuests();
//    setColor( BASE_COLOR );
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

BOOL inputIsNotRelevant( VOID )
{
    RETURN (     input IS_NOT KEY_w AND input IS_NOT KEY_W
             AND input IS_NOT KEY_s AND input IS_NOT KEY_S
             AND input IS_NOT KEY_t AND input IS_NOT KEY_T
             AND input IS_NOT ENTER AND input IS_NOT ESC
             AND input IS_NOT UP    AND input IS_NOT DOWN
             AND input IS_NOT PGUP  AND input IS_NOT PGDN
           );
}

VOID getUserInput( VOID )
{
    input SHOULD_BE getch();
}

VOID mainCalc( VOID )
{
    IF(    menu IS ON_NEW_EVENT_MENU     OR  menu IS ON_NEW_GUEST_MENU
        OR menu IS ON_EVENT_MODIFY_MENU  OR  menu IS ON_GUEST_MODIFY_MENU
       )                        RETURN;
    ACCORDING_TO( input )
    {
        IF_PRESSED KEY_w AS IF_PRESSED KEY_W AS IF_PRESSED  UP  THEN moveCursor( BY_MINUS_ONE ); AND_STOP_HERE;
        IF_PRESSED KEY_s AS IF_PRESSED KEY_S AS IF_PRESSED DOWN THEN moveCursor( BY_ONE );       AND_STOP_HERE;
        IF_PRESSED KEY_t AS IF_PRESSED KEY_T                    THEN deleteRecord();             AND_STOP_HERE;
        IF_PRESSED                  PGUP                        THEN moveCursor( BY_MINUS_TEN ); AND_STOP_HERE;
        IF_PRESSED                  PGDN                        THEN moveCursor( BY_TEN );       AND_STOP_HERE;
        IF_PRESSED                  ENTER                       THEN enterWasPressed();          AND_STOP_HERE;
        IF_PRESSED                  ESC                         THEN escWasPressed();            AND_STOP_HERE;
        DO_NOTHING BY_DEFAULT;
    }
}

VOID mainRender( VOID )
{
    system( CLEAR );
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

VOID goBackToPreviousMenu( VOID )
{
    menu   SHOULD_BE menuK1;
    cursor SHOULD_BE STARTING_POSITION;
}

VOID goBackToMainMenu( VOID )
{
    menu SHOULD_BE IN_MAIN_MENU;
    cursor SHOULD_BE STARTING_POSITION;
}

VOID escWasPressed( VOID )
{
    IN_WHICH( menu )
    {
        IF_WE_ARE IN_MAIN_MENU THEN  enterExitPage();     AND_STOP_HERE;
        DEFAULT:                     goBackToMainMenu();  AND_STOP_HERE;
    }
}

VOID waitForSomeInput( VOID )
{
    getch();
}

/*
VOID hideCursor( VOID )
{
   CONSOLE_CURSOR_INFO info;
   info.dwSize   SHOULD_BE  HUNDRED;
   info.bVisible SHOULD_BE  FALSE;
   SetConsoleCursorInfo(hConsole, &info);
}
*/
