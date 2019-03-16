/*
    Tárgy: Elte IK Operációs rendszerek 1. Beadandó
    Készítette: Sándor Balázs - AZA6NL
	Git: https://github.com/SandorBalazsHU/elte-ik-os-1

    Az eseményeket reprezentáló adatszerkezetethez tartozó fügvények.
*/

//mallochoz szügséges
#include <stdlib.h>
//strcpy-hez
#include <string.h>
#include "./Event.h"
#include "./Visitor.h"

//Új üres esemény
struct Event* newEvent(char* name)
{
	struct Event* _this = malloc(sizeof(struct Event));
	strcpy(_this->name, name);
	_this->size = 0;
	_this->_realSize = defaultArraySize;
	Visitor**  _visitorArray = malloc(_this->_realSize * sizeof(Visitor*));
	_this->visitorArray = _visitorArray;
	return _this;
}


//Látogató hozzáadását teszi lehetőve az eseményhez
/*	Ha a tömb betelt, akkor dupla méretűt deklarálunk, átmásoljuk az adatokat
 és a régit töröljük a C++ STD Vector mintájára.*/
int addVisitorToEvent(Event* event, Visitor* visitor)
{
	int i;
	
	if(event->_realSize > event->size)
	{
		event->visitorArray[event->size] = visitor;
		event->size++;
	}
	else
	{
		event->_realSize *= 2;
		Visitor**  _visitorArray = malloc(event->_realSize * sizeof(Visitor*));
		for(i = 0; i < event->size; ++i)
		{
			_visitorArray[i] = event->visitorArray[i];
		}

		/*Felszabadítjuk a címeket tároló tömböt,
		de az elemeket nem. Azok címeit az új tömb már tárolja.*/
		Visitor** arr = event->visitorArray;
		free(arr);

		//A helyérekerül az új nagyobb tömb címe.
		event->visitorArray = _visitorArray;
		
		//Elvégezzük a beszúrást
		event->visitorArray[event->size] = visitor;
		event->size++;
	}
	return 0;
}


//Látogató törlése az eseményről
struct Event* deleteVisitorFromEvent(Event* event, Visitor* visitor)
{
	int i, l = 0;
	Visitor**  _visitorArray = malloc(event -> _realSize * sizeof(Visitor*));
	for(i = 0; i < event->size; ++i)
	{
		if(event->visitorArray[i] != visitor)
		{
			_visitorArray[l] = event->visitorArray[i];
		}
		else
		{
			l = i-1;
			freeVisitor(event->visitorArray[i]);
		}
		++l;
	}
	Visitor** arr = event -> visitorArray;
	free(arr);
	event -> visitorArray = _visitorArray;
	event->size--;
	return event;
}

//Látogató törlése az eseményről
struct Event* deleteVisitorFromEventById(Event* event, int visitorId)
{
	int i, l = 0;
	Visitor**  _visitorArray = malloc(event -> _realSize * sizeof(Visitor*));
	for(i = 0; i < event->size; ++i)
	{
		if(visitorId != i)
		{
			_visitorArray[l] = event->visitorArray[i];
		}
		else
		{
			l = i-1;
			freeVisitor(event->visitorArray[i]);
		}
		++l;
	}
	Visitor** arr = event -> visitorArray;
	free(arr);
	event -> visitorArray = _visitorArray;
	event->size--;
	return event;
}


//Index szerinti keresés a látogatók között
struct Visitor* getVisitorFromEventById(Event* event, int id)
{
	return event->visitorArray[id];
}

//Visszaadja a látogatók számát.
int getVisitorsNumberFromEvent(Event* event)
{
	return event->size;
}

/*Az Event típus destruktora
Kell destrukfor, mivel az visitorArray külön álló egységet képez a memóriában
Törli a tömb által mutatott elemeket is!*/
void freeEvent(Event* event)
{
	int i;
	for(i = 0; i < event->size; ++i)
	{
		freeVisitor(event->visitorArray[i]);
	}
	free(event->visitorArray);
	free(event);
}