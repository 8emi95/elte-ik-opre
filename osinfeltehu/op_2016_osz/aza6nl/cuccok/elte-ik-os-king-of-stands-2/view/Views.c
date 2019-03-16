/*
    Tárgy: Elte IK Operációs rendszerek 1. Beadandó
    Készítette: Sándor Balázs - AZA6NL
    Leadás: 2016.11.07
	Git: https://github.com/SandorBalazsHU/elte-ik-os-king-of-stands

    A menünézetek megvalósításai.
    A nézetek részletezését lásd a Documentation.txt-ben.
*/
#include <unistd.h>
#include "Views.h"
#include "ConsoleIO.h"
#include "../model/Events.h"
#include "../simulation/Simulation.h"

int view1(Events* events)
{
    clearScrean();
    printIntro();
    if(events->size > 0) printEvents(events); else printMessage("Nincsenek események!");

    char* mainMenu[3];
    mainMenu[0] = "Jelentkezés         ";
    mainMenu[1] = "Adminisztráció      ";
    mainMenu[2] = "Szimuláció          ";
    mainMenu[3] = "Kilépés             ";
    printMenu(mainMenu, 4);

    char selectedMainMenuitem = menuGenerator("1234");

    switch(selectedMainMenuitem)
    {
        case '1' : view2(events);
        break;
        case '2' : view3(events);
        break;
        case '3' : view11(events);
        break;
        case '4' : return 0;
        break;
        default : errorMessage("Hiba a menüben!\n" );
    }
}

void view2(Events* events)
{
    clearScrean();
    printIntro();
    printHeader("Jelentkezés");
    if(events->size > 0)
    {
        printEvents(events);
        _clearInputBuffer();
        char name[100];
        readFromConsole(name, "Név");
        char email[100];
        readFromConsole(email, "E-mail");
        int eventID = readIntFromConsole("A választott rendezvény száma", events->size)-1;

        char* subMenu[1];
        subMenu[0] = "Mentés              ";
        subMenu[1] = "Mégse               ";
        printMenu(subMenu, 2);

        char selectedMainMenuitem = menuGenerator("12");

        switch(selectedMainMenuitem)
        {
            case '1' :
                addVisitorToEvent(getEventFromEventsById(events, eventID), newVisitor(name, email, getDate()));
                eventsWriterForBinFiles(events);
                printMessage("Jelentkezésed elmentettük!");
                sleep(sleepTime);
                view1(events);
            break;
            case '2' : view1(events);
            break;
            default : errorMessage("Hiba a menüben!\n" );
        }
    }
    else
    {
        printMessage("Nincsenek események!");
        sleep(sleepTime);
        view1(events);
    }
}

void view3(Events* events)
{
    clearScrean();
    printIntro();
    printHeader("Adminisztráció");
    if(events->size > 0) printEvents(events); else printMessage("Nincsenek események!");
    char* subMenu[4];
    subMenu[0] = "Vendéglista         ";
    subMenu[1] = "Rendezvényszervezés ";
    subMenu[2] = "Rendezvénytörlés    ";
    subMenu[3] = "Vissza              ";
    printMenu(subMenu, 4);

    char selectedMainMenuitem = menuGenerator("1234");

    switch(selectedMainMenuitem)
    {
        case '1' : view4(events);
        break;
        case '2' : view5(events);
        break;
        case '3' : view6(events);
        break;
        case '4' : view1(events);
        break;
        default : errorMessage("Hiba a menüben!\n" );
    }
}

void view4(Events* events)
{
    clearScrean();
    printIntro();
    printHeader("Vendéglista");
    if(events->size > 0)
    {
        printEvents(events);

        _clearInputBuffer();
        int eventId = readIntFromConsole("Mely rendezvény vendéglistályát szerené látni (ID)", events->size)-1;
        view7(events, eventId);
    }
    else
    {
        printMessage("Nincsenek események!");
        sleep(sleepTime);
        view3(events);
    }
}
void view5(Events* events)
{
    clearScrean();
    printIntro();
    printHeader("Rendezvényszervezés");
    _clearInputBuffer();

    char name[100];
    readFromConsole(name, "A létrehozandó rendezvény veve");
    char* subMenu[1];
    subMenu[0] = "Mentés              ";
    subMenu[1] = "Mégse               ";
    printMenu(subMenu, 2);

    char selectedMainMenuitem = menuGenerator("12");

    switch(selectedMainMenuitem)
    {
        case '1' : 
            eventsWriterForBinFiles(addEventToEvents(events, newEvent(name)));
            printMessage("Az új esemény létrejött!");
            sleep(sleepTime);
            view3(events);
        break;
        case '2' : view3(events);
        break;
        default : errorMessage("Hiba a menüben!\n" );
    }
}
void view6(Events* events)
{
    clearScrean();
    printIntro();
    printHeader("Rendezvénytörlés");
    if(events->size > 0)
    {
        printEvents(events);

        _clearInputBuffer();
        int id = readIntFromConsole("A törölni kívánt rendezvény ID-ja", events->size)-1;

        char* subMenu[1];
        subMenu[0] = "Törlés              ";
        subMenu[1] = "Mégse               ";
        printMenu(subMenu, 2);

        char selectedMainMenuitem = menuGenerator("12");

        switch(selectedMainMenuitem)
        {
            case '1' : 
                eventsWriterForBinFiles(deleteEventFromEventsById(events, id));
                printMessage("A kiválasztott rendezvényt törtöltük!");
                sleep(sleepTime);
                view3(events);
            break;
            case '2' : view3(events);
            break;
            default : errorMessage("Hiba a menüben!\n" );
        }
    }
    else
    {
        printMessage("Nincsenek események!");
        sleep(sleepTime);
        view3(events);
    }
}

void view7(Events* events, int eventId)
{
    clearScrean();
    printIntro();
    printHeader("Vendéglista");
    if(getEventFromEventsById(events, eventId)->size > 0)
    {
        printVisitors(events, eventId);

        char* subMenu[4];
        subMenu[0] = "Vendégmódosítás     ";
        subMenu[1] = "Vendégtörlés        ";
        subMenu[2] = "Vissza              ";
        printMenu(subMenu, 3);

        char selectedMainMenuitem = menuGenerator("123");

        switch(selectedMainMenuitem)
        {
            case '1' : view8(events, eventId);
            break;
            case '2' : view10(events, eventId);
            break;
            case '3' : view3(events);
            break;
            default : errorMessage("Hiba a menüben!\n" );
        }
    }
    else
    {
        printMessage("Nincsenek résztvevők az eseményen!");
        sleep(sleepTime);
        view3(events);
    }
}

void view8(Events* events, int eventId)
{
    clearScrean();
    printIntro();
    printHeader("Vendégmódosítás");
    printVisitors(events, eventId);

    _clearInputBuffer();
    int visitorId = readIntFromConsole("A módosítani kívánt vendég ID-ja", getEventFromEventsById(events, eventId)->size)-1;
    view9(events, eventId, visitorId);
}

void view9(Events* events, int eventId, int visitorId)
{
    clearScrean();
    printIntro();
    printHeader("Vendégmódosítás");
    printEvents(events);
    printVisitor(getVisitorFromEventById(getEventFromEventsById(events, eventId), visitorId));

    _clearInputBuffer();
    char name[100];
    readFromConsole(name, "Név");
    char email[100];
    readFromConsole(email, "Email");

    char* subMenu[1];
    subMenu[0] = "Mentés              ";
    subMenu[1] = "Mégse               ";
    printMenu(subMenu, 2);

    char selectedMainMenuitem = menuGenerator("12");

    switch(selectedMainMenuitem)
    {
        case '1' :
            modifyVisitor(getVisitorFromEventById(getEventFromEventsById(events, eventId), visitorId), name, email);
            eventsWriterForBinFiles(events);
            printMessage("A módosítást elmentettük!");
            sleep(sleepTime);
            view7(events, eventId);
        break;
        case '2' : view7(events, eventId);
        break;
        default : errorMessage("Hiba a menüben!\n" );
    }

}

void view10(Events* events, int eventId)
{
    clearScrean();
    printIntro();
    printHeader("Vendégtörlés");
    printVisitors(events, eventId);

    _clearInputBuffer();
    int visitorId = readIntFromConsole("A törölni kívánt vendég ID-ja", events->size)-1;

    char* subMenu[1];
    subMenu[0] = "Törlés              ";
    subMenu[1] = "Mégse               ";
    printMenu(subMenu, 2);

    char selectedMainMenuitem = menuGenerator("12");

    switch(selectedMainMenuitem)
    {
        case '1' : 
            deleteVisitorFromEventById(getEventFromEventsById(events, eventId), (int) (visitorId));
            eventsWriterForBinFiles(events);
            printMessage("A kiválasztott rendezvényt törtöltük!");
            sleep(sleepTime);
            view7(events, eventId);
        break;
        case '2' : view7(events, eventId);
        break;
        default : errorMessage("Hiba a menüben!\n" );
    }
}
void view11(Events* events)
{
    clearScrean();
    printIntro();
    if(events->size > 0)
    {
        printEvents(events);

        int i; for(i=0; i<events->size; i++) simulation(events, i);

        char* mainMenu[3];
        mainMenu[0] = "Vissza              ";
        printMenu(mainMenu, 1);

        char selectedMainMenuitem = menuGenerator("1");

        switch(selectedMainMenuitem)
        {
            case '1' : view1(events);
            break;
            default : errorMessage("Hiba a menüben!\n" );
        }
        _clearInputBuffer();
    }
    else
    {
        printMessage("Nincsenek résztvevők az eseményen!");
        sleep(sleepTime);
        view3(events);
    }
}