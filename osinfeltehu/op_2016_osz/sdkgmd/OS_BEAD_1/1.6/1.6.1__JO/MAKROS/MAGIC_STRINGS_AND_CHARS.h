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
#define ASK_FOR_EVENT_ID                                "Adja meg a rendezveny azonositojat:\t"
#define ASK_FOR_EVENT_TITLE                             "Adja meg a rendezveny cimet:\t\t"
#define ASK_FOR_EVENT_DATE                              "Adja meg a rendezveny datumat:\t\t"
#define ASK_FOR_GUEST_PLACE                             "Adja meg a vendeg helyet:\t\t"
#define ASK_FOR_GUEST_NAME                              "Adja meg a vendeg nevet:\t\t"
#define ASK_FOR_GUEST_EMAIL                             "Adja meg a vendeg email cimet:\t\t"
#define ASK_FOR_GUEST_DATE                              "Adja meg a vendeg jelentkezesi datumat:\t"
#define GOODBYE                                         "\n\tViszlat!\n"
#define PRESS_KEY_TO_CONTINUE                           "A folytatashoz nyomjon meg egy gombot."

#define MAIN_INFO_FOR_EVENTS_STRING                     "\n\n\n\t  w    -     Fel\n\t  s    -     Le\n\tENTER  -   Belepes a\n\t\t   rendezvenyek\n\t\t   listajahoz\n\t ESC   -   Kilepes"
#define MAIN_INFO_FOR_GUESTS_STRING                     "\n\n\n\t  w    -     Fel\n\t  s    -     Le\n\tENTER  -   Belepes a\n\t\t   vendegek\n\t\t   listajahoz\n\t ESC   -   Kilepes"
#define MAIN_INFO_FOR_INFO_STRING                       "\n\n\n\t  w    -     Fel\n\t  s    -     Le\n\tENTER  -   Belepes az\n\t\t   informacios\n\t\t   oldalra\n\t ESC   -   Kilepes"
#define MAIN_INFO_FOR_EXIT_STRING                       "\n\n\n\t  w    -     Fel\n\t  s    -     Le\n\tENTER  -   Kilepes\n\n\n\t ESC   -   Kilepes"

#define EVENT_INFO_FOR_NEW_EVENT_BUTTON_STRING          "\n\n\n\t  w    -     Fel\n\t  s    -     Le\n\tENTER  -   Uj rendezveny felvetele\n\n\n\t ESC   -   Vissza a menube"
#define EVENT_INFO_FOR_EVENT_STRING                     "\n\n\n\t  w    -     Fel\n\t  s    -     Le\n\tENTER  -   Rendezveny modositasa\n\t  t    -   Rendezveny torlese\n\n\t ESC   -   Vissza a menube"

#define GUEST_INFO_FOR_NEW_GUEST_BUTTON_STRING          "\n\n\n\t  w    -     Fel\n\t  s    -     Le\n\tENTER  -   Uj vendeg felvetele\n\n\n\t ESC   -   Vissza a menube"
#define GUEST_INFO_FOR_GUEST_STRING                     "\n\n\n\t  w    -     Fel\n\t  s    -     Le\n\tENTER  -   Vendeg modositasa\n\t  t    -   Vendeg torlese\n\n\t ESC   -   Vissza a menube"

#define INFO_FOR_BACK_BUTTON_STRING                     "\n\n\n\t  w    -     Fel\n\t  s    -     Le\n\tENTER  -   Vissza a menube\n\n\n\t ESC   -   Vissza a menube"


#define GUEST_INFO_STRING                               "\n\n\nin guests"

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
#define INFO_PAGE_FILE_1                                "cat TEMPLATES/INFO_PAGE_FILE_1.TXT"
#define INFO_PAGE_FILE_2                                "cat TEMPLATES/INFO_PAGE_FILE_2.TXT"
#define INFO_PAGE_FILE_3                                "cat TEMPLATES/INFO_PAGE_FILE_3.TXT"

#define SUCCESSFUL_EVENT_CREATE_STRING                  "Sikeresen letrehozta az esemenyt!"
#define SUCCESSFUL_GUEST_CREATE_STRING                  "Sikeresen hozzaadta az uj vendeget a listahoz!"
#define SUCCESSFUL_EVENT_MODIFICATION_STRING            "Sikeresen modositotta az esemenyt!"
#define SUCCESSFUL_GUEST_MODIFICATION_STRING            "Sikeresen modositotta a vendeg adatat!"
#define SUCCESSFUL_EVENT_DELETE_STRING                  "Sikeresen torolte az esemenyt!"
#define SUCCESSFUL_GUEST_DELETE_STRING                  "Sikeresen torolte a vendeget a listarol!"

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
#define EMPTY_INPUT										' '
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
