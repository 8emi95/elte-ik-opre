#ifndef MAKROS_H_INCLUDED
#define MAKROS_H_INCLUDED


#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>



#define PROGRAMNAME                  "\n   | / | |\\  |  /-\\    /--\\ |---\n   ||  | | \\ | | __    |  | |--\n   | \\ | |  \\|  \\_/    \\__/ |\n--------------------------------\n /-- -----  /\\   |\\  | |--\\  /--\n \\__   |   /__\\  | \\ | |   | \\__\n __/   |  /    \\ |  \\| |__/  __/\n--------------------------------\n\n\n"
#define PROGRAMINFO                  "  A program egy rendezvenyeket es az   |\n    azokra feliratkozo vendegeket      |\n    tartalmazo adatbazist tartja       |\n        nyilvan es kezeli is.          |\n                                       |\n   Az adatbazist TXT kiterjesztesu     |\n       fajlokkal valositja meg.        |\n  A fajlok tartalmarol feltetelezzuk,  |\n        hogy azt csak a program        |\n    szerkesztheti, es annak kezdeti    |\n      tartalma helyes formatumu.       |\n                                       |\n  A felhasznalonak lehetosege van uj   |\n  latogatot felvenni az adatbazisba,   |\n    anannak barmely adatat torolni,    |\n   illetve barmely latogatot torolni   |\n            a listarol.                |\n  A program uj rendezveny hozzaadasat  |\n    is biztositja, illetve ezek is     |\n    torolhetok vagy barmely adatuk     |\n            modosithato.               |\n                                       |\n Iranyitas billentyuzet segitsegevel:  |\n   a fel es le nyilak, illetve a \'w\'   |\n    es az \'s\' billyentyukkel fel es    |\n    lefele navigalhatunk; az ENTER     |\n  lenyomasaval az aktualis menuponton  |\n    aktivalhatjuk annak funkciojat     |\n  (pl: belep egy masik menube); a \'t\'  |\n  billentyuvel torolhetjuk az kurzor   |\n    aktualis poziciojan levo elemet    |\n (vendeg vagy rendezveny); az ESC-ape  |\n  gombbal pedig bármikor kilephetunk   |\n            a programbol.              |\n_______________________________________|"
#define PROGRAMINFOCONTROL           "\n\n\n Visszatereshez nyomjon meg egy gombot.\n"
#define GOODBYESTRING                "\n\n\n\n\n\n\tViszont Latasra!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
#define WELCOMESTRING                "--------------------------------------\n\tA program udvozli Ont!\n--------------------------------------\n\n\n\n\n"
#define MODIFICATIONCHIOCES          "\n\nVENDEG MODOSITASA:\n----------\n1. Nev modositasa.\n2. Email cim modositasa.\n3. Rendezveny azonositojanak modositasa.\n\nAdja meg melyik adatot szeretne modositani:\t"
#define TOSTARTSTRING                " Inditashoz nyomjon le egy billenytut.\n"
#define ADDNEWGUEST                  "\tUj Vendeg Jelentkezese\t\t\n"
#define MENUSEPARATOR                "----------------------------------------\n"
#define AREYOUSURETODELETE           "BIZTOS, HOGY TOROLNI AKAROD? (I/N)\t\t"
#define GUESTINFOCOMMENT             "%d vendeg van a listan."
#define NEWGUESTCOMMENT              "\n\nSikeres jelentkezes %d. jelentkezokent!\n"
#define DELETECOMMENT                "\n\nSikeresen torolte a jelentkezot!\n"
#define MODIFYCOMMENT                "\n\nSikeresen modositotta a jelentkezot!\n"
#define INFOFORGUEST                 "ENTER - Modositas\tt - Torles\t\t\n"
#define INFOFORNAVIGATION            "ENTER - Vissza a fomenube\t\n"
#define INFOFORNEWGUEST              "ENTER - Uj Vendeg Felvetele\t\n"
#define NFOCONTROL                   "\n\n\n--------------------------------\n\t   NAVIGACIO\n--------------------------------\n\t   UP  -  %c, w, W\n         DOWN  -  %c, s, S\n    Ugras fol  -  PGUP\n     Ugras le  -  PGDN\n      Kilepes  -  ESC\n--------------------------------\n"
#define ASKFORNAME                   "Adja meg a vendeg nevet:\t"
#define ASKFORNEWNAME                "\n\nAdja meg az uj nevet:\t\t"
#define ASKFOREMAIL                  "\nAdja meg a vendeg email cimet:\t"
#define ASKFORNEWEMAIL               "\n\nAdja meg az uj email cimet:\t\t"
#define ASKFOREVENTID                "\nAz esemeny azonositoja:\t\t"
#define ASKFORNEWEVENTID             "\n\nAdja meg az uj rendezveny kodot:\t\t"
#define NEWGUESTSTRING               "\nUJ VENDEG:\n----------\n"
#define CHANGENAMESTRING             "Nev modositasa!"
#define CHANGEEMAILSTRING            "Email cim modositasa!"
#define CHANGEEVENTSTRING            "rendezveny azonosito modositasa!"
#define FILEERROR                    "Hiba a fajl megnyitasnal!\n"
#define FNAME                        "database/guests.txt"
#define ADATE                        "2016/10/14"
#define BACK                         "\n\n\t\tVissza\t\t\t\n"
#define EVENTS                       "\t  Rendezvenyek\t\t\n"
#define GUESTS                       "\t    Vendegek\t\t\n"
#define INFORMATION                  "\t   Informacio\t\t\n"
#define QUIT                         "\t    Kilepes\t\t\n"
#define FOURNEWLINE                  "\n\n\n\n"
#define DATEFORMAT                   "%Y/%m/%d"
#define STRINGPLACENEWLINE           "%s\n"
#define LONGDECIMALSTRING            "%ld"
#define CLEAR                        "CLS"
#define TOREAD                       "r"
#define TOWRITE                      "w"
#define TOAPPEND                     "a"
#define CHANGENAME                   '1'
#define CHANGEEMAIL                  '2'
#define CHANGEEVENT                  '3'
#define NEWLINECH                    '\n'
#define TERMINATINGCHARCH            '\0'
#define TABULATORCH                  '\t'
#define GUESTSEPARATORCH             ';'
#define NEWLINE                      "\n"
#define TERMINATINGCHAR              "\0"
#define TABULATOR                    "\t"
#define GUESTSEPARATOR               ";"
#define SOMESTRING                   " "
#define EMPTYCHAR                    ' '
#define NO                           'N'
#define no                           'n'
#define YES                          'I'
#define yes                          'i'
#define VOID                         void
#define BOOL                         int
#define COLOR                        int
#define C_STRING                     char*
#define WELLDONE                   0
#define FALSE                      0
#define TRUE                       1
#define ESC                       27
#define UP                        72
#define DOWN                      80
#define KEY_w                    119
#define KEY_s                    115
#define KEY_t                    116
#define KEY_W                     87
#define KEY_S                     83
#define KEY_T                     84
#define ENTER                     13
#define SPACE                     32
#define PGUP                      73
#define PGDN                      81
#define NUMBEROFMENUOPTIONS        4
#define INMAINMENU                 0
#define INEVENTS                   1
#define INLISTING                  2
#define ININFO                     3
#define EXIT                       4
#define BASECOLOR                 15
#define SELECTCOLOR              207    //240
#define COMMENTCOLOR              12
#define INFOCOLOR                 14
#define GUESTOCOLOR               11
#define GUESTOCOLORSELECTED      203
#define UNFINISHEDCOLOR           12
#define UNFINISHEDSELECTEDCOLOR  192    //207   //244
#define PROGRAMNAMECOLOR          10
#define WELCOMECOLOR              10
#define STARTPOSFORLISTING         0
#define STARTPOSFORMENU            1
#define ERR                        0    //-
#define BYTE                     256
#define DATELENGTH                11
#define PAGEJUMP                  10
#define UPARROW                   24
#define DOWNARROW                 25
#define HUNDRED                  100
#define FILEOPENERRORWHILEAPPEND   4
#define FILEOPENERRORWHILEDELETE   3
#define FILEOPENERRORWHILECLEAR    2
#define TENTHNUMBERSYSTEM         10
#define NUMBEROFGUESTARGUMENTS     5
#define NEWGUESTOPTION             0
#define PLACEDATA                  0
#define NAMEDATA                   1
#define EMAILDATA                  2
#define EVENTDATA                  3
#define DATEDATA                   4
#define PLACEDATA                  0
#define NAMEDATA                   1
#define EMAILDATA                  2
#define EVENTIDDATA                3
#define DATEDATA                   4
#define MININDEXOFLISTING          0
#define MININDEXOFMENUOPTIONS      1
#define STEPSFORMENU               1
#define MODIFYNAME                 1
#define MODIFYEMAIL                2
#define MODIFYEVENTID              3
#define INFOOPTION                 2
#define EVENTSOPTION               0
#define ZERO                       0
#define ONE                        1
#define TWO                        2

#endif // MAKROS_H_INCLUDED
