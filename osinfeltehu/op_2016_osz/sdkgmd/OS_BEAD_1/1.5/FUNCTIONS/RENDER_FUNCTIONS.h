#ifndef RENDER_FUNCTIONS_H_INCLUDED
#define RENDER_FUNCTIONS_H_INCLUDED


#include "../INCLUDES.h"
#include "../MAKROS/MAKROS.h"
#include "../STRUCT/EVENT_STRUCT.h"
#include "../STRUCT/GUEST_STRUCT.h"

EXTERN STRUCT  Event* events;
EXTERN STRUCT  Guest* guests;
EXTERN STRING  menuLabels[];
EXTERN STRING  currentFooterInfo;
EXTERN INT     numberOfEvents;
EXTERN INT     numberOfGuests;
EXTERN INT     cursor;
EXTERN INT     menu;


EXTERN VOID    catFileToConsole( STRING );
EXTERN VOID    goBackToMainMenu( VOID );
EXTERN INT     getMaxCursorValueByMenu( VOID );
EXTERN VOID    setColor( CONST COLOR );
EXTERN VOID    modifyEvent( VOID );
EXTERN VOID    createNewEvent( VOID );
EXTERN VOID    modifyGuest( VOID );
EXTERN VOID    createNewGuest( VOID );


VOID    renderMainMenu( VOID );
VOID    renderEventMenu( VOID );
VOID    renderGuestMenu( VOID );
VOID    renderInfoPage( VOID );
VOID    renderExitPage( VOID );

VOID    renderHeader( VOID );
VOID    renderContent( VOID );
VOID    renderFooter( VOID );

VOID    renderEvents( VOID );
VOID    renderGuests( VOID );

VOID    renderBackButton( VOID );
VOID    renderNewEventButton( VOID );
VOID    renderNewGuestButton( VOID );

VOID    setColor( CONST COLOR );


#endif // RENDER_FUNCTIONS_H_INCLUDED
