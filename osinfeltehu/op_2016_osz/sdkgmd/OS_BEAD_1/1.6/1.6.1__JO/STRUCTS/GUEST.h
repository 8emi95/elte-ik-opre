#ifndef GUEST_H_INCLUDED
#define GUEST_H_INCLUDED


STRUCT Guest
{
    STRING place;
    STRING name;
    STRING email;
    STRING event_id;
    STRING date;

    STRING stringToFile;
    STRING stringToConsole;
};


VOID    createNewGuest( VOID );
VOID    modifyGuest( VOID );
VOID    deleteGuest( VOID );

VOID    countGuests( VOID );
VOID    saveGuests( VOID );
VOID    refreshGuests( VOID );
VOID    memoryAllocForGuests( VOID );

VOID    readGuestsFromFile( VOID );
VOID    skimGuestsFromFile( FILE_POINTER );

VOID    modifyNameOnGuest( VOID );
VOID    modifyEmailOnGuest( VOID );
VOID    modifyEventIdOnGuest( VOID );
VOID    decreaseGuestPlaceByOne( STRUCT Guest* );

VOID    getNewGuestDataFromUser( STRUCT Guest* );
VOID    getGuestNextPlace( STRUCT Guest* );
VOID    getGuestName( STRUCT Guest* );
VOID    getGuestEmail( STRUCT Guest* );
VOID    getGuestEventId( STRUCT Guest* );
VOID    getGuestDate( STRUCT Guest* );
VOID    appendNewGuestToEndOfGuestFile( STRUCT Guest );

VOID    makeGuestStringToConsole( CONST INT );
VOID    makeGuestStringToFile( CONST INT );



#endif // GUEST_H_INCLUDED
