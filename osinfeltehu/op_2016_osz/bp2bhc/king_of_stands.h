#ifndef KING_OF_STANDS_INCLUDED
#define KING_OF_STANDS_INCLUDED

#define DEFAULT_SIZE 50

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>

typedef struct Person{
	char *name;
	char *email;
}Person;

Person createPerson();
void freePerson(Person);
void writePersonToFile(Person, char*);
int countPeople(char*);
char* personToLine(Person);
int deleteFromFile(Person, char*);

void displayMenu();
void addPerson();
void modifyPerson();
void removePerson();
void createEvent();
void listEvent();
void deleteEvent();
char* readLine(FILE*);
long getInt(FILE*);
int eventExists(char*);

#endif
