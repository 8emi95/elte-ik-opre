#ifndef EVENT_FUNCTIONS_H_INCLUDED
#define EVENT_FUNCTIONS_H_INCLUDED

#include "../INCLUDES.h"
#include "../MAKROS/MAKROS.h"
#include "../STRUCT/EVENT_STRUCT.h"
#include "../STRUCT/GUEST_STRUCT.h"


EXTERN STRUCT  Event* events;
EXTERN INT     menu;
EXTERN INT     cursor;
EXTERN INT     numberOfEvents;
EXTERN INT     numberOfLinesInFile( STRING );
EXTERN VOID    openTheFile( FILE_POINTER*, STRING, MODE );
EXTERN VOID    catFileToConsole( STRING );
EXTERN STRING  getLINE( VOID );
EXTERN VOID    setColor( CONST COLOR );
EXTERN VOID    goBackToMainMenu( VOID );
EXTERN VOID    goBackToPreviousMenu( VOID );
EXTERN VOID    renderBackButton( VOID );
EXTERN VOID    renderEventMenu( VOID );
EXTERN VOID    renderNewEventButton( VOID );
EXTERN VOID    renderEvents( VOID );



VOID countEvents( VOID );
VOID deleteEvent( VOID );

VOID refreshEvents( VOID );
VOID memoryAllocForEvents( VOID );

VOID readEventsFromFile( VOID );
VOID skimEventsFromFile( FILE_POINTER file );

VOID makeEventStringToConsole( CONST INT );
VOID makeEventStringToFile( CONST INT );

VOID createNewEvent( VOID );
VOID getNewEventDataFromUser( STRUCT Event* );
VOID getEventID( STRUCT Event* );
VOID getEventTitle( STRUCT Event* );
VOID getEventDate( STRUCT Event* );
VOID appendNewEventToEndOfEventFile( STRUCT Event* );

VOID goToCreateNewEventMenu( VOID );
VOID goToModifyEventMenu( VOID );

VOID modifyEvent( VOID );
VOID modifyIdOnEvent( VOID );
VOID modifyTitleOnEvent( VOID );
VOID modifyDateOnEvent( VOID );
VOID saveEvents( VOID );



#endif // EVENT_FUNCTIONS_H_INCLUDED
