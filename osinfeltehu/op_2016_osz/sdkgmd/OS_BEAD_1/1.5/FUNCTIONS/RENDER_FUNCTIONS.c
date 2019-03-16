#include "RENDER_FUNCTIONS.h"



VOID renderHeader( VOID )
{
    catFileToConsole( HEADER_FILE );
}

VOID renderContent( VOID )
{
    IN_WHICH( menu )
    {
        IF_WE_ARE IN_MAIN_MENU:          renderMainMenu();  BREAK;
        IF_WE_ARE IN_GUEST_MENU:         renderGuestMenu(); BREAK;
        IF_WE_ARE IN_EVENT_MENU:         renderEventMenu(); BREAK;
        IF_WE_ARE ON_INFO_PAGE:          renderInfoPage();  BREAK;
        IF_WE_ARE ON_EXIT_PAGE:          renderExitPage();  BREAK;
        IF_WE_ARE ON_NEW_EVENT_MENU:     createNewEvent();  BREAK;
        IF_WE_ARE ON_NEW_GUEST_MENU:     createNewGuest();  BREAK;
        IF_WE_ARE ON_EVENT_MODIFY_MENU:  modifyEvent();     BREAK;
        IF_WE_ARE ON_GUEST_MODIFY_MENU:  modifyGuest();     BREAK;
        DEFAULT: BREAK;
    }
}

VOID renderFooter( VOID )
{
    printf( currentFooterInfo );
    catFileToConsole( FOOTER_FILE );
}

VOID renderMainMenu( VOID )
{
    INT i;
    FOR( i SHOULD_BE ZERO; i IS_LESS_THEN NUMBER_OF_MENU_ITEMS; i INCREASE_BY ONE )
    {
        //IF( cursor IS i ) setColor( SELECT_COLOR );
        printf( menuLabels[ i ] );
        //setColor( BASE_COLOR );
    }
}

VOID renderBackButton( VOID )
{
    INT MAX SHOULD_BE getMaxCursorValueByMenu();
    printf( MENU_SEPARATOR_STRING );
    //IF( cursor IS_EQUAL_TO MAX ) setColor( SELECT_COLOR );
    printf( BACK_BUTTON_STRING );
    //setColor( BASE_COLOR );
}

VOID renderInfoPage( VOID )
{
    catFileToConsole( INFO_PAGE_FILE );
    goBackToMainMenu();
}

VOID renderExitPage( VOID )
{
    catFileToConsole( EXIT_PAGE_FILE );
}

VOID renderEventMenu( VOID )
{
    renderNewEventButton();
    renderEvents();
    renderBackButton();
}

VOID renderNewEventButton( VOID )
{
    //IF( cursor IS_ON STARTING_POSITION ) setColor( SELECT_COLOR );
    printf( NEW_EVENT_BUTTON_STRING );
    //setColor( BASE_COLOR );
    printf( MENU_SEPARATOR_STRING );
}

VOID renderEvents( VOID )
{
    INT i;
    FOR( i SHOULD_BE ZERO; i IS_LESS_THEN numberOfEvents; ++i )
    {
        //IF( cursor IS_EQUAL_TO i PLUS_ONE ) setColor( SELECT_COLOR );
        printf( events[ i ].stringToConsole );
        //setColor( BASE_COLOR );
    }
}

VOID renderGuestMenu( VOID )
{
    renderNewGuestButton();
    renderGuests();
    renderBackButton();
}

VOID renderNewGuestButton( VOID )
{
    //IF( cursor IS_ON STARTING_POSITION ) setColor( SELECT_COLOR );
    printf( NEW_GUEST_BUTTON_STRING );
    //setColor( BASE_COLOR );
    printf( MENU_SEPARATOR_STRING );
}

VOID renderGuests( VOID )
{
    INT i;
    FOR( i SHOULD_BE ZERO; i IS_LESS_THEN numberOfGuests; ++i )
    {
        //IF( cursor IS_EQUAL_TO i PLUS_ONE ) setColor( SELECT_COLOR );
        printf( guests[ i ].stringToConsole );
        //setColor( BASE_COLOR );
    }
}
/*
VOID setColor( CONST COLOR color )
{
    SetConsoleTextAttribute( hConsole, color );
}
*/
