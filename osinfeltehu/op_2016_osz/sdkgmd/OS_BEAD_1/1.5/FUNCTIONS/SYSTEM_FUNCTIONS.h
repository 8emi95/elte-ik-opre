#ifndef SYSTEM_FUNCTIONS_H_INCLUDED
#define SYSTEM_FUNCTIONS_H_INCLUDED

#include "../INCLUDES.h"
#include "../MAKROS/MAKROS.h"
#include "../STRUCT/EVENT_STRUCT.h"
#include "../STRUCT/GUEST_STRUCT.h"
#include "EVENT_FUNCTIONS.h"
#include "GUEST_FUNCTIONS.h"
#include "FILE_FUNCTIONS.h"
#include "RENDER_FUNCTIONS.h"

//EXTERN HANDLE hConsole;


VOID    getUserInput( VOID );
VOID    waitForSomeInput( VOID );
BOOL    inputIsNotRelevant( VOID );
VOID    moveCursor( INT );
INT     getMaxCursorValueByMenu( VOID );

VOID    initSystem( VOID );
VOID    programIsRunning( VOID );
BOOL    userIsNotQuitting( VOID );
VOID    mainCalc( VOID );
VOID    mainRender( VOID );

VOID    deleteRecord( VOID );

VOID    enterEventsMenu( VOID );
VOID    enterGuestsMenu( VOID );
VOID    enterInfoPage( VOID );
VOID    enterExitPage( VOID );

VOID    enterWasPressed( VOID );
VOID    enterWasPressedInMainMenu( VOID );
VOID    enterWasPressedInEventMenu( VOID );
VOID    enterWasPressedInGuestMenu( VOID );
VOID    enterWasPressedInEventMenu( VOID );
VOID    enterWasPressedInGuestMenu( VOID );

VOID    escWasPressed( VOID );

VOID    goBackToMainMenu( VOID );
VOID    goBackToPreviousMenu( VOID );

//VOID    hideCursor( VOID );


#endif // SYSTEM_FUNCTIONS_H_INCLUDED
