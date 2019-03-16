#ifndef RENDER_FUNCTIONS_H_INCLUDED
#define RENDER_FUNCTIONS_H_INCLUDED


VOID    renderHeader( VOID );
VOID    renderContent( VOID );
VOID    renderFooter( VOID );

VOID    renderMainMenu( VOID );
VOID    renderEventMenu( VOID );
VOID    renderGuestMenu( VOID );

VOID    renderEventSuccessIfNeeded( VOID );
VOID    renderGuestSuccessIfNeeded( VOID );

VOID    renderInfoPage( VOID );
VOID    renderExitPage( VOID );

VOID    renderNewEventButton( VOID );
VOID    renderNewGuestButton( VOID );

VOID    renderEvents( VOID );
VOID    renderGuests( VOID );

VOID    renderBackButton( VOID );




#endif // RENDER_FUNCTIONS_H_INCLUDED
