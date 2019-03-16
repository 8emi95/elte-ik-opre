#include "SYSTEM_FUNCTIONS.h"

EXTERN CHAR    input;
EXTERN INT     menu;
EXTERN INT     cursor;
EXTERN INT     numberOfGuests;
EXTERN INT     numberOfEvents;
EXTERN STRING  menuLabels[];

VOID initSystem( VOID )
{

}

BOOL userIsNotQuitting( VOID )
{
    RETURN FALSE;
}

VOID programIsRunning( VOID )
{
    getUserInput();
    mainCalc();
    mainRender();
}

VOID getUserInput( VOID )
{

}

VOID mainCalc( VOID )
{
    ACCORDING_TO( input )
    {
        PRESSED KEY_w OR PRESSED KEY_W OR PRESSED  UP  THEN moveCursor( BY_MINUS_ONE ); AND_STOP_HERE;
        PRESSED KEY_s OR PRESSED KEY_S OR PRESSED DOWN THEN moveCursor( BY_ONE );       AND_STOP_HERE;
        PRESSED KEY_t OR PRESSED KEY_T                 THEN deleteRecord();             AND_STOP_HERE;
        PRESSED                  PGUP                  THEN moveCursor( BY_MINUS_TEN ); AND_STOP_HERE;
        PRESSED                  PGDN                  THEN moveCursor( BY_TEN );       AND_STOP_HERE;
        PRESSED                  ENTER                 THEN enterWasPressed();          AND_STOP_HERE;
        PRESSED                   ESC                  THEN escWasPressed();            AND_STOP_HERE;
        DO_NOTHING BY_DEFAULT;
    }
}

VOID moveCursor( INT step )
{

}

VOID deleteRecord( VOID )
{

}

VOID enterWasPressed( VOID )
{
    IN_WHICH( menu )
    {
        ENTER_WAS_PRESSED_IN IN_MAIN_MENU  THEN  enterWasPressedInMainMenu();   AND_STOP_HERE;
        ENTER_WAS_PRESSED_IN IN_GUEST_MENU THEN  enterWasPressedInGuestMenu();  AND_STOP_HERE;
        ENTER_WAS_PRESSED_IN IN_EVENT_MENU THEN  enterWasPressedInEventMenu();  AND_STOP_HERE;
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

VOID enterEventsMenu( VOID )
{

}

VOID enterGuestsMenu( VOID )
{

}

VOID enterInfoPage( VOID )
{

}

VOID enterExitPage( VOID )
{

}

VOID enterWasPressedInGuestMenu( VOID )
{
    CONST BOOL CURSOR_IS_ON_NEW_GUEST_BUTTON    SHOULD_BE_TRUE_IF ( cursor IS ON_NEW_GUEST_BUTTON     );
    CONST BOOL ON_BACK_TO_MENU_BUTTON           SHOULD_BE_TRUE_IF ( cursor IS numberOfGuests PLUS_ONE );
    CONST BOOL CURSOR_IS_ON_BACK_TO_MENU_BUTTON SHOULD_BE_TRUE_IF ( cursor IS ON_BACK_TO_MENU_BUTTON  );
    IF     ( CURSOR_IS_ON_NEW_GUEST_BUTTON )
        createNewGuest();
    ELSE IF( CURSOR_IS_ON_BACK_TO_MENU_BUTTON )
        goBackToMainMenu();
    ELSE
        modifyGuest();
}

VOID createNewGuest( VOID )
{

}

VOID goBackToMainMenu( VOID )
{

}

VOID modifyGuest( VOID )
{

}

VOID enterWasPressedInEventMenu( VOID )
{

}

VOID escWasPressed( VOID )
{

}

VOID mainRender( VOID )
{
    IN_WHICH( menu )
    {
        IF_WE_ARE IN_MAIN_MENU:  renderMainMenu(); BREAK;
        IF_WE_ARE IN_GUEST_MENU: renderEvents();   BREAK;
        IF_WE_ARE IN_EVENT_MENU: renderGuests();   BREAK;
        IF_WE_ARE ON_INFO_PAGE:  renderInfoPage(); BREAK;
        IF_WE_ARE ON_EXIT_PAGE:  renderExitPage(); BREAK;
        DEFAULT: BREAK;
    }
}

VOID renderMainMenu( VOID )
{
    int i;
    for( i = 0; i < NUMBER_OF_MENU_ITEMS; ++i )
    {
        if( cursor IS i ) setColor( SELECT_COLOR );
        printf( menuLabels[ i ] );
        setColor( BASE_COLOR );
    }
}

VOID renderEvents( VOID )
{

}

VOID renderGuests( VOID )
{

}

VOID renderInfoPage( VOID )
{

}

VOID renderExitPage( VOID )
{

}


VOID sayGoodBye( VOID )
{

}

