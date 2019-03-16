#ifndef GUEST_FUNCTIONS_H_INCLUDED
#define GUEST_FUNCTIONS_H_INCLUDED

#include "../INCLUDES.h"
#include "../MAKROS/MAKROS.h"
#include "../STRUCT/EVENT_STRUCT.h"
#include "../STRUCT/GUEST_STRUCT.h"


EXTERN STRUCT  Guest* guests;
EXTERN INT     menu;
EXTERN INT     cursor;
EXTERN INT     numberOfGuests;
EXTERN INT     numberOfLinesInFile( STRING );
EXTERN VOID    openTheFile( FILE_POINTER*, STRING, MODE );
EXTERN VOID    goBackToPreviousMenu( VOID );
EXTERN VOID    catFileToConsole( STRING );
EXTERN STRING  getLINE( VOID );
EXTERN VOID    setColor( CONST COLOR );
EXTERN VOID    goBackToMainMenu( VOID );
EXTERN VOID    goBackToPreviousMenu( VOID );
EXTERN VOID    renderBackButton( VOID );
EXTERN VOID    renderGuestMenu( VOID );
EXTERN VOID    renderNewGuestButton( VOID );
EXTERN VOID    renderGuests( VOID );


VOID countGuests( VOID );
VOID deleteGuest( VOID );

VOID refreshGuests( VOID );
VOID memoryAllocForGuests( VOID );

VOID readGuestsFromFile( VOID );
VOID skimGuestsFromFile( FILE_POINTER );

VOID makeGuestStringToConsole( CONST INT );
VOID makeGuestStringToFile( CONST INT );

VOID createNewGuest( VOID );
VOID getNewGuestDataFromUser( STRUCT Guest* );
VOID getGuestNextPlace( STRUCT Guest* );
VOID getGuestName( STRUCT Guest* );
VOID getGuestEmail( STRUCT Guest* );
VOID getGuestEventId( STRUCT Guest* );
VOID getGuestDate( STRUCT Guest* );
VOID appendNewGuestToEndOfGuestFile( STRUCT Guest );
VOID decreaseGuestPlaceByOne( STRUCT Guest* );

VOID goToCreateNewGuestMenu( VOID );
VOID goToModifyGuestMenu( VOID );

VOID modifyGuest( VOID );
VOID modifyNameOnGuest( VOID );
VOID modifyEmailOnGuest( VOID );
VOID modifyEventIdOnGuest( VOID );
VOID saveGuests( VOID );


#endif // GUEST_FUNCTIONS_H_INCLUDED
