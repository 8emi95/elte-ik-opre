#ifndef GLOBAL_VARS_H_INCLUDED
#define GLOBAL_VARS_H_INCLUDED



STRUCT Event* events;
STRUCT Guest* guests;
CHAR    input;
INT     menu;
INT     menuK1;
INT     cursor;
INT     numberOfEvents;
INT     numberOfGuests;
BOOL    wasInInfo;
BOOL    wasInEventModify;
BOOL    wasInEventCreate;
BOOL    wasInEventDelete;
BOOL    wasInGuestModify;
BOOL    wasInGuestCreate;
BOOL    wasInGuestDelete;
STRING  currentFooterInfo;
STRING  menuLabels[ NUMBER_OF_MENU_ITEMS ] = { EVENTS_LABEL, GUESTS_LABEL, INFO_LABEL, EXIT_LABEL };



#endif // GLOBAL_VARS_H_INCLUDED
