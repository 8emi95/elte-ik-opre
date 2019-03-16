//--------------------------------------------------------------------
//-------------------------------INCLUDES-----------------------------
//--------------------------------------------------------------------


#include "MAKROS.h"
#include "GUESTSTRUCT.h"
#include "GLOBALVARS.h"
#include "FUNCTIONS.h"


//--------------------------------------------------------------------
//------------------------------ENTRY POINT---------------------------
//--------------------------------------------------------------------


int main()
{
    initSystem();
    while( userIsNotQuitting() ) programIsRunning();

    sayGoodBye();
    return WELLDONE;
}


//--------------------------------------------------------------------
//------------------------------FUNCTIONS-----------------------------
//--------------------------------------------------------------------

                        /* INIT SYSTEM */
VOID initSystem( VOID )
{
    initConsoleUI();
    initWelcomePage();
    initInput();
}
                        /* INIT CONSOLE UI */
VOID initConsoleUI( VOID )
{
    initConsole();
    initConsoleCursor();
    initConsoleColor();
}
                        /* INIT CONSOLE */
VOID initConsole( VOID )
{
    hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
}
                        /* INIT CONSOLE CURSOR */
VOID initConsoleCursor( VOID )
{
    CONSOLE_CURSOR_INFO info;
    info.dwSize   = HUNDRED;
    info.bVisible = FALSE;
    SetConsoleCursorInfo( hConsole, &info );
}
                        /* INIT CONSOLE COLOR */
VOID initConsoleColor( VOID )
{
    setColor( BASECOLOR );
    clearConsole();
}
                        /* INIT CONSOLE COLOR */
VOID setColor( COLOR color )
{
    SetConsoleTextAttribute( hConsole, color );
}

VOID  clearConsole( VOID )
{
    system( CLEAR );
}
                        /* INIT WELCOME PAGE */
VOID initWelcomePage( VOID )
{
    setColor( WELCOMECOLOR );
    renderStringToConsole( WELCOMESTRING, ZERO );
    setColor( INFOCOLOR );
    renderStringToConsole( TOSTARTSTRING, ZERO );
    setColor( BASECOLOR );
}
                        /* PRINT A STRING TO THE SCREEN */
VOID renderStringToConsole( C_STRING string, const int num )
{
    printf( string, num );
}
                        /* INIT INPUT */
VOID initInput( VOID )
{
    input = EMPTYCHAR;
}
                        /* CHECK IF USER WANTS TO QUIT */
BOOL userIsNotQuitting( VOID )
{
    BOOL answer = ( input != ESC && menu != EXIT );
    return answer;
}
                        /* RUN THE PROGRAM */
VOID programIsRunning( VOID )
{
    getUserInput();
    if( inputIsNotRelevant() ) return;
    doControlCycleOn();
}
                        /* GETTING INPUT FROM USER */
VOID getUserInput( VOID )
{
    input = getch();
}
                        /* CHECK IF INPUT IS RELEVANT */
BOOL inputIsNotRelevant( VOID )
{
    BOOL answer = (    input != ESC   && input != ENTER && input != KEY_w
                    && input != KEY_s && input != KEY_t && input != KEY_W
                    && input != KEY_S && input != KEY_T && input != UP
                    && input != DOWN  && input != PGUP  && input != PGDN
                  );
    return answer;
}
                        /* DO A RUNNING CYCLE */
VOID doControlCycleOn( VOID )
{
    calc();
    render();
    postCalc();
}
                        /* CALCULATE SYSTEM STATE */
VOID calc( VOID )
{
    switch( menu )
    {
        case INMAINMENU: calcInMenu();     break;
        case INEVENTS:   calcInEvents();   break;
        case INLISTING:  calcInListing();  break;
        case ININFO:     calcInInfo();     break;
        default: break;
    }
}
                        /* CALCULATE NEXT STEP IN MENU */
VOID calcInMenu( VOID )
{
    switch( input )
    {
        case KEY_w: case KEY_W: case UP:
            tryMoveCursor( MININDEXOFMENUOPTIONS, NUMBEROFMENUOPTIONS, -STEPSFORMENU  );
                 break;
        case KEY_s: case KEY_S: case DOWN:
            tryMoveCursor( NUMBEROFMENUOPTIONS, MININDEXOFMENUOPTIONS, STEPSFORMENU  );
                 break;
        case ENTER:  enterInMenu();
                 break;
        default: break;
    }
}
                        /* TRY TO MOVE CURSOR TO THE DIRECTION COMES FROM THE USER */
VOID tryMoveCursor( const int min, const int max, const int step )
{
    cursor = ( ( cursor == min )
    ? max
    : cursor + step
             );
}
                        /* USER PRESSED ENTER IN MENU */
VOID enterInMenu( VOID )
{
    menu   = cursor;
    cursor = STARTPOSFORLISTING;
    initInput();
    calc();
}

VOID calcInEvents( VOID )
{
    // TODO ...
    backToMenu();
}
                        /* CALCULATE NEXT STEP IN GUEST LISTING */
VOID calcInListing( VOID )
{
    handleGuestsLoading();
    int MAXINDEXOFLISTING = numberOfGuests + ONE;
    switch( (int)input )
    {
        case KEY_w: case KEY_W: case UP:
            tryMoveCursor( MININDEXOFLISTING, MAXINDEXOFLISTING, -ONE );
                                                       break;
        case KEY_s: case KEY_S: case DOWN:
            tryMoveCursor( MAXINDEXOFLISTING, MININDEXOFLISTING,  ONE );
                                                       break;
        case ENTER:              enterInListing();     break;
        case KEY_t: case KEY_T:  deleteGuest();        break;
        case PGUP:               tryPgUpWithCursor();  break;
        case PGDN:               tryPgDnWithCursor();  break;
        default: break;
    }
}
                        /* CHECK IF GUEST LIST LOAD IS NEEDED */
VOID handleGuestsLoading( VOID )
{
    BOOL NOTFIRSTATTEMPT = ( menu_K1 == menu );
    if( NOTFIRSTATTEMPT ) return;
    loadGuestsList();
    menu_K1 = menu;
}
                        /* LOAD GUEST LIST */
VOID loadGuestsList( VOID )
{
    refreshGuests();
    reloadGuests();
}
                        /* RESET LOCAL GUEST LIST */
VOID refreshGuests( VOID )
{
    free( guests );
    refreshNumberOfGuests();
    mallocForGuests();
}

VOID refreshNumberOfGuests( VOID )
{
    numberOfGuests = countGuestsInFile();
}
                        /* COUNT GUESTS IN THE DATABASE */
int countGuestsInFile( VOID )
{
    FILE *file = fopen( FNAME, TOREAD );
    if( file ) return countLines( file );
    return ERR;
}
                        /* COUNT NUMBER OF LINES IN FILE */
int countLines( FILE* file )
{
    char nextChar;
    int  numberOfLines = ZERO;
    while ( ( nextChar = getc( file ) ) != EOF )
        if( nextChar == NEWLINECH )
            numberOfLines++;
    fclose( file );
    return numberOfLines;
}

VOID mallocForGuests( VOID )
{
    guests = malloc( numberOfGuests * sizeof( struct Guest ) );
}
                        /* RELOAD LOCAL GUEST LIST */
VOID reloadGuests( VOID )
{
    FILE *file = fopen( FNAME, TOREAD );
    if ( file ) readGuestsDataFromFile( file );
}
                        /* READ GUESTS FROM FILE */
VOID readGuestsDataFromFile( FILE* file )
{
    char nextChar;
    int guestsCount  = -ONE;
    while( ( nextChar = getc( file ) ) != EOF )
    {
        guestsCount += ONE;
        mallocForGuestsData( guestsCount );
        readOneGuest( &nextChar, guestsCount, file );
        setToStringOnGuest( guestsCount );
    }
       fclose( file );
 }
                        /* ALLOCATE MEMORY FOR GUESTS */
VOID mallocForGuestsData( int guestsCount )
{
    guests[guestsCount].placeInQueue  = (char *)malloc( BYTE );
    guests[guestsCount].name          = (char *)malloc( BYTE );
    guests[guestsCount].email         = (char *)malloc( BYTE );
    guests[guestsCount].event_id      = (char *)malloc( BYTE );
    guests[guestsCount].date          = (char *)malloc( BYTE );
}
                        /* READ ONLY ONE GUEST FROM FILE */
VOID readOneGuest( char *nextChar, int guestsCount, FILE *file )
{
    int  strCount  =  ZERO;
    int  dataCount =  ZERO;
    BOOL ITISAGUESTDATASEPARATOR = ( *nextChar == GUESTSEPARATORCH );
    while( guestRemains( nextChar ) )
    {
        if( ITISAGUESTDATASEPARATOR )
            switchOnGuestData( &dataCount, &guestsCount, &strCount );
        else
            keepAppending( &dataCount, &guestsCount, &strCount, nextChar );
        *nextChar = getc( file );
        ITISAGUESTDATASEPARATOR = ( *nextChar == GUESTSEPARATORCH );
    }
}
                        /* CHECK IF THERE IS STILL GUEST IN THE FILE TO READ */
BOOL guestRemains( char* nextChar )
{
    return ( *nextChar != NEWLINECH && *nextChar != EOF
    ? TRUE
    : FALSE
           );
}
                        /* CHANGE THE GUEST DATA TO LOAD */
VOID switchOnGuestData( int* dataCount, int* guestsCount, int* strCount )
{
    closeDataStringWithAppropriateClosing( *dataCount,
                                           *guestsCount,
                                           *strCount,
                                           TERMINATINGCHARCH );
    *strCount   = ZERO;
    *dataCount += ONE;
}
                        /* CLOSE A DATA C_STRING WITH A CHARACTER (OFTEN \0) */
VOID closeDataStringWithAppropriateClosing( int dataCount, int guestsCount,
                                            int strCount,  char char_t )
{
    switch( dataCount )
    {
        case PLACEDATA: guests[guestsCount].placeInQueue[strCount] = char_t; break;
        case NAMEDATA:  guests[guestsCount].name[strCount]         = char_t; break;
        case EMAILDATA: guests[guestsCount].email[strCount]        = char_t; break;
        case EVENTDATA: guests[guestsCount].event_id[strCount]     = char_t; break;
        case DATEDATA:  guests[guestsCount].date[strCount]         = char_t; break;
        default: break;
    }
}
                        /* APPEND NEXT CHARACTER TO GUEST DATA */
VOID keepAppending( int* dataCount, int* guestsCount, int* strCount, char* nextChar )
{
    closeDataStringWithAppropriateClosing( *dataCount, *guestsCount, *strCount, *nextChar );
    *strCount  += ONE;
}
                        /* SET GUEST'S TOSTRING DATA FIELD */
VOID setToStringOnGuest( int guestsCount )
{
    char* result = mallocForStringResult( guestsCount );
    buildGuestResultString( &result, guestsCount );
    guests[guestsCount].toString = result;
}

char* mallocForStringResult( const int guestsCount )
{
    return malloc(   strlen( guests[guestsCount].placeInQueue )
                   + strlen( guests[guestsCount].name )
                   + strlen( guests[guestsCount].email )
                   + strlen( guests[guestsCount].event_id)
                   + strlen( guests[guestsCount].date )
                   + NUMBEROFGUESTARGUMENTS );
}

char* buildGuestResultString(char** result, const int guestsCount )
{
    strcpy( *result, guests[guestsCount].placeInQueue);
    strcat( *result, TABULATOR );
    strcat( *result, guests[guestsCount].name );
    strcat( *result, TABULATOR );
    strcat( *result, guests[guestsCount].email );
    strcat( *result, TABULATOR );
    strcat( *result, guests[guestsCount].event_id);
    strcat( *result, TABULATOR );

    char *ptr;
    long t_l = strtol(guests[guestsCount].date, &ptr, TENTHNUMBERSYSTEM);
    char buffer [BYTE];
    struct tm * timeinfo;
    timeinfo = localtime( &t_l );
    strftime ( buffer, BYTE, DATEFORMAT, timeinfo );
    strcat( *result, buffer );

    return *result;
}
                        /* USER PRESSED ENTER IN GUEST LISTING */
void enterInListing( void )
{
    int LASTPOSFORLISTING = numberOfGuests + ONE;
    BOOL ONNEWGUESTBUTTON = ( cursor == STARTPOSFORLISTING );
    BOOL ONBACKBUTTON     = ( cursor == LASTPOSFORLISTING );
    if     ( ONNEWGUESTBUTTON )  addNewGuest();
    else if( ONBACKBUTTON )      backToMenu();
    else                         modifyGuest();
}
                        /* ADD NEW GUEST TO DATABASE */
void addNewGuest( void )
{
    char* name, * email, * event_id, * date, * result;
    renderPreInNewGuest();
    refreshNumberOfGuests();
    readInputFromUser( &name, &email, &event_id, &date );
    result = buildNewGuestStringResult( name, email, event_id, date, FALSE, SOMESTRING );
    appendResultGuestStringToFileDataBase( result );
    postCalcNewGuest();
}

void renderPreInNewGuest( void )
{
    clearConsole();
    renderProgramName();
    setColor( INFOCOLOR );
    renderStringToConsole( NEWGUESTSTRING, ZERO );
    renderStringToConsole( ASKFORNAME, ZERO );
    setColor( BASECOLOR );
}
                        /* RENDER THE PROGRAM'S NAME TO UI */
void renderProgramName( void )
{
    setColor( PROGRAMNAMECOLOR );
    renderStringToConsole( PROGRAMNAME, ZERO );
    setColor( BASECOLOR );
}

void readInputFromUser(char** name, char** email, char** event_id, char** date )
{
    *name = getline();
    setColor( INFOCOLOR );
    renderStringToConsole( ASKFOREMAIL, ZERO );
    setColor( BASECOLOR );
    *email = getline();
    setColor( INFOCOLOR );
    renderStringToConsole( ASKFOREVENTID, ZERO );
    setColor( BASECOLOR );
    *event_id = getline();
    time_t* rawtime = (time_t*)malloc( sizeof( time_t ) );
    time( rawtime );
    char* str = malloc( BYTE * sizeof(char) );
    sprintf( str, LONGDECIMALSTRING, (long)*rawtime );
    *date = str;
}
                        /* GET ONE LINE OUT OF A FILE */
/* NEEDS REFAKTORING !!!!! */
char* getline( void )
{
    char* line = malloc( HUNDRED ), * linep = line;
    size_t lenmax = HUNDRED, len = lenmax;
    int c;

    if( line == NULL ) return NULL;

    for(;;) {
        c = fgetc( stdin );
        if( c == EOF )
            break;

        if( --len == ZERO ) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= TWO);

            if( linen == NULL ) {
                free( linep );
                return NULL;
            }
            line = linen + ( line - linep );
            linep = linen;
        }

        if( ( *line = c ) == NEWLINECH )
            break;
        (int)*line++;
    }
    *line = TERMINATINGCHARCH;
    return linep;
}

char* buildNewGuestStringResult( char* name,       char*  email,
                                 char* event_id,   char*  date,
                                 BOOL  lastNeeded, char*  placeInQueue )
{
    int sizeToMalloc = (   strlen( name )
                         + strlen( email )
                         + strlen( event_id )
                         + strlen( date )
                         + BYTE );
    char *result = (char *)malloc( sizeToMalloc );
    char buffer[BYTE];
    if( !lastNeeded ) strcpy(  result,
                               itoa( ( numberOfGuests + ONE ),
                                       buffer,
                                       TENTHNUMBERSYSTEM ) );
    else strcpy(  result, placeInQueue );
    strcat( result, GUESTSEPARATOR );
    strcat( result, name );
    strcat( result, GUESTSEPARATOR );
    strcat( result, email );
    strcat( result, GUESTSEPARATOR );
    strcat( result, event_id );
    strcat( result, GUESTSEPARATOR );
    strcat( result, date );

    return result;
}

void appendResultGuestStringToFileDataBase( char* result )
{
    FILE *f = fopen( FNAME, TOAPPEND );
    if ( f == NULL ) exit( FILEOPENERRORWHILEAPPEND );
    fprintf( f, STRINGPLACENEWLINE, result );
    fclose( f );
}

void postCalcNewGuest( void )
{
    successAdd       = TRUE;
    numberOfGuests  += ONE;
    cursor           = STARTPOSFORLISTING;
    loadGuestsList();
}
                        /* USER WANTS TO GO BACK TO MENU */
void backToMenu( void )
{
    menu   = INMAINMENU;
    cursor = STARTPOSFORMENU;
}
                        /* USER WANTS TO MODIFY ONE GUEST */
void modifyGuest( void )
{
    loadGuestsList();
    int choice = getUserChoiceOfModification();
    switch( choice )
    {
        case MODIFYNAME:     modifyName();      break;
        case MODIFYEMAIL:    modifyEmail();     break;
        case MODIFYEVENTID:  modifyEvent_id();  break;
        default: break;
    }
    endModification();
}

int getUserChoiceOfModification( void )
{
    char ch_t = EMPTYCHAR;
    while( userInputIsNotApproriate( ch_t ) )
    {
        clearConsole();
        renderProgramName();
        renderModificationChoices();
        ch_t = getch();
        renderUserChoiceOnUI( ch_t );
    }
    return atoi( &ch_t );
}

BOOL userInputIsNotApproriate( char ch_t )
{
    return ( (    ( ch_t != CHANGENAME )
               && ( ch_t != CHANGEEMAIL )
               && ( ch_t != CHANGEEVENT )
               && ( ch_t != ESC ) )
    ? TRUE
    : FALSE
            );
}

void renderModificationChoices( void )
{
    renderSelectedGuestToModify();
    setColor( INFOCOLOR );
    renderStringToConsole( MODIFICATIONCHIOCES, ZERO );
    setColor( BASECOLOR );
}

void renderSelectedGuestToModify( void )
{
    setColor( COMMENTCOLOR );
    renderStringToConsole( guests[ cursor - ONE ].toString, ZERO );
    setColor( BASECOLOR );
}

void renderUserChoiceOnUI( char ch_t )
{
    BOOL  render = FALSE;
    char* str;
    switch( ch_t )
    {
        case CHANGENAME:   str = CHANGENAMESTRING;  render = TRUE; break;
        case CHANGEEMAIL:  str = CHANGEEMAILSTRING; render = TRUE; break;
        case CHANGEEVENT:  str = CHANGEEVENTSTRING; render = TRUE; break;
        default: break;
    }
    if( render ) renderStringToConsole( str, ZERO );
}

void modifyName( void )
{
    setColor( INFOCOLOR );
    renderStringToConsole( ASKFORNEWNAME, ZERO );
    setColor( BASECOLOR );
    char* name = getline();
    guests[ cursor - ONE ].name = name;
}

void modifyEmail( void )
{
    setColor( INFOCOLOR );
    renderStringToConsole( ASKFORNEWEMAIL, ZERO );
    setColor( BASECOLOR );
    char* email = getline();
    guests[ cursor - ONE ].email = email;
}

void modifyEvent_id( void )
{
    setColor( INFOCOLOR );
    renderStringToConsole( ASKFORNEWEVENTID, ZERO );
    setColor( BASECOLOR );
    char* event_id = getline();
    guests[ cursor - ONE ].event_id = event_id;
}
                        /* MAKE NECESSARY CHANGES TO END THE GUEST MODIFICATION */
void endModification( void )
{
    clearFile();
    cursor = STARTPOSFORLISTING;
    saveModification();
}

void saveModification( void )
{
    deleteAppropriateGuest();
    successModify      = TRUE;
    numberOfGuests    -= ONE;
    loadGuestsList();
}
                        /* USER WANTS TO DELETE ONE GUEST */
void deleteGuest( void )
{
    int BACKBUTTONOPTION = numberOfGuests + ONE;
    BOOL NOTONGUEST = (    cursor == NEWGUESTOPTION
                        || cursor == BACKBUTTONOPTION  );
    if( NOTONGUEST )      return;
    if( userIsNotSure() ) return;
    loadGuestsList();
    clearFile();
    deleteAppropriateGuest();
    successRemove      = TRUE;
    numberOfGuests    -= ONE;
    loadGuestsList();
}

BOOL userIsNotSure( VOID )
{
    setColor( COMMENTCOLOR );
    printf( AREYOUSURETODELETE );
    setColor( BASECOLOR );
    char ch = ' ';
    while( ch != yes && ch != YES && ch != no && ch != NO && ch != ESC && ch != ENTER ) ch = getch();
    return ( ( ( ch == no ) || ( ch == NO ) || ch == ESC )
    ? TRUE
    : FALSE
           );
}
                        /* MAKE THE DATABASE FILE EMPTY */
VOID clearFile( VOID )
{
    FILE *f = fopen( FNAME, TOWRITE );
    if( f == NULL ) exit( FILEOPENERRORWHILECLEAR );
    fclose( f );
}
                        /* WRITE GUESTS BACK TO FILE APART FROM THE DELETED ONE */
VOID deleteAppropriateGuest( VOID )
{
    FILE* f = fopen( FNAME, TOAPPEND );
    if ( f == NULL ) exit( FILEOPENERRORWHILEDELETE );
    iteratingThroughAllGuests( f );
    fclose( f );
}

VOID iteratingThroughAllGuests( FILE* f )
{
    int i;
    BOOL after = FALSE;
    for( i = ZERO; i < numberOfGuests; ++i )
        writingOneGuestToFile( f, &after, i );
}

VOID writingOneGuestToFile( FILE* f, BOOL* after, const int guestIndex )
{
    BOOL GUESTTODELETE = ( guestIndex == ( cursor - ONE ) );
    if( GUESTTODELETE )
    {
        *after = TRUE;
        return;
    }
    changePlaceInQueueIfNeeded( after, guestIndex );
    char *result = buildNewGuestStringResult( guests[guestIndex].name,
                                              guests[guestIndex].email,
                                              guests[guestIndex].event_id,
                                              guests[guestIndex].date,
                                              TRUE,
                                              guests[guestIndex].placeInQueue );
    fprintf( f, STRINGPLACENEWLINE, result );
}

VOID changePlaceInQueueIfNeeded( BOOL* after, const int i )
{
    if( (BOOL)*after )
    {
        char buffer[BYTE];
        guests[i].placeInQueue = itoa( (atoi( guests[i].placeInQueue ) - ONE ),
                                        buffer,
                                        TENTHNUMBERSYSTEM );
    }
}
                        /* TRY MOVE THE CURSOR WITH PAGEJUMP UNITS UP */
VOID tryPgUpWithCursor( VOID )
{
    cursor = ( ( cursor - PAGEJUMP <= STARTPOSFORLISTING )
    ? STARTPOSFORLISTING
    : cursor - PAGEJUMP
             );
}
                        /* TRY MOVE THE CURSOR WITH PAGEJUMP UNITS DOWN */
VOID tryPgDnWithCursor( VOID )
{
    int LASTITEMOFLISTING = numberOfGuests + ONE;
    cursor = ( ( cursor + PAGEJUMP >= LASTITEMOFLISTING )
    ? LASTITEMOFLISTING
    : cursor + PAGEJUMP
             );
}

VOID calcInInfo( VOID )
{
    // TODO ...
    clearConsole();
    renderProgramName();
    renderStringToConsole( PROGRAMINFO, ZERO );
    setColor( INFOCOLOR );
    renderStringToConsole( PROGRAMINFOCONTROL, ZERO );
    setColor( BASECOLOR );
    getch();
    backToMenu();
}
                        /* RENDER OUTPUT TO UI */
VOID render( VOID )
{
    clearConsole();
    renderProgramName();
    switch( menu )
    {
        case INMAINMENU:  renderMenuItems();  break;
        case INEVENTS:    renderEventItems(); break;
        case INLISTING:   renderListItems();  break;
        case ININFO:      renderInfoItems();  break;
        default: break;
    }
}
                        /* RENDER MENU ITEMS TO UI */
VOID renderMenuItems( VOID )
{
    int i;
    for( i = ZERO; i < (int)NUMBEROFMENUOPTIONS; ++i )
        renderMenuItem( i );
    renderControlInfo();
}
                        /* RENDER MENU ONE ITEM TO UI */
VOID renderMenuItem( const int itemIndex )
{
    int  INDEXINMENU = ( itemIndex + ONE );
    BOOL ONITEM = ( cursor == INDEXINMENU );
    BOOL ONEVENTSOPTION = ( itemIndex == EVENTSOPTION );
    if( ONITEM )
    {
        if( ONEVENTSOPTION )
            setColor( UNFINISHEDSELECTEDCOLOR );
        else
            setColor( SELECTCOLOR );
    }
    else if( ONEVENTSOPTION )
        setColor( UNFINISHEDCOLOR );
    renderStringToConsole( menuOptions[itemIndex], ZERO );
    setColor( BASECOLOR );
}

VOID renderControlInfo( VOID )
{
    setColor( INFOCOLOR );
    renderStringToConsole2( NFOCONTROL, UPARROW, DOWNARROW );
    setColor( BASECOLOR );
}

VOID renderStringToConsole2( char* string, char c1, char c2 )
{
    printf( string, c1, c2 );
}

VOID renderEventItems( VOID )
{
    // ToDo ...
}
                        /* RENDER LIST ITEMS TO UI */
VOID renderListItems( VOID )
{
    renderNewGuestButton();
    renderEacListItem();
    renderGuestsInfo();
    renderBackButton();
    renderInf();
    checkSuccesses();
}
                        /* RENDER NEW GUEST BUTTON TO UI */
VOID renderNewGuestButton( VOID )
{
    BOOL ONNEWGUESTBUTTON = ( cursor == STARTPOSFORLISTING );
    if( ONNEWGUESTBUTTON ) setColor( SELECTCOLOR );
    renderStringToConsole( ADDNEWGUEST, ZERO );
    setColor( BASECOLOR );
    renderStringToConsole( MENUSEPARATOR, ZERO );
}
                        /* RENDER EACH LIST ITEM TO UI */
VOID renderEacListItem( VOID )
{
    int guestIndex;
    for( guestIndex = ZERO; guestIndex < numberOfGuests; ++guestIndex )
        renderListItem( guestIndex );
}
                        /* RENDER ONE LIST ITEM TO UI */
VOID renderListItem( const int guestIndex )
{
    BOOL ONBACKBUTTON = ( cursor == ( guestIndex + ONE ) );
    if( ONBACKBUTTON )  setColor( GUESTOCOLORSELECTED );
    else                setColor( GUESTOCOLOR );
    renderStringToConsole( guests[guestIndex].toString, ZERO );
    renderStringToConsole( NEWLINE, ZERO );
    setColor( BASECOLOR );
}
                        /* RENDER THE NUMBER OF GUESTS TO UI */
VOID renderGuestsInfo( VOID )
{
    renderStringToConsole( MENUSEPARATOR, ZERO );
    setColor( COMMENTCOLOR );
    renderStringToConsole( GUESTINFOCOMMENT, numberOfGuests );
    setColor( BASECOLOR );
}
                        /* RENDER BACK BUTTON TO UI */
VOID renderBackButton( VOID )
{
    int LASTITEMOFLISTING = numberOfGuests + ONE;
    BOOL ONBACKBUTTON = ( cursor == LASTITEMOFLISTING );
    if( ONBACKBUTTON ) setColor( SELECTCOLOR );
    renderStringToConsole( BACK, ZERO );
    setColor( BASECOLOR );
}
                        /* RENDER INFORMATION TO UI */
VOID renderInf( VOID )
{
    int LASTITEMOFLISTING = numberOfGuests + ONE;
    BOOL ONAGUEST = (    cursor > STARTPOSFORLISTING
                      && cursor < LASTITEMOFLISTING  );
    BOOL ONBACKBUTTON = ( cursor == LASTITEMOFLISTING );
    BOOL ONNEWGUESTBUTTON = ( cursor == STARTPOSFORLISTING );
    if( ONAGUEST )              renderInfo( INFOFORGUEST );
    else if( ONBACKBUTTON )     renderInfo( INFOFORNAVIGATION );
    else if( ONNEWGUESTBUTTON ) renderInfo( INFOFORNEWGUEST );
    renderControlInfo();
}
                        /* RENDER SPECIFIC INFORMATION TO UI */
VOID renderInfo( char* info )
{
    renderStringToConsole( NEWLINE, ZERO );
    setColor( INFOCOLOR );
    renderStringToConsole( info, ZERO );
    setColor( BASECOLOR );
}
                        /* CHECK IF USER MADE ANY SUCCESSFUL OPERATION */
VOID checkSuccesses( VOID )
{
    BOOL SUCCESSFULUSERACTION = (    successAdd
                                  || successRemove
                                  || successModify  );
    if ( SUCCESSFULUSERACTION )
    {
        char* str_t;
        if     ( successAdd )    str_t = NEWGUESTCOMMENT;
        else if( successRemove ) str_t = DELETECOMMENT;
        else if( successModify ) str_t = MODIFYCOMMENT;
        renderSuccess( str_t );
    }
}
                        /* RENDER THE SUCCESS MESSAGE OF THE OPERATION TO UI */
VOID renderSuccess( char* str_t )
{
    setColor( COMMENTCOLOR );
    renderStringToConsole( str_t, numberOfGuests );
    setColor( BASECOLOR );
    resetSuccess();
}
                        /* SET ALL SUCCESS VARIABLES TO FALSE */
VOID resetSuccess( VOID )
{
    successAdd    = FALSE;
    successRemove = FALSE;
    successModify = FALSE;
}

VOID renderInfoItems( VOID )
{
    // ToDo ...
}
                        /* POST CALCULATION OF THE SYSTEM AFTER RENDER */
VOID postCalc( VOID )
{
    menu_K1 = menu;
}
                        /* PRINT GOOD BYE MESSAGE TO THE UI */
VOID sayGoodBye( VOID )
{
    clearConsole();
    setColor( INFOCOLOR );
    renderStringToConsole( GOODBYESTRING, ZERO );
    setColor( BASECOLOR );
}


//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//-----------------------------END OF STORY---------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
