#ifndef SYSTEM_FUNCTIONS_H_INCLUDED
#define SYSTEM_FUNCTIONS_H_INCLUDED

#include "../MAKROS/MAKROS.h"
//#include "../INCLUDES/STANDARD_INCLUDE.h"
//#include <conio.h>


extern  CHAR    input;
extern  INT     menu;
extern  INT     menuK1;
extern  INT     cursor;
extern  INT     numberOfEvents;
extern  INT     numberOfGuests;

extern  VOID    countEvents( VOID );
extern  VOID    countGuests( VOID );
extern  VOID    deleteEvent( VOID );
extern  VOID    deleteGuest( VOID );
extern  VOID    refreshEvents( VOID );
extern  VOID    refreshGuests( VOID );


VOID    initSystem( VOID );
BOOL    userIsNotQuitting( VOID );
VOID    programIsRunning( VOID );

VOID    getUserInput( VOID );
BOOL    inputIsNotRelevant( VOID );

VOID    mainCalc( VOID );
VOID    mainRender( VOID );

VOID    moveCursor( INT );
INT     getMaxCursorValueByMenu( VOID );

VOID    deleteRecord( VOID );

VOID    enterWasPressed( VOID );
VOID    enterWasPressedInMainMenu( VOID );
VOID    enterWasPressedInEventMenu( VOID );
VOID    enterWasPressedInGuestMenu( VOID );
VOID    escWasPressed( VOID );

VOID    goToCreateNewEventMenu( VOID );
VOID    goToCreateNewGuestMenu( VOID );
VOID    goToModifyEventMenu( VOID );
VOID    goToModifyGuestMenu( VOID );
VOID    goBackToPreviousMenu( VOID );
VOID    goBackToMainMenu( VOID );

VOID    enterEventsMenu( VOID );
VOID    enterGuestsMenu( VOID );
VOID    enterInfoPage( VOID );
VOID    enterExitPage( VOID );

VOID    clearConsole( VOID );
VOID    changeColor( STRING );

/*
INT     getch( VOID );
INT     getche( VOID );
*/
STRING  getLINE( VOID );

STRING  Itoa( INT, STRING, INT );

#endif // SYSTEM_FUNCTIONS_H_INCLUDED
