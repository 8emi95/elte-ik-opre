/*
    Tárgy: Elte IK Operációs rendszerek 1. Beadandó
    Készítette: Sándor Balázs - AZA6NL
    Leadás: 2016.11.07
	Git: https://github.com/SandorBalazsHU/elte-ik-os-1

    A konzolműveletek megvalósítása.
*/
#ifndef CONSOLEIO_H
#define CONSOLEIO_H

    #include <time.h>
    #include "../model/Events.h"

    //Lehetséges betűszínek
    #define FontBlack           "\x1B[30m"
    #define FontRed             "\x1B[31m"
    #define FontGreen           "\x1B[32m"
    #define FontYellow          "\x1B[33m"
    #define FontBlue            "\x1B[34m"
    #define FontPurple          "\x1B[35m"
    #define FontCyan            "\x1b[36m"
    #define FontWhite           "\x1B[37m"

    //Lehetséges háttérszínek
    #define FontBackBlack        "\x1b[40m"
    #define FontBackRed          "\x1b[41m"
    #define FontBackGreen        "\x1b[42m"
    #define FontBackYellow       "\x1b[43m"
    #define FontBackBlue         "\x1b[44m"
    #define FontBackPurple       "\x1b[45m"
    #define FontBackCyan         "\x1b[46m"
    #define FontBackWhite        "\x1b[47m"

    //Színezés vége
    #define ColorClear           "\x1B[0m"

    //Képernyő töröl
    void clearScrean();

    //Lekérdezi a dátumot
    time_t getDate();

    //Kiírja a saját dátum reprezentációt a képernyőre
    void datePrintOut(time_t date);

    //Megadja a képernyő jelenlegi szélességét
    int getConsoleWindowWidth();

    //Kiírja a köszöntő szöveget
    void printIntro();

    //A menü kirajzolása
    void printMenu(char**, int);

    /*A menük logikája
      Tetszőleges menü geráltatható vele.
      Meg kell adni egy karaktertömböt amiben egy karakter egy heles
      választási lehetőséget jelent a menüben.*/
    char menuGenerator(char*);

    //Kilistázza a paraméterrül kapott EVENTS ből az eseményeket táblázatos formában.ú
    void printEvents(Events* event);

    void printVisitors(Events*, int);
    void printVisitor(Visitor*);

    //Állandó karaktersor generátor
    void spacerGenerator(char*, char, int);

    int _strlenForUnicode(char*);

    void errorMessage(char*);

    void printHeader(char*);

    char* readFromConsole(char*, char*);

    void _clearInputBuffer();

    int numPlaces (int n);

    int readIntFromConsole(char*, int);

    void printMessage(char*);

    void simulationStateMessage(char*, int, char*);
    
    void simulationMessage(char*);
#endif