#ifndef MAGIC_STRINGS_AND_CHARS_H_INCLUDED
#define MAGIC_STRINGS_AND_CHARS_H_INCLUDED



#define NORMAL                                          "\x1B[0m"
#define RED                                             "\x1B[31m"
#define GREEN                                           "\x1B[32m"
#define YELLOW                                          "\x1B[33m"
#define BLUE                                            "\x1B[34m"
#define MAGENTA                                         "\x1B[35m"
#define CYAN                                            "\x1B[36m"
#define WHITE                                           "\x1B[37m"

#define EVENTS_LABEL                                    "\n\t  Rendezvenyek\t\t"
#define GUESTS_LABEL                                    "\t    Vendegek\t\t"
#define INFO_LABEL                                      "\t   Informacio\t\t"
#define EXIT_LABEL                                      "\t    Kilepes\t\t"
#define NEW_EVENT_BUTTON_STRING                         "\nUj Rendezveny felvetel\t\t"
#define NEW_GUEST_BUTTON_STRING                         "\nUj Vendeg jelentkezese\t\t"
#define BACK_BUTTON_STRING                              "\nVissza\t\t\t\t"
#define MENU_SEPARATOR_STRING                           "--------------------------------"
#define MODIFICATION_OPTIONS_FOR_EVENT                  "Rendezveny adatainak modositasa:\n--------------------------------\nMelyik adatot szeretne modositani?\n1. azonosito\n2. cim\n3. datum"
#define MODIFICATION_OPTIONS_FOR_GUEST                  "Vengeg adatainak modositasa:\n----------------------------\nMelyik adatot szeretne modositani?\n1. nev\n2. email cim\n3. rendezveny"
#define CREATE_NEW_EVENT_STRING                         "Uj Rendezveny Letrehozasa:\n--------------------------------"
#define CREATE_NEW_GUEST_STRING                         "Uj Vengeg Feliratkozasa:\n----------------------------"
#define ASK_FOR_EVENT_ID                                "\nAdja meg a rendezveny azonositojat:\t"
#define ASK_FOR_EVENT_TITLE                             "\nAdja meg a rendezveny cimet:\t\t"
#define ASK_FOR_EVENT_DATE                              "\nAdja meg a rendezveny datumat:\t\t"
#define ASK_FOR_GUEST_PLACE                             "\nAdja meg a vendeg helyet:\t\t"
#define ASK_FOR_GUEST_NAME                              "\nAdja meg a vendeg nevet:\t\t"
#define ASK_FOR_GUEST_EMAIL                             "\nAdja meg a vendeg email cimet:\t\t"
#define ASK_FOR_GUEST_DATE                              "\nAdja meg a vendeg jelentkezesi datumat:\t"
#define GOODBYE                                         "\n\n\tViszlat!\n"
#define PRESS_KEY_TO_CONTINUE                           "A folytatashoz nyomjon meg egy gombot."

#define CLEAR                                           "clear"
#define CURSOR_OFF                                      "setterm -cursor off"
#define CURSOR_ON                                       "setterm -cursor on"
#define CAT_HEADER                                      "cat TEMPLATES/HEADER.TXT"
#define FOOTER_FILE                                     "cat TEMPLATES/FOOTER.TXT"
#define SUCCESSFUL_EVENT_CREATE                         "cat TEMPLATES/SUCCESFUL_EVENT_CREATE.TXT"
#define SUCCESSFUL_GUEST_CREATE                         "cat TEMPLATES/SUCCESFUL_GUEST_CREATE.TXT"
#define SUCCESSFUL_EVENT_MODIFICATION                   "cat TEMPLATES/SUCCESSFUL_EVENT_MODIFICATION.TXT"
#define SUCCESSFUL_GUEST_MODIFICATION                   "cat TEMPLATES/SUCCESSFUL_GUEST_MODIFICATION.TXT"
#define EXIT_PAGE_FILE                                  "cat TEMPLATES/EXIT_PAGE_FILE.TXT"
#define INFO_PAGE_FILE                                  "cat TEMPLATES/INFO_PAGE_FILE.TXT"

#define EVENTS_FILE                                     "DATABASE/EVENTS.TXT"
#define GUESTS_FILE                                     "DATABASE/GUESTS.TXT"

#define STRING_FOR_COLOR                                "%s"
#define USE_COLOR                                       "%s%s"
#define STRING_PLUS_NEW_LINE                            "%s\n"
#define NEW_LINE_PLUS_STRING                            "\n%s"
#define DATEFORMAT                                      "%Y/%m/%d"
#define LONG_DECIMAL_STRING                             "%ld"
#define END_LINE_CHAR_PLUS_TERMINATING_CHAR_STRING      "\n\0"
#define COLOR_CHANGING_STRING                           "%s \n"

#define EMPTY_STRING                                    " "
#define TABULATOR_STRING                                "\t"
#define SEPARATOR_STRING                                ";"
#define TERMINATING_STRING                              "\0"
#define END_OF_FILE                                     EOF

#define TO_READ                                         "r"
#define TO_WRITE                                        "w"
#define TO_APPEND                                       "a"

#define EMPTY_CHAR                                      ' '
#define END_OF_LINE_CHAR                                '\n'
#define SEPARATOR_CHAR                                  ';'
#define TERMINATING_CHAR                                '\0'

#define MOD_OPT_EVE_1                                   '1'
#define MOD_OPT_EVE_2                                   '2'
#define MOD_OPT_EVE_3                                   '3'
#define MOD_OPT_GUE_1                                   '1'
#define MOD_OPT_GUE_2                                   '2'
#define MOD_OPT_GUE_3                                   '3'
#define MOD_OPT_GUE_4                                   '4'
#define MOD_OPT_GUE_5                                   '5'



#endif // MAGIC_STRINGS_AND_CHARS_H_INCLUDED
