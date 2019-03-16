/*
    Tárgy: Elte IK Operációs rendszerek 1. Beadandó
    Készítette: Sándor Balázs - AZA6NL
	Git: https://github.com/SandorBalazsHU/elte-ik-os-1

    A látogatókat reprezentáló adatszerkezetethez tartozó fügvények.
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "./Tests.h"
#include "../view/ConsoleIO.h"
#include "../model/Events.h"
#include "../model/Event.h"
#include "../model/Visitor.h"
#include "../controller/File.h"

//A teszteket egyszerre futtató függvény.
int test(int argc, char** argv)
{
    if(argc > 1)
    {
        if(strcmp(argv[1], testConsoleParameter) == 0)
        {
        //UI tesztek
            //Képernyőtörlés
            clearScrean();

            //A konzol színezését teszteli
            consoleColorsTest();

        //Adatszerkezetek tesztjei
            //A visitor adatszerkezet tesztje.
            testVisitor();

            //Az Event adatszerkezet tesztje kevesebb mint 5 elemre.
            testEvent( 3 );

            //Az Event adatszerkezet tesztje több mint 5 elemre.
            testEvent( 9 );

            //Az Events adatszerkezet tesztje kevesebb mint 5 elemre.
            testEvents( 3, 3 );

            //Az Events adatszerkezet tesztje több mint 5 elemre.
            testEvents( 8, 3 );

            //Az event listázó függvény tesztje
            testPrintEvents( 10, 20 );

            testPrintVisitors(11);

            return 1;
        }
    }
    return 0;
}

int consoleColorsTest()
{
    printf("\n-consoleColorsTest-+\n");
    printf("                   |-> %s%sFontBlack%s\n",     FontBackWhite, FontBlack,   ColorClear);
    printf("                   |-> %sFontRed%s\n",         FontRed,       ColorClear);
    printf("                   |-> %sFontGreen%s\n",       FontGreen,     ColorClear);
    printf("                   |-> %sFontYellow%s\n",      FontYellow,    ColorClear);
    printf("                   |-> %sFontPurple%s\n",      FontPurple,    ColorClear);
    printf("                   |-> %sFontCyan%s\n",        FontCyan,      ColorClear);
    printf("                   |-> %sFontWhite%s\n",       FontWhite,     ColorClear);
    printf("\n-consoleColorsTest-+\n");
    printf("                   |-> %sFontBackBlack%s\n",   FontBackBlack, ColorClear);
    printf("                   |-> %sFontBackRed%s\n",     FontBackRed,   ColorClear);
    printf("                   |-> %sFontBackGreen%s\n",   FontBackGreen, ColorClear);
    printf("                   |-> %sFontBackYellow%s\n",  FontBackYellow,ColorClear);
    printf("                   |-> %sFontBackBlue%s\n",    FontBackBlue,  ColorClear);
    printf("                   |-> %sFontBackPurple%s\n",  FontBackPurple,ColorClear);
    printf("                   |-> %sFontBackCyan%s\n",    FontBackCyan,  ColorClear);
    printf("                   |-> %s%sFontBackWhite%s\n", FontBlack,   FontBackWhite, ColorClear);
}

//A visitor adatszerkezet tesztje.
int testVisitor()
{
    printf("\n-testVisitor-+\n");

    Visitor* visitor1 = newVisitor("visitor1", "visitor1@mail.org", getDate());
    printf("             |-> newVisitor - %sOK%s\n", FontGreen, ColorClear);

    freeVisitor(visitor1);
    printf("             |-> freeVisitor - %sOK%s\n", FontGreen, ColorClear);
}

//Az Event adatszerkezet tesztje.
int testEvent(int n)
{
    printf("\n-testEvent---%i--+\n", n);
    Event* event1 = newEvent("event1");
    printf("                |-> newEvent - %sOK%s\n", FontGreen, ColorClear);
    int i;
        for(i = 0; i < n; ++i)
        {
            Visitor* visitor = newVisitor("visitor", "visitor@mail.org", getDate());
            printf("                |-> newVisitor %i - %sOK%s\n", (i+1), FontGreen, ColorClear);
            addVisitorToEvent(event1, visitor);
            printf("                |-> addVisitorToEvent %i - %sOK%s\n", (i+1), FontGreen, ColorClear);
        }
    int getVisitorsNumberFromEventTestValue = getVisitorsNumberFromEvent(event1);
    if(getVisitorsNumberFromEventTestValue == n)
    {
        printf("                |-> getVisitorsNumberFromEvent= %i == %i - %sOK%s\n", getVisitorsNumberFromEventTestValue, n, FontGreen, ColorClear);
    }
    else
    {
        printf("                |-> getVisitorsNumberFromEvent= %i =/= %i - %sFALSE%s\n", getVisitorsNumberFromEventTestValue, n, FontRed, ColorClear);
    }
    freeEvent(event1);
    printf("                |-> freeEvent - %sOK%s\n", FontGreen, ColorClear);
}

//Az Events adatszerkezet tesztje.
int testEvents(int n, int m)
{
    printf("\n-testEvents---%i-%i-+\n", n, m);

    Events* events = newEvents();
    printf("                  |-> newEvents - %sOK%s \n", FontGreen, ColorClear);

    int i;
    for(i = 0; i < n; ++i)
    {
        Event* event = newEvent("event");
        printf("                  |-> newEvent-%i---%sOK%s-+ \n", (i+1), FontGreen, ColorClear);
        int j;
        for(j = 0; j < m; ++j)
        {
            Visitor* visitor = newVisitor("visitor", "visitor@mail.org", getDate());
            printf("                  |                   |-> newVisitor %i - %sOK%s\n", (j+1), FontGreen, ColorClear);
            addVisitorToEvent(event, visitor);
            printf("                  |                   |-> addVisitorToEvent %i - %sOK%s\n", (j+1), FontGreen, ColorClear);
        }
        addEventToEvents(events, event);
        printf("                  |-> addEventToEvents %i - %sOK%s \n", (i+1), FontGreen, ColorClear);
    }

    freeEvents(events);
    printf("                  |-> freeEvents - %sOK%s \n", FontGreen, ColorClear);
}

int testPrintEvents(int n, int m)
{
    printf("\n-testPrintEvents---%i-%i-+\n", n, m);
    Events* events = newEvents();
    int i;
    srand(time(NULL));
    for(i = 0; i < n; ++i)
    {
        int randomStringLenght = 1+rand()%50;
        char randomString[randomStringLenght+2];
		spacerGenerator(randomString, '*', randomStringLenght);
        Event* event = newEvent(randomString);
        int j;
        int randomVisitorNumber = rand()%m;
        for(j = 0; j < randomVisitorNumber; ++j)
        {
            Visitor* visitor = newVisitor("visitor", "visitor@mail.org", getDate());
            addVisitorToEvent(event, visitor);
        }
        addEventToEvents(events, event);
    }
    printEvents(events);
    freeEvents(events);
}

int testPrintVisitors(int n)
{
    srand(time(NULL));
    printf("\n-testPrintVisitors---%i-+\n", n);
    Events* events = newEvents();
    Event* event = newEvent("árvíztűrőtükörfúrógép - ÁRVÍZTŰRŐTÜKÖRFÚRÓGÉP");
    int i;
    for(i = 0; i < n; ++i)
    {
        int randomNameLenght = 1+rand()%50;
        char randomName[randomNameLenght+2];
        spacerGenerator(randomName, '*', randomNameLenght);
        int randomEmailLenght = 1+rand()%20;
        char randomEmail[randomEmailLenght+2];
        spacerGenerator(randomEmail, '*', randomEmailLenght);
        //strcat(randomEmail,"@mail.org");

        Visitor* visitor = newVisitor(randomName, randomEmail, getDate());
        addVisitorToEvent(event, visitor);
    }
    addEventToEvents(events, event);
    printVisitors(events, 0);
    freeEvents(events);
}