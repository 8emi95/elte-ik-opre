#ifndef GLOBAL_VARS_H_INCLUDED
#define GLOBAL_VARS_H_INCLUDED


STRING menuLabels[ NUMBER_OF_MENU_ITEMS ] = { EVENTS_LABEL, GUESTS_LABEL, INFO_LABEL, EXIT_LABEL };
STRING currentFooterInfo;
STRUCT Guest* guests;
STRUCT Event* events;
//HANDLE hConsole;
INT  numberOfGuests;
INT  numberOfEvents;
INT  cursor;
INT  menu;
INT  menuK1;
CHAR input;


#endif // GLOBAL_VARS_H_INCLUDED
