/*
    Tárgy: Elte IK Operációs rendszerek 1. Beadandó
    Készítette: Sándor Balázs - AZA6NL
	Git: https://github.com/SandorBalazsHU/elte-ik-os-1

    A fájlkezelésért felelős függvények
*/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "File.h"
#include "../model/Events.h"
#include "../model/Event.h"
#include "../model/Visitor.h"

void eventsWriterForBinFiles(Events* events)
{
    FILE* datafileForWrite = openBinFileForWrite(_dataFileName);
    fwrite(&events->size, sizeof(events->size), 1, datafileForWrite);
    int i;
    for(i = 0; i<events->size; ++i) eventWriterForBinFiles(getEventFromEventsById(events, i), datafileForWrite);
    fclose(datafileForWrite);
}

void eventWriterForBinFiles(Event* event, FILE* datafileForWrite)
{
    fwrite(&event->size, sizeof(event->size), 1, datafileForWrite);
    int nameSize = (int) strlen(event->name);
    fwrite(&nameSize, sizeof(nameSize), 1, datafileForWrite);
    fwrite(event->name, sizeof(char), nameSize+1, datafileForWrite);
    int i;
    for(i = 0; i<event->size; ++i)visitorWriterForBinFiles(getVisitorFromEventById(event, i), datafileForWrite);
}

void visitorWriterForBinFiles(Visitor* visitor, FILE* datafileForWrite)
{
    int nameSize = (int) strlen(visitor->name);
    fwrite(&nameSize, sizeof(nameSize), 1, datafileForWrite);
    fwrite(visitor->name, sizeof(char), nameSize+1, datafileForWrite);
    int emailSize = (int) strlen(visitor->email);
    fwrite(&emailSize, sizeof(emailSize), 1, datafileForWrite);
    fwrite(visitor->email, sizeof(char), emailSize+1, datafileForWrite);
    fwrite(&visitor->date, sizeof(&visitor->date), 1, datafileForWrite);
}

Events* eventsReaderForBinFiles()
{
    FILE* datafileForRead = openBinFileForRead(_dataFileName);
    Events* events = newEvents();
	if(getFileSize(datafileForRead) > 0)
	{
		int size;
		fread(&size,sizeof(size),1,datafileForRead);
		int i;
		for(i = 0; i<size; ++i) addEventToEvents(events, eventReaderForBinFiles(datafileForRead));
	}
    fclose(datafileForRead);
	return events;
}

Event* eventReaderForBinFiles(FILE* datafileForRead)
{
    int size;
    fread(&size,sizeof(size), 1, datafileForRead);
    int nameSize;
    fread(&nameSize,sizeof(nameSize), 1, datafileForRead);
    char name[nameSize+1];
    fread(name, sizeof(char), nameSize+1, datafileForRead);

    Event* event = newEvent(name);

    int i;
    for(i = 0; i<size; ++i)
    {
        addVisitorToEvent(event, visitorReaderForBinFiles(datafileForRead));
    }
    return event;
}

Visitor* visitorReaderForBinFiles(FILE* datafileForRead)
{
    int nameSize;
    fread(&nameSize,sizeof(nameSize), 1, datafileForRead);
    char name[nameSize+1];
    fread(name, sizeof(char), nameSize+1, datafileForRead);
    int emailSize;
    fread(&emailSize,sizeof(emailSize), 1, datafileForRead);
    char email[emailSize+1];
    fread(email, sizeof(char), emailSize+1, datafileForRead);
    time_t date;
    fread(&date, sizeof(date), 1, datafileForRead);

    Visitor* visitor = newVisitor(name, email, date);

    return visitor;
}

FILE* openBinFileForRead(const char* name)
{
	FILE* f;
	f=fopen(name,"rb");
	if (f==NULL)
	{
		f=fopen(name,"wb+");  if (f==NULL) printf("Nem sikerült létrehozni a filet!\n");
		fclose(f);
		f=fopen(name,"rb");   if (f==NULL) printf("Hiba az olvasásra nyitás során!\n");
	}
	return f;
}

FILE* openBinFileForWrite(const char* name)
{
	FILE* f;
	f=fopen(name,"wb+");
	if (f==NULL)
	{
		printf("Hiba az írásra nyitás során\n");
	}
	return f;
}

int getFileSize(FILE* f)
{
	fseek(f, 0, SEEK_END);
    int fileSize = ftell(f);
    rewind(f);
    return(fileSize);
}