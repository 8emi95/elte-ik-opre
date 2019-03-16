#ifndef VALUE_MAKROS_H_INCLUDED
#define VALUE_MAKROS_H_INCLUDED


#define WELLDONE          0
#define IS_DONE           0


#define FALSE       0
#define TRUE        1
#define ZERO        0
#define ONE         1
#define TWO         2
#define THREE       3
#define FOUR        4
#define FIVE        5
#define HUNDRED   100
#define THOUSAND 1000


#define TENTH_NUMBER_SYSTEM      10
#define BYTE                    256


#define KEY_w                    119
#define KEY_W                     87
#define KEY_s                    115
#define KEY_S                     83
#define KEY_t                    116
#define KEY_T                     84
#define PGUP                      73
#define PGDN                      81
#define ENTER                     13
#define UP                        72
#define DOWN                      80
#define ESC                       27
#define SPACE                     32


#define ON_EVENTS_MENU         0
#define ON_GUESTS_MENU         1
#define ON_INFO_MENU           2
#define ON_EXIT_MENU           3
#define ON_NEW_EVENT_MENU      4
#define ON_NEW_GUEST_MENU      5
#define ON_EVENT_MODIFY_MENU   6
#define ON_GUEST_MODIFY_MENU   7

#define IN_MAIN_MENU  -1
#define IN_EVENT_MENU  0
#define IN_GUEST_MENU  1
#define ON_INFO_PAGE   2
#define ON_EXIT_PAGE   3
#define ON_NEW_EVENT   4
#define ON_NEW_GUEST   5

#define BY_MINUS_ONE    -1
#define BY_ONE           1
#define BY_MINUS_TEN   -10
#define BY_TEN          10

#define NUMBER_OF_MENU_ITEMS 4


#define ON_NEW_GUEST_BUTTON 0
#define PLUS_ONE            +1
#define MINUS_ONE           -1


#define SELECT_COLOR        46
#define BASE_COLOR          10

#define STARTING_POSITION    0


#define EVENTS_LABEL        "\t Rendezvenyek\t\t\n"
#define GUESTS_LABEL        "\t   Vendegek\t\t\n"
#define INFO_LABEL          "\t  Informacio\t\t\n"
#define EXIT_LABEL          "\t   Kilepes\t\t\n"

#define CLEAR           "CLS"


#define EVENTS_FILE             "DATABASE/EVENTS.TXT"
#define GUESTS_FILE             "DATABASE/GUESTS.TXT"

#define TO_READ                 "r"
#define TO_WRITE                "w"
#define TO_APPEND               "a"



#define NEW_EVENT_BUTTON_STRING     "\nUj Rendezveny felvetel\t\t\n"
#define NEW_GUEST_BUTTON_STRING     "\nUj Vendeg jelentkezese\t\t\n"
#define BACK_BUTTON_STRING          "\nVissza\t\t\t\t\n"

#define MENU_SEPARATOR_STRING   "--------------------------------\n"



#define NUMBER_OF_EVENT_DATA 3
#define NUMBER_OF_GUEST_DATA 5

#define EVENT_ID_DATA           0
#define EVENT_TITLE_DATA        1
#define EVENT_DATE_DATA         2

#define GUEST_PLACE_DATA           0
#define GUEST_NAME_DATA        1
#define GUEST_EMAIL_DATA         2
#define GUEST_EVENTID_DATA        3
#define GUEST_DATE_DATA         4


#define EXIT_PAGE_FILE                      "TEMPLATES/EXIT_PAGE_FILE.TXT"
#define INFO_PAGE_FILE                      "TEMPLATES/INFO_PAGE_FILE.TXT"
#define HEADER_FILE                         "TEMPLATES/HEADER.TXT"
#define FOOTER_FILE                         "TEMPLATES/FOOTER.TXT"
#define SUCCESSFUL_EVENT_CREATE             "TEMPLATES/SUCCESFUL_EVENT_CREATE.TXT"
#define SUCCESSFUL_GUEST_CREATE             "TEMPLATES/SUCCESFUL_GUEST_CREATE.TXT"
#define SUCCESSFUL_EVENT_MODIFICATION       "TEMPLATES/SUCCESSFUL_EVENT_MODIFICATION.TXT"
#define SUCCESSFUL_GUEST_MODIFICATION       "TEMPLATES/SUCCESSFUL_GUEST_MODIFICATION.TXT"



#define END_OF_FILE         EOF

#define SEPARATOR_CHAR           ';'
#define TERMINATING_CHAR         '\0'
#define SEPARATOR_STRING         ";"
#define TERMINATING_STRING       "\0"
#define EMPTY_CHAR               ' '
#define EMPTY_STRING             " "
#define END_OF_LINE_CHAR '\n'



#define FILE_CANNOT_BE_OPENNED -1


#define END_LINE_CHAR_PLUS_TERMINATING_CHAR_STRING      "\n\0"
#define STRING_PLUS_NEW_LINE                            "%s\n"
#define TABULATOR_STRING                                "\t"

#define DATEFORMAT                   "%Y/%m/%d"


#define ASK_FOR_EVENT_ID        "\nAdja meg a rendezveny azonositojat:\t"
#define ASK_FOR_EVENT_TITLE     "\nAdja meg a rendezveny cimet:\t\t"
#define ASK_FOR_EVENT_DATE      "\nAdja meg a rendezveny datumat:\t\t"

#define ASK_FOR_GUEST_PLACE     "\nAdja meg a vendeg helyet:\t\t"
#define ASK_FOR_GUEST_NAME      "\nAdja meg a vendeg nevet:\t\t"
#define ASK_FOR_GUEST_EMAIL     "\nAdja meg a vendeg email cimet:\t\t"
#define ASK_FOR_GUEST_DATE      "\nAdja meg a vendeg jelentkezesi datumat:\t"


#define MODIFICATION_OPTIONS_FOR_EVENT      "Rendezveny adatainak modositasa:\n--------------------------------\nMelyik adatot szeretne modositani?\n1. azonosito\n2. cim\n3. datum\n"
#define MODIFICATION_OPTIONS_FOR_GUEST      "Vengeg adatainak modositasa:\n----------------------------\nMelyik adatot szeretne modositani?\n1. nev\n2. email cim\n3. rendezveny\n"

#define CREATE_NEW_EVENT_STRING             "Uj Rendezveny Letrehozasa:\n--------------------------------\n"
#define CREATE_NEW_GUEST_STRING             "Uj Vengeg Feliratkozasa:\n----------------------------\n"


#define MOD_OPT_EVE_1   '1'
#define MOD_OPT_EVE_2   '2'
#define MOD_OPT_EVE_3   '3'

#define MOD_OPT_GUE_1   '1'
#define MOD_OPT_GUE_2   '2'
#define MOD_OPT_GUE_3   '3'
#define MOD_OPT_GUE_4   '4'
#define MOD_OPT_GUE_5   '5'

#endif // VALUE_MAKROS_H_INCLUDED
