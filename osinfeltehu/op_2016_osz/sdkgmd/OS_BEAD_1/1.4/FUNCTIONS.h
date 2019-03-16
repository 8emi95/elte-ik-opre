#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED



#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>


VOID  initSystem( VOID );
  VOID  initConsoleUI( VOID );
    VOID  initConsole( VOID );
    VOID  initConsoleCursor( VOID );
    VOID  initConsoleColor( VOID );
      VOID  setColor( COLOR );
      VOID  clearConsole( VOID );
  VOID  initWelcomePage( VOID );
    VOID  renderStringToConsole( C_STRING, const int);
  VOID  initInput( VOID );
BOOL  userIsNotQuitting( VOID );
VOID  programIsRunning( VOID );
  VOID  getUserInput( VOID );
  BOOL  inputIsNotRelevant( VOID );
  VOID  doControlCycleOn( VOID );
    VOID  calc( VOID );
      VOID  calcInMenu( VOID );
        VOID  tryMoveCursor( const int, const int, const int );
        VOID  enterInMenu( VOID );
      VOID  calcInEvents( VOID );
      VOID  calcInListing( VOID );
        VOID  handleGuestsLoading( VOID );
          VOID  loadGuestsList( VOID );
            VOID  refreshGuests( VOID );
              VOID  refreshNumberOfGuests( VOID );
                int   countGuestsInFile( VOID );
                  int   countLines( FILE* );
              VOID  mallocForGuests( VOID );
            VOID  reloadGuests( VOID );
              VOID  readGuestsDataFromFile( FILE* );
                VOID  mallocForGuestsData( int );
                VOID  readOneGuest( char*, int, FILE* );
                  BOOL  guestRemains( char* );
                  VOID  switchOnGuestData( int*, int*, int* );
                    VOID  closeDataStringWithAppropriateClosing( int, int, int, char );
                  VOID  keepAppending( int*, int*, int*, char* );
                VOID  setToStringOnGuest( int );
                  char* mallocForStringResult( const int );
                  char* buildGuestResultString(char**, const int );
        VOID  enterInListing( VOID );
          VOID  addNewGuest( VOID );
            VOID  renderPreInNewGuest( VOID );
              VOID  renderProgramName( VOID );
            VOID  readInputFromUser(char**, char**, char**, char** );
              char* getline( VOID );
            char* buildNewGuestStringResult( char*, char*, char*, char*, BOOL, char* );
            VOID  appendResultGuestStringToFileDataBase( char* );
            VOID  postCalcNewGuest( VOID );
          VOID  backToMenu( VOID );
          VOID  modifyGuest( VOID );
            int   getUserChoiceOfModification( VOID );
              BOOL  userInputIsNotApproriate( char );
              VOID  renderModificationChoices( VOID );
              VOID  renderSelectedGuestToModify( VOID );
              VOID  renderUserChoiceOnUI( char );
            VOID  modifyName( VOID );
            VOID  modifyEmail( VOID );
            VOID  modifyEvent_id( VOID );
            VOID  endModification( VOID );
              VOID  saveModification( VOID );
        VOID  deleteGuest( VOID );
          BOOL  userIsNotSure( VOID );
            BOOL inputIsNotRelevantForDeleteConfirm( char );
          VOID  clearFile( VOID );
          VOID  deleteAppropriateGuest( VOID );
            VOID  iteratingThroughAllGuests( FILE* );
              VOID  writingOneGuestToFile( FILE* f, BOOL*, const int );
                VOID  changePlaceInQueueIfNeeded( BOOL*, const int );
        VOID  tryPgUpWithCursor( VOID );
        VOID  tryPgDnWithCursor( VOID );
      VOID  calcInInfo( VOID );
    VOID  render( VOID );
      VOID  renderMenuItems( VOID );
        VOID  renderMenuItem( const int );
        VOID  renderControlInfo( VOID );
          VOID renderStringToConsole2( char*, char, char );
      VOID  renderEventItems( VOID );
      VOID  renderListItems( VOID );
        VOID  renderNewGuestButton( VOID );
        VOID  renderEacListItem( VOID );
          VOID  renderListItem( const int );
        VOID  renderGuestsInfo( VOID );
        VOID  renderBackButton( VOID );
        VOID  renderInf( VOID );
          VOID  renderInfo( char* );
        VOID  checkSuccesses( VOID );
          VOID  renderSuccess( char* );
          VOID  resetSuccess( VOID );
      VOID  renderInfoItems( VOID );
    VOID  postCalc( VOID );
VOID  sayGoodBye( VOID );



#endif // FUNCTIONS_H_INCLUDED
