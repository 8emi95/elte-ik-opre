/*
    Tárgy: Elte IK Operációs rendszerek 1. Beadandó
    Készítette: Sándor Balázs - AZA6NL
    Leadás: 2016.11.07
	Git: https://github.com/SandorBalazsHU/elte-ik-os-1

    A konzolműveletek megvalósítása.
*/
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "./ConsoleIO.h"
#include "../model/Event.h"
#include "../model/Events.h"

//Képernyő töröl
void clearScrean()
{
    int r = system("clear");
}

time_t getDate()
{
    return time(NULL);
}

//Kiírja a saját dátum reprezentációt a képernyőre
void datePrintOut(time_t date)
{
	struct tm tm = *localtime(&date);
	printf("%d-%d-%d %d:%d:%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

//Megadja a képernyő jelenlegi szélességét
int getConsoleWindowWidth()
{
	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_col;
}

//Kiírja a köszöntő szöveget
void printIntro()
{
	printf("\n");
	int windowCenter = (int) (getConsoleWindowWidth()/2);
	int spacerPosition = windowCenter-29-1;
	char spacer[spacerPosition+2];
	spacerGenerator(spacer, ' ', spacerPosition);
	int linePosition = windowCenter-30-1;
	char line[linePosition+2];
	spacerGenerator(line, '-', linePosition);

	printf("%s%s+---------------------------------------------------------+%s\n", spacer, FontGreen, ColorClear);
	printf("+%s%s|  \"King of Stands\" elektronikus látogatói nyilvántartás  |%s%s+\n", line, FontGreen, ColorClear, line);
	printf("%s%s+---------------------------------------------------------+%s\n", spacer, FontGreen, ColorClear);

}

void printHeader(char* headerText)
{
	printf("\n");
	int position = (int) (getConsoleWindowWidth()/2) - (_strlenForUnicode(headerText)/2) - 4 - 1;
	char spacer[position+2];
	spacerGenerator(spacer, ' ', position);
	printf("%s<-| %s%s%s |->\n", spacer, FontGreen, headerText, ColorClear);
	printf("\n");
}

void printMessage(char* message)
{
	printf("\n");
	int position = (int) (getConsoleWindowWidth()/2) - (_strlenForUnicode(message)/2) - 4 - 1;
	char spacer[position+2];
	spacerGenerator(spacer, ' ', position);
	printf("%s<-|%s%s %s %s|->\n", spacer, FontBlack, FontBackYellow, message, ColorClear);
	printf("\n");
}

//A menü kirajzolása
void printMenu(char** menuItems, int menuItemsNumber)
{
	int i;
	for(i = 0; i < menuItemsNumber; ++i)
	{
	printf("          +--------------------+ \n");
	printf("      %s%i.)%s |%s| \n", FontGreen, (i+1), ColorClear, menuItems[i]);
	printf("          +--------------------+ \n");
	}
	printf("      %s+%s Valassz funkciot: ", FontGreen, ColorClear);
}

/*A menük logikája
  Tetszőleges menü geráltatható vele.
  Meg kell adni egy karaktertömböt amiben egy karakter egy heles
  választási lehetőséget jelent a menüben.*/
char menuGenerator(char* menuItems)
{
	char selectedMenuItem = '~';
	short int state = 0;
	short int isCorrect = 0;
	int i;
	while( isCorrect == 0 )
	{
		if ((selectedMenuItem != '~') && ( selectedMenuItem != '\n') && ( state == 0))
		{
			printf("      + %s%sHibás érték!%s Add meg újra: ", FontWhite, FontBackRed, ColorClear);
			state++;
		}
		selectedMenuItem = getchar();

		if(selectedMenuItem == '\n') state = 0;

		for(i = 0; i < strlen(menuItems); ++i)
		{
			if(menuItems[i] == selectedMenuItem)
			{
				isCorrect = 1;
				break;
			}
		}
	}
	return selectedMenuItem;
}

void printEvents(Events* events)
{
	if(events->size > 0)
	{
		int i;
		int fullWidth = 0;
		int fullNameWidth = 10;

		for(i=0; i<events->size; ++i)
		{
			int len = _strlenForUnicode(events->eventsArray[i]->name);
			if(len>fullNameWidth) fullNameWidth = len;
		}
		fullWidth = (int) fullNameWidth + 30;

		int spacerWidth = (int) (getConsoleWindowWidth() - fullWidth)/2;
		char spacer[spacerWidth+2];
		spacerGenerator(spacer, ' ', spacerWidth);

		char nameLine[fullNameWidth+4];
		spacerGenerator(nameLine, '-', (int) (fullNameWidth+2));
		
		printf("%s%s+%s-----%s+%s%s%s+%s------------------%s+%s\n", spacer, FontGreen, ColorClear, FontGreen, ColorClear, nameLine, FontGreen, ColorClear, FontGreen, ColorClear);
		char nameSpacer[fullNameWidth-7+2];
		spacerGenerator(nameSpacer, ' ', fullNameWidth-7);
		printf("%s| Id  | Esemény%s | Résztvevők száma |\n", spacer, nameSpacer);
		printf("%s%s+%s-----%s+%s%s%s+%s------------------%s+%s\n", spacer, FontGreen, ColorClear, FontGreen, ColorClear, nameLine, FontGreen, ColorClear, FontGreen, ColorClear);
		
		for(i=0; i<events->size; ++i)
		{
			Event* tmpEvent = getEventFromEventsById(events, i);
			char nameSpacer[fullNameWidth-2-strlen(tmpEvent->name)+2];
			spacerGenerator(nameSpacer, ' ', fullNameWidth-2-strlen(tmpEvent->name)+2);
			char eventSizeSpacer[17-numPlaces(tmpEvent->size)+2];
			spacerGenerator(eventSizeSpacer, ' ', 17-numPlaces(tmpEvent->size));
			char idSpacer[4-numPlaces(i+1)+2];
			spacerGenerator(idSpacer, ' ', 4-numPlaces(i+1));
			printf("%s| %i%s| %s%s | %i%s|\n", spacer, i+1, idSpacer, tmpEvent->name, nameSpacer, tmpEvent->size, eventSizeSpacer);
			printf("%s%s+%s-----%s+%s%s%s+%s------------------%s+%s\n", spacer, FontGreen, ColorClear, FontGreen, ColorClear, nameLine, FontGreen, ColorClear, FontGreen, ColorClear);
		}
	}
	else
	{
		printMessage("Nincsenek események!");
	}
}

void printVisitors(Events* events, int eventId)
{
	int i;
	int fullWidth = 0;
	int fullNameWidth = 0;
	int fullEmailWidth = 0;

	for(i=0; i<events->eventsArray[eventId]->size; ++i)
	{
		int len = _strlenForUnicode(events->eventsArray[eventId]->visitorArray[i]->name);
		if(len>fullNameWidth) fullNameWidth = len;
	}

	for(i=0; i<events->eventsArray[eventId]->size; ++i)
	{
		int len = _strlenForUnicode(events->eventsArray[eventId]->visitorArray[i]->email);
		if(len>fullEmailWidth) fullEmailWidth = len;
	}

	fullWidth = (int) fullNameWidth + fullEmailWidth + 30;

	int spacerWidth = (int) (getConsoleWindowWidth() - fullWidth)/2;
	char spacer[spacerWidth+2];
	spacerGenerator(spacer, ' ', spacerWidth);

	char nameLine[fullNameWidth+4];
	spacerGenerator(nameLine, '-', (int) (fullNameWidth+2));
	char emailLine[fullEmailWidth+4];
	spacerGenerator(emailLine, '-', (int) (fullEmailWidth+2));
	
	printf("%s%s+%s A/Az [%s] eseményre jelentkezett vendégek: \n", spacer, FontGreen, ColorClear, events->eventsArray[eventId]->name);
	printf("%s%s+%s-----%s+%s%s%s+%s%s%s+%s---------------------%s+%s\n", spacer, FontGreen, ColorClear, FontGreen, ColorClear,  nameLine, FontGreen, ColorClear, emailLine, FontGreen, ColorClear, FontGreen, ColorClear);
	for(i=0; i<events->eventsArray[eventId]->size; ++i)
	{
		Visitor* tmpVisitor =  getVisitorFromEventById(getEventFromEventsById(events, eventId), i);

		char nameSpacer[fullNameWidth-strlen(tmpVisitor->name)+2];
		spacerGenerator(nameSpacer, ' ', fullNameWidth-strlen(tmpVisitor->name));
		char emailSpacer[fullEmailWidth-strlen(tmpVisitor->email)+2];
		spacerGenerator(emailSpacer, ' ', fullEmailWidth-strlen(tmpVisitor->email));
		char idSpacer[4-numPlaces(i+1)+2];
		spacerGenerator(idSpacer, ' ', 4-numPlaces(i+1));

		printf("%s| %i%s| %s%s | %s%s | ", spacer, i+1, idSpacer, tmpVisitor->name, nameSpacer, tmpVisitor->email, emailSpacer);
		datePrintOut(tmpVisitor->date);
		printf(" |\n");
		printf("%s%s+%s-----%s+%s%s%s+%s%s%s+%s---------------------%s+%s\n", spacer, FontGreen, ColorClear, FontGreen, ColorClear,  nameLine, FontGreen, ColorClear, emailLine, FontGreen, ColorClear, FontGreen, ColorClear);
	}
}

void printVisitor(Visitor* visitor)
{
	int i;
	int fullWidth = 0;
	int fullNameWidth = 0;
	int fullEmailWidth = 0;

	fullNameWidth = _strlenForUnicode(visitor->name);
	fullEmailWidth = _strlenForUnicode(visitor->email);
	fullWidth = (int) fullNameWidth + fullEmailWidth + 30;

	int spacerWidth = (int) (getConsoleWindowWidth() - fullWidth)/2;
	char spacer[spacerWidth+2];
	spacerGenerator(spacer, ' ', spacerWidth);
	char nameLine[fullNameWidth+4];
	spacerGenerator(nameLine, '-', (int) (fullNameWidth+2));
	char emailLine[fullEmailWidth+4];
	spacerGenerator(emailLine, '-', (int) (fullEmailWidth+2));
	
	printf("%s%s+%s A/Az [%s] látogató adatai: \n", spacer, FontGreen, ColorClear, visitor->name);
	printf("%s%s+%s%s%s+%s%s%s+%s---------------------%s+%s\n", spacer, FontGreen, ColorClear,  nameLine, FontGreen, ColorClear, emailLine, FontGreen, ColorClear, FontGreen, ColorClear);

	char nameSpacer[fullNameWidth-strlen(visitor->name)+2];
	spacerGenerator(nameSpacer, ' ', fullNameWidth-strlen(visitor->name));
	char emailSpacer[fullEmailWidth-strlen(visitor->email)+2];
	spacerGenerator(emailSpacer, ' ', fullEmailWidth-strlen(visitor->email));
	
	printf("%s| %s%s | %s%s | ", spacer, visitor->name, nameSpacer, visitor->email, emailSpacer);
	datePrintOut(visitor->date);
	printf(" |\n");
	printf("%s%s+%s%s%s+%s%s%s+%s---------------------%s+%s\n", spacer, FontGreen, ColorClear,  nameLine, FontGreen, ColorClear, emailLine, FontGreen, ColorClear, FontGreen, ColorClear);
}

/*
	Megadott hosszúságú sort generál egy karakterből és a hosszából.
	FIGYELEM: A szügségesnél 2-vel nagyobb tömböt kell deklarálni számára.
	-Paraméterek: +
				  |-spacer: A tömb amivel dolgozni fog (Mérete N+2 legyen!)
				  |-c: A karakter amiből a sort geneálja
				  |-n: A kívánt sor hossza.
*/
void spacerGenerator(char* spacer, char c, int n)
{
	int i;
	for(i = 0; i < n; ++i)
	{
		spacer[i] = c;
	}
	spacer[n] = '\0';
	spacer[n+1] = '\0';
}

int _strlenForUnicode(char* s)
{
   int i = 0, j = 0;
   while (s[i]) {
     if ((s[i] & 0xc0) != 0x80) j++;
     i++;
   }
   return j;
}

void errorMessage(char* errorMessageText)
{
	printf("\n");
	printf("+ %s%sHiba%s: %s\n", FontWhite, FontBackRed, ColorClear, errorMessageText);
	printf("\n");
}

void simulationMessage(char* message)
{
	printf("     %s+%s %s\n", FontGreen, ColorClear, message);
}

void simulationStateMessage(char* name, int pid, char* message)
{
	printf("+ %s%s[%s]>%i:%s %s\n", FontBlack, FontBackYellow, name, pid, ColorClear, message);
}

char* readFromConsole(char* inputString, char* label)
{
	printf("\n");
	printf("%s+ %s: %s",FontGreen, label, ColorClear);
	int i = 0;
	char c;
	while ((c = getchar()) != '\n' && c != EOF)
	{
		inputString[i] = c;
		++i;
	}
	inputString[i] = '\0';
	printf("\n");
	return inputString;
}
int readIntFromConsole(char* label, int range)
{
	char inputString[100];
	readFromConsole(inputString, label);
	int readedNumber = 0;
	char *garbage = NULL;
	readedNumber = strtol(inputString, &garbage, range+1);
	while(readedNumber == 0)
	{
		errorMessage("Hibás bemenet! Add meg újra!");
		readFromConsole(inputString, label);
		readedNumber = strtol(inputString, &garbage, range+1);
	}
	return readedNumber;
}

// works only if the input buffer is not empty
void _clearInputBuffer()
{
	char c;
    do 
    {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

//Hogy ne kelljen logaritmust számolni.
int numPlaces (int n)
{
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    return 10;
}