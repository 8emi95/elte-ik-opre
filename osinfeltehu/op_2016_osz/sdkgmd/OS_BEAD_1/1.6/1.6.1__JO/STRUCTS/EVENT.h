#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED


STRUCT Event
{
    STRING id;
    STRING title;
    STRING date;

    STRING stringToFile;
    STRING stringToConsole;
};


VOID    createNewEvent( VOID );
VOID    modifyEvent( VOID );
VOID    deleteEvent( VOID );

VOID    countEvents( VOID );
VOID    saveEvents( VOID );
VOID    refreshEvents( VOID );
VOID    memoryAllocForEvents( VOID );

VOID    readEventsFromFile( VOID );
VOID    skimEventsFromFile( FILE_POINTER );

VOID    modifyIdOnEvent( VOID );
VOID    modifyTitleOnEvent( VOID );
VOID    modifyDateOnEvent( VOID );

VOID    getNewEventDataFromUser( STRUCT Event* );
VOID    getEventID( STRUCT Event* event );
VOID    getEventTitle( STRUCT Event* );
VOID    getEventDate( STRUCT Event* );
VOID    appendNewEventToEndOfEventFile( STRUCT Event* );

VOID    makeEventStringToConsole( CONST INT );
VOID    makeEventStringToFile( CONST INT );



#endif // EVENT_H_INCLUDED
