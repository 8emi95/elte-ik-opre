#ifndef FILE_FUNCTIONS_H_INCLUDED
#define FILE_FUNCTIONS_H_INCLUDED

#include "../INCLUDES.h"
#include "../MAKROS/MAKROS.h"
#include "../STRUCT/EVENT_STRUCT.h"
#include "../STRUCT/GUEST_STRUCT.h"


VOID    openTheFile( FILE_POINTER*, STRING, MODE );
INT     numberOfLinesInFile( STRING );
VOID    catFileToConsole( STRING );
STRING  getLINE( VOID );



#endif // FILE_FUNCTIONS_H_INCLUDED
