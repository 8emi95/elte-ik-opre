/*
    Tárgy: Elte IK Operációs rendszerek 1. Beadandó
    Készítette: Sándor Balázs - AZA6NL
	Git: https://github.com/SandorBalazsHU/elte-ik-os-1

    Az eseményeket tároló adatszerkezetethez tartozó fügvények.
*/

//mallochoz szügséges
#include <stdlib.h>
//strcpy-hez
//#include <string.h>
#include "./Events.h"
#include "./Event.h"

//Új üres eseménytároló létrehozásához
struct Events* newEvents()
{
	struct Events* _this = malloc(sizeof(struct Events));
	_this->size = 0;
	_this->_realSize = defaultEventArraySize;
	Event**  _eventsArray = malloc(_this->_realSize * sizeof(Event*));
	_this->eventsArray = _eventsArray;
	return _this;
}


//Esemény hozzáadását teszi lehetőve az eseménytárolóhoz
/*	Ha a tömb betelt, akkor dupla méretűt deklarálunk, átmásoljuk az adatokat
 és a régit töröljük a C++ STD Vector mintájára.*/
struct Events* addEventToEvents(Events* events, Event* event)
{
	int i;
	
	if(events->_realSize > events->size)
	{
		events->eventsArray[events->size] = event;
		events->size++;
	}
	else
	{
		events->_realSize *= 2;
		Event**  _eventsArray = malloc(events->_realSize * sizeof(Event*));
		for(i = 0; i < events->size; ++i)
		{
			_eventsArray[i] = events->eventsArray[i];
		}

		/*Felszabadítjuk a címeket tároló tömböt,
		de az elemeket nem. Azok címeit az új tömb már tárolja.*/
		Event** arr = events->eventsArray;
		free(arr);

		//A helyérekerül az új nagyobb tömb címe.
		events->eventsArray = _eventsArray;
		
		//Elvégezzük a beszúrást
		events->eventsArray[events->size] = event;
		events->size++;
	}
	return events;
}


//Esemény törlése az eseménytárolóból
struct Events* deleteEventFromEvents(Events* events, Event* event)
{
	int i, l = 0;
	Event**  _eventsArray = malloc(events -> _realSize * sizeof(Event*));
	for(i = 0; i < events->size; ++i)
	{
		if(events->eventsArray[i] != event)
		{
			_eventsArray[l] = events->eventsArray[i];
		}
		else
		{
			l = i-1;
			freeEvent(events->eventsArray[i]);
		}
		++l;
	}
	Event** arr = events -> eventsArray;
	free(arr);
	events -> eventsArray = _eventsArray;
	events->size--;
	return events;
}

//Esemény törlése az eseménytárolóból
struct Events* deleteEventFromEventsById(Events* events, int id)
{
	int i, l = 0;
	Event**  _eventsArray = malloc(events -> _realSize * sizeof(Event*));
	for(i = 0; i < events->size; ++i)
	{
		if(i != id)
		{
			_eventsArray[l] = events->eventsArray[i];
		}
		else
		{
			l = i-1;
			freeEvent(events->eventsArray[i]);
		}
		++l;
	}
	Event** arr = events -> eventsArray;
	free(arr);
	events -> eventsArray = _eventsArray;
	events->size--;
	return events;
}

struct Event* getEventFromEventsById(Events* events, int id){
	return events->eventsArray[id];
}

/*Az Events típus destruktora
Kell destrukfor, mivel az eventArray külön álló egységet képez a memóriában
Törli a tömb által mutatott elemeket is!*/
void freeEvents(Events* events)
{
	int i;
	for(i = 0; i < events->size; ++i)
	{
		freeEvent(events->eventsArray[i]);
	}
	free(events->eventsArray);
	free(events);
}