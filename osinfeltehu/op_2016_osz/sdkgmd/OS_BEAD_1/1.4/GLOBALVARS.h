#ifndef GLOBALVARS_H_INCLUDED
#define GLOBALVARS_H_INCLUDED


struct Guest* guests;
struct Event* events;
HANDLE hConsole;
char*  menuOptions[NUMBEROFMENUOPTIONS] =  { EVENTS, GUESTS, INFORMATION, QUIT };
char   input                             =  EMPTYCHAR;
int    cursor                            =  STARTPOSFORMENU;
int    menu                              =  ZERO;
int    menu_K1                           =  ZERO;
int    numberOfGuests                    =  ZERO;
BOOL   successRemove                     =  FALSE;
BOOL   successAdd                        =  FALSE;
BOOL   successModify                     =  FALSE;


#endif // GLOBALVARS_H_INCLUDED
