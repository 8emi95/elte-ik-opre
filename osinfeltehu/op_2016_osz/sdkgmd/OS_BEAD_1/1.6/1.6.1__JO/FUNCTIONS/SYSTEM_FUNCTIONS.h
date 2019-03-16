#ifndef SYSTEM_FUNCTIONS_H_INCLUDED
#define SYSTEM_FUNCTIONS_H_INCLUDED

#include "../MAKROS/MAKROS.h"
#include "../INCLUDES/STANDARD_INCLUDE.h"


//extern  CHAR  input;
//extern  INT  menu;
//extern  INT  menuK1;
//extern  INT  cursor;


VOID    initSystem( VOID );
BOOL    userIsNotQuitting( VOID );
VOID    programIsRunning( VOID );
VOID 	closeProgram( VOID );

VOID    getUserInput( VOID );
BOOL    inputIsNotRelevant( VOID );
VOID    mainCalc( VOID );
VOID    mainRender( VOID );
BOOL    commingBackFromInfoPage( VOID );

VOID    calcInfoString( VOID );
VOID    calcInfoStringInMainMenu( VOID );
VOID    calcInfoStringInEventMenu( VOID );
VOID    calcInfoStringInGuestMenu( VOID );

VOID    moveCursor( INT );
INT     getMaxCursorValueByMenu( VOID );

VOID    deleteRecord( VOID );
BOOL    userWantsToCancelDelete( VOID );

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
*/
INT     getch( VOID );
INT     getche( VOID );
STRING  getLINE( VOID );

STRING  Itoa( INT, STRING, INT );

#endif // SYSTEM_FUNCTIONS_H_INCLUDED
