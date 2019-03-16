#include "FUNCTIONS.h"

void initSystem( void )
{
    initConsoleUI();
    initWelcomePage();
    initInput();
}

void initConsoleUI( void )
{
    initConsole();
    initConsoleCursor();
    initConsoleColor();
}

void initConsole( void )
{
    hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
}

void initConsoleCursor( void )
{
    CONSOLE_CURSOR_INFO info;
    info.dwSize   = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo( hConsole, &info );
}

void initConsoleColor( void )
{
    SetConsoleTextAttribute( hConsole, BASECOLOR );
    clearConsole();
}

void clearConsole( void ) { system( CLEAR ); }
void initWelcomePage( void ) { printf( WELCOMESTRING ); }
void initInput( void ) { input = ' '; }

int userIsNotQuitting( void )
{
    return ( ( (int)input != ESC && menu != EXIT )
    ? TRUE
    : FALSE
           );
}

void runProgram( void )
{
    getUserInput();
    if( inputIsNotRelevant() ) return;
    doControlCycleOn();
}

void getUserInput( void )
{
    input = getch();
}

int inputIsNotRelevant( void )
{
    return ( (    ( input != ESC   && input != ENTER && input != KEY_W )
               && ( input != KEY_S && input != KEY_T && input != UP )
               && ( input != DOWN  && input != PGUP  && input != PGDN )
             )
    ? TRUE
    : FALSE
           );
}

void doControlCycleOn( void )
{
    calc();
    render();
    postCalc();
}

void calc( void )
{
    switch( menu )
    {
        case INMAINMENU: calcInMenu();    break;
        case INLISTING:  calcInListing(); break;
        default: break;
    }
}

void calcInMenu( void )
{
    switch( (int)input )
    {
        case KEY_W: case UP:     tryMoveCursor( 1, NUMOFMENUOPTIONS, -1 );   break;
        case KEY_S: case DOWN:   tryMoveCursor( NUMOFMENUOPTIONS, 1, 1 );    break;
        case ENTER:              enterInMenu();                             break;
        default: break;
    }
}

void tryMoveCursor( const int min, const int max, const int step )
{
    cursor = ( ( cursor == min )
    ? max
    : cursor + step
             );
}

void enterInMenu( void )
{
    menu   = cursor;
    cursor = STARTPOSFORLISTING;
    initInput();
    calc();
}

void calcInListing( void )
{
    handleGuestsLoading();
    switch( (int)input )
    {
        case KEY_W: case UP:   tryMoveCursor( 0, numberOfGuests + 1, -1 ); break;
        case KEY_S: case DOWN: tryMoveCursor( numberOfGuests + 1, 0,  1 ); break;
        case ENTER:            checkCursorToEnter();                       break;
        case KEY_T:            deleteGuest();                              break;
        case PGUP:             tryPgUpWithCursor();                        break;
        case PGDN:             tryPgDnWithCursor();                        break;
        default: break;
    }
}

void handleGuestsLoading( void )
{
    if( menu_K1 == menu ) return;
    loadGuestsList();
    menu_K1 = menu;
}

void loadGuestsList( void )
{
    refreshGuests();
    reloadGuests();
}

void refreshGuests( void )
{
    free( guests );
    numberOfGuests = countGuestsInFile();
    guests         = (struct Guest *) malloc( numberOfGuests * sizeof( Guest ) );
}

int countGuestsInFile( void )
{
    FILE *file = fopen( FNAME, TOREAD );
    if( file ) return countLines( file );
    return ERR;
}

int countLines( FILE* file )
{
    char nextChar;
    int numberOfLines = 0;
    while ( ( nextChar = getc( file ) ) != EOF )
        if( nextChar == NEWLINECH ) numberOfLines++;
    fclose( file );
    return numberOfLines;
}

void reloadGuests( void )
{
    FILE *file = fopen( FNAME, TOREAD );
    if ( file ) readGuestsDataFromFile( file );
}

void readGuestsDataFromFile( FILE* file )
{
    char nextChar;
    int guestsCount  = -1;
    while( ( nextChar = getc( file ) ) != EOF )
    {
        guestsCount += 1;
        mallocForGuests( guestsCount );
        readOneGuest( &nextChar, guestsCount, file );
        setToStringOnGuest( guestsCount );
    }
       fclose( file );
 }

void mallocForGuests( int guestsCount )
{
    guests[guestsCount].placeInQueue  = (char *)malloc( BYTE );
    guests[guestsCount].name          = (char *)malloc( BYTE );
    guests[guestsCount].email         = (char *)malloc( BYTE );
    guests[guestsCount].exhib_id      = (char *)malloc( BYTE );
    guests[guestsCount].date          = (char *)malloc( DATELENGTH );
}

void readOneGuest( char *nextChar, int guestsCount, FILE *file )
{
    int  strCount  =  0;
    int  dataCount =  0;
    while( guestRemains( nextChar ) )
    {
        if(*nextChar == GUESTSEPARATORCH)
            switchOnGuestData( &dataCount, &guestsCount, &strCount );
        else
            keepAppending( &dataCount, &guestsCount, &strCount, nextChar );
        *nextChar = getc( file );
    }
}

int guestRemains( char* nextChar )
{
    return ( *nextChar != NEWLINECH && *nextChar != EOF
    ? TRUE
    : FALSE
           );
}

void switchOnGuestData( int* dataCount, int* guestsCount, int* strCount )
{
    closeDataStringWithAppropriateClosing( *dataCount, *guestsCount, *strCount, TERMINATINGCHARCH );
    *strCount   = 0;
    *dataCount += 1;
}

void closeDataStringWithAppropriateClosing( int dataCount, int guestsCount, int strCount, char char_t )
{
    switch( dataCount )
    {
        case 0: guests[guestsCount].placeInQueue[strCount] = char_t; break;
        case 1: guests[guestsCount].name[strCount]         = char_t; break;
        case 2: guests[guestsCount].email[strCount]        = char_t; break;
        case 3: guests[guestsCount].exhib_id[strCount]     = char_t; break;
        case 4: guests[guestsCount].date[strCount]         = char_t; break;
        default: break;
    }
}

void keepAppending( int* dataCount, int* guestsCount, int* strCount, char* nextChar )
{
    closeDataStringWithAppropriateClosing( *dataCount, *guestsCount, *strCount, *nextChar );
    *strCount  += 1;
}

void setToStringOnGuest( int guestsCount )
{
    char* result;
    //mallocStringResult( &result, guestsCount );
    result = malloc(   strlen( guests[guestsCount].placeInQueue )
                     + strlen( guests[guestsCount].name )
                     + strlen( guests[guestsCount].email )
                     + strlen( guests[guestsCount].exhib_id)
                     + strlen( guests[guestsCount].date )
                     + 5 );
    //buildStringResult( &result, vendegCnt );
    strcpy( result, guests[guestsCount].placeInQueue);
    strcat( result, TABULATOR );
    strcat( result, guests[guestsCount].name );
    strcat( result, TABULATOR );
    strcat( result, guests[guestsCount].email );
    strcat( result, TABULATOR );
    strcat( result, guests[guestsCount].exhib_id);
    strcat( result, TABULATOR );
    strcat( result, guests[guestsCount].date );
    strcat( result, "\0" );
    //setVendegStringToResult( &result, vendegCnt );
    guests[guestsCount].toString = result;
}

void checkCursorToEnter( void )
{
    int LASTPOSFORLISTING = numberOfGuests + 1;
    if(      cursor == STARTPOSFORLISTING ) addNewGuest();
    else if( cursor == LASTPOSFORLISTING )  backToMenu();
    else                                    modifyGuest(); //cursor = STARTPOSFORLISTING;
}

void addNewGuest( void )
{
    //readInputFromUser();
    clearConsole();
    printf( ASKFORNAME );
    char* name = getline();
    printf( ASKFOREMAIL );
    char* email = getline();
    printf( ASKFOREXHIBID );
    char* exhib_id = getline();
    const char* date = ADATE;
    //refreshVendegek
    numberOfGuests = countGuestsInFile();
    //Open file to append
    FILE *f = fopen( FNAME, TOAPPEND );
    if ( f == NULL )
    {
        printf( FILEERROR );
        exit( 1 );
    }
    //Build Vendeg ('row') to write into file
    char *result = (char *)malloc( 500 );
    char buffer[BYTE];
    strcpy( result, itoa( ( numberOfGuests + 1),buffer, 10 ) );
    strcat( result, GUESTSEPARATOR );
    strcat( result, name );
    strcat( result, GUESTSEPARATOR );
    strcat( result, email );
    strcat( result, GUESTSEPARATOR );
    strcat( result, exhib_id );
    strcat( result, GUESTSEPARATOR );
    strcat( result, date );
    //Write Vendeg ('row') into file
    fprintf( f, STRINGPLACENEWLINE, result );
    //Close file
    fclose( f );
    //PostCalcHere
    successAdd       = TRUE;
    numberOfGuests  += 1;
    cursor           = 1;
    loadGuestsList();
}

char* getline( void )
{
    char* line = malloc( 100 ), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if( line == NULL )
        return NULL;

    for(;;) {
        c = fgetc( stdin );
        if( c == EOF )
            break;

        if( --len == 0 ) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if( linen == NULL ) {
                free( linep );
                return NULL;
            }
            line = linen + ( line - linep );
            linep = linen;
        }

        if( ( *line = c ) == NEWLINECH )
            break;
        *line++;
    }
    *line = TERMINATINGCHARCH;
    return linep;
}

void backToMenu( void )
{
    menu   = 0;
    cursor = 1;
}

void modifyGuest( void )
{
    //   ---
    // ToDO ...
    //   ---
    endModification();
}

void endModification( void )
{
    successModify = TRUE;
    loadGuestsList();
    cursor = STARTPOSFORLISTING;
}

void deleteGuest( void )
{
    if( cursor == 0 || cursor == numberOfGuests + 1 ) return;
    loadGuestsList();
    clearFile();
    deleteAppropriateGuest();
    successRemove      = TRUE;
    numberOfGuests    -= 1;
    loadGuestsList();
}

void clearFile( void )
{
    FILE *f = fopen( FNAME, TOWRITE );
    if( f == NULL )
    {
        printf( FILEERROR );
        exit( 1 );
    }
    fprintf( f, EMPTYSTRING );
    fclose( f );
}

void deleteAppropriateGuest( void )
{
    FILE *f = fopen( FNAME, TOAPPEND );
    if ( f == NULL )
    {
        printf( FILEERROR );
        exit( 1 );
    }
    int i;
    int after = FALSE;
    for( i = 0; i < numberOfGuests; ++i )
    {
        if( i != ( cursor - 1 ) )
        {
            //change Sorsam if needed
            if( after == TRUE )
            {
                char buffer[BYTE];
                guests[i].placeInQueue = itoa( atoi( guests[i].placeInQueue ) - 1, buffer, BYTE );
            }
            //allocate memory
            char *result = malloc( 500 );
            //buildResultSrting
            strcpy( result, guests[i].placeInQueue );
            strcat( result, GUESTSEPARATOR );
            strcat( result, guests[i].name );
            strcat( result, GUESTSEPARATOR );
            strcat( result, guests[i].email );
            strcat( result, GUESTSEPARATOR );
            strcat( result, guests[i].exhib_id );
            strcat( result, GUESTSEPARATOR );
            strcat( result, guests[i].date );
            //Write Vendeg into File
            fprintf( f, STRINGPLACENEWLINE, result );
        }
        else
            after = TRUE;
    }
    fclose( f );
}

void tryPgUpWithCursor( void )
{
    cursor = ( ( cursor - PAGEJUMP <= 0 )
    ? 0
    : cursor - PAGEJUMP
             );
}

void tryPgDnWithCursor( void )
{
    cursor = ( ( cursor + PAGEJUMP >= numberOfGuests + 1 )
    ? numberOfGuests + 1
    : cursor + PAGEJUMP
             );
}

void render( void )
{
    clearConsole();
    renderProgramName();
    switch( menu )
    {
        case 0:  renderMenuItems();  break;
        case 1:  renderListItems();  break;
        default: break;
    }
}

void renderProgramName( void )
{
    printf( PROGRAMNAME );
}

void renderMenuItems( void )
{
    int i;
    for( i = 0; i < (int)NUMOFMENUOPTIONS; ++i )
        renderMenuItem( i );
}

void renderMenuItem( const int i )
{
    if( cursor == ( i + 1 ) )
        SetConsoleTextAttribute( hConsole, SELECTCOLOR );
    printf( menuOptions[i] );
    SetConsoleTextAttribute( hConsole, BASECOLOR );
}

void renderListItems( void )
{
    renderNewGuest();
    renderEacItem();
    renderGuestsInfo();
    renderBackButton();
    renderInf();
    checkSuccesses();
}

void renderNewGuest( void )
{
    if( cursor == 0 )
        SetConsoleTextAttribute( hConsole, SELECTCOLOR );
    printf( ADDNEWGUEST );
    SetConsoleTextAttribute( hConsole, BASECOLOR );
    printf( MENUSEPARATOR );
}

void renderEacItem( void )
{
    int i;
    for( i = 0; i < numberOfGuests; ++i )
        renderListItem( i );
}

void renderListItem( const int i )
{
    if( cursor == ( i + 1 ) ) SetConsoleTextAttribute( hConsole, SELECTCOLOR );
    printf( guests[i].toString );
    printf( NEWLINE );
    SetConsoleTextAttribute( hConsole, BASECOLOR );
}

void renderGuestsInfo( void )
{
    printf( MENUSEPARATOR );
    SetConsoleTextAttribute( hConsole, COMMENTCOLOR );
    printf( GUESTINFOCOMMENT, numberOfGuests );
    SetConsoleTextAttribute( hConsole, BASECOLOR );
}

void renderBackButton( void )
{
    if( cursor == numberOfGuests + 1 )
        SetConsoleTextAttribute( hConsole, SELECTCOLOR );
    printf( BACK );
    SetConsoleTextAttribute( hConsole, BASECOLOR );
}

void renderInf( void )
{
    if( cursor > 0 && cursor < numberOfGuests + 1 )
        renderInfo( INFOFORGUEST );
    else if( cursor == numberOfGuests + 1 )
        renderInfo( INFOFORNAVIGATION );
    else if( cursor == 0 )
        renderInfo( INFOFORNEWGUEST );
}

void renderInfo( const char* info )
{
    printf( FOURNEWLINE );
    SetConsoleTextAttribute( hConsole, INFOCOLOR );
    printf( info );
    SetConsoleTextAttribute( hConsole, BASECOLOR );
}

void checkSuccesses( void )
{
    if ( successAdd || successRemove || successModify )
    {
        char* str_t;
        if( successAdd )         str_t = NEWGUESTCOMMENT;
        else if( successRemove ) str_t = DELETECOMMENT;
        else if( successModify ) str_t = MODIFYCOMMENT;
        renderSuccess( str_t );
    }
}

void renderSuccess( char* str_t )
{
    SetConsoleTextAttribute( hConsole, COMMENTCOLOR );
    printf( str_t, guests );
    SetConsoleTextAttribute( hConsole, BASECOLOR );
    resetSuccess();
}

void resetSuccess( void )
{
    successAdd    = FALSE;
    successRemove = FALSE;
    successModify = FALSE;
}

void postCalc( void )
{
    menu_K1 = menu;
}

void sayGoodBye( void )
{
    clearConsole();
    printf( GOODBYESTRING );
}

