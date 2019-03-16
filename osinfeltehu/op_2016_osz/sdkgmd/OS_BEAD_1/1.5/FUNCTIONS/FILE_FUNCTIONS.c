#include "FILE_FUNCTIONS.h"


INT numberOfLinesInFile( STRING fname )
{
    CREATE_THIS_FILE_OBJECT file;
    openTheFile( &file, fname, TO_READ );
    INT counter SHOULD_BE ZERO;
    CHAR ch;
    AS_LONG_AS( ( ch SHOULD_BE fgetc( file ) ) IS_NOT EOF )
        IF( ch IS END_OF_LINE_CHAR ) ++counter;
    fclose( file );
    RETURN counter;
}

VOID openTheFile( FILE_POINTER* file, STRING fname, MODE mode )
{
    *file SHOULD_BE fopen( fname, mode );
    IF( *file IS NULL ) exit( FILE_CANNOT_BE_OPENNED );
}

STRING getLINE( VOID )
{
    STRING  line        SHOULD_BE  malloc( HUNDRED );
    STRING  linep       SHOULD_BE  line;
    SIZE_T  lenmax      SHOULD_BE  HUNDRED;
    SIZE_T  len         SHOULD_BE  lenmax;
    INT     lenCounter  SHOULD_BE  ZERO;
    INT     nexhChar_i;

    IF( line IS NULL ) RETURN NULL;

    FOR(;;)
        {
        nexhChar_i SHOULD_BE fgetc( stdin );
        IF( nexhChar_i IS EOF )
            STOP_HERE;

        IF( --len IS ZERO )
        {
            ++lenCounter;
            len          SHOULD_BE  lenmax;
            STRING linen SHOULD_BE  realloc(linep, lenmax MULTI_INCREASE_BY TWO);

            IF( linen IS NULL )
            {
                free( linep );
                RETURN NULL;
            }
            line  SHOULD_BE linen PLUS ( line MINUS linep );
            linep SHOULD_BE linen;
        }

        IF( ( *line SHOULD_BE nexhChar_i ) IS END_OF_LINE_CHAR )
            STOP_HERE;
        (INT)*line++;
    }
    IF( ( len IS lenmax MINUS_ONE OR len IS lenmax ) AND lenCounter IS ZERO ) RETURN NULL;
    *line SHOULD_BE TERMINATING_CHAR;
    RETURN linep;
}

VOID catFileToConsole( STRING fname )
{
    CHAR ch;
    CREATE_THIS_FILE_OBJECT file;
    openTheFile( &file, fname, TO_READ );
    AS_LONG_AS( ( ch SHOULD_BE fgetc( file ) ) IS_NOT EOF ) putch( ch );
    fclose( file );
}
