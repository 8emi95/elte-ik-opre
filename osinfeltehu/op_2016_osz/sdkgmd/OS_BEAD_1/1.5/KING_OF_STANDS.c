#include "INCLUDES.h"
#include "MAKROS/MAKROS.h"
#include "GLOBAL_VARS.h"
#include "STRUCT/GUEST_STRUCT.h"
#include "STRUCT/EVENT_STRUCT.h"
#include "FUNCTIONS/SYSTEM_FUNCTIONS.h"


EXTERN  VOID    initSystem( VOID );
EXTERN  VOID    programIsRunning( VOID );
EXTERN  BOOL    userIsNotQuitting( VOID );
EXTERN  VOID    waitForSomeInput( VOID );


INT main()
{
    initSystem();
    WHILE( userIsNotQuitting() ) programIsRunning();
    waitForSomeInput();
    PROGRAM IS_DONE;
}

