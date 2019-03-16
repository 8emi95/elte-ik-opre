#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct Registrations {
	int id;
	char name[100];
	char email[100];
	int eventID;
	char time[20];
} Registration;
	
typedef struct Events {
	char name[100];
	int id;
} Event;

void printMenu();
char getKey();
char* getLine(size_t, char*);

Event* readEvents(FILE*, int*, int*, int*);
void writeEvents(FILE*, Event*, const int*, const int*);
void printEvents(Event*, const int*);
void addEvent(Event**, int*, int*, char*, int*);
int findEventById(Event*, const int*, const int);
void removeEvent(Event*, int*, const int, Registration*, int*);

Registration* readRegistrations(FILE*, int*, int*, int*);
void writeRegistrations(FILE*, Registration*, const int*, const int*);
void printRegistrations(Registration*, const int*);
void addRegistration(Registration**, int*, int*, char*, char*, int, int*);
void editRegistration(Registration*, const int*, char*, char*, const int);
void removeRegistration(Registration*, int*, const int);
int findRegistrationById(Registration*, const int*, const int);

int main(int argc, char* argv[])
{
	if(argc != 2) {
		printf("Usage: %s \"<storage file name>\"\n", argv[0]);
		exit(-1);
	}
	
	FILE *storageFile = fopen(argv[1], "rb");
	
	int *numberOfRegistrations, *allocatedRegistrations, *registrationIncrementer, *numberOfEvents, *allocatedEvents, *eventIncrementer;
	
	numberOfRegistrations = malloc(sizeof(int));
	allocatedRegistrations = malloc(sizeof(int));
	registrationIncrementer = malloc(sizeof(int));
	
	numberOfEvents = malloc(sizeof(int));
	allocatedEvents = malloc(sizeof(int));
	eventIncrementer = malloc(sizeof(int));
	
	Registration *registrations;
	Event *events;
	
	if(storageFile == 0) {
		printf("The file \"%s\" does not exist!\nProceeding without preloaded data...\n", argv[1]);
		registrations = malloc(sizeof(Registration));
		events = malloc(sizeof(Event));
		
		*numberOfRegistrations = 0;
		*allocatedRegistrations = 1;
		*registrationIncrementer = 1;
		
		*numberOfEvents = 0;
		*allocatedEvents = 1;
		*eventIncrementer = 1;
	} else {
		printf("Reading data from \"%s\"\n", argv[1]);
		
		events = readEvents(storageFile, numberOfEvents, allocatedEvents, eventIncrementer);
		printf("Reading events completed -- # of events read: %d [allocated: %d]\n", *numberOfEvents, *allocatedEvents);
		
		registrations = readRegistrations(storageFile, numberOfRegistrations, allocatedRegistrations, registrationIncrementer);
		printf("Reading registrations completed -- # of registrations read: %d [allocated: %d]\n", *numberOfRegistrations, *allocatedRegistrations);
		
		fclose(storageFile);
	}
	
	char* command;
	char key;
	while(key != '0') {
		printf("\nEvents: %d/%d Registrations: %d/%d Incrementers: event: %d - registrations: %d\n", *numberOfEvents, *allocatedEvents, *numberOfRegistrations, *allocatedRegistrations, *eventIncrementer, *registrationIncrementer);
		printMenu();
		command = getLine(100, "Command: ");
		if(strlen(command) != 1)
			continue;
		
		key = command[0];
		printf("\n");
		switch(key) {
			case '1': {
				printEvents(events, numberOfEvents);
				break;
			}
			case '2': {
				printRegistrations(registrations, numberOfRegistrations);
				break;
			}
			case '3': {
				addEvent(&events, numberOfEvents, allocatedEvents, getLine(100, "Event: "), eventIncrementer);
				break;
			}
			case '4': {
				char *id;
				removeEvent(events, numberOfEvents, atoi(id = getLine(1, "ID of the event to remove: ")), registrations, numberOfRegistrations);
				free(id);
				break;
			}
			case '5': {
				char *id;
				if(findEventById(events, numberOfEvents, atoi(id = getLine(1, "ID of the Event to registrate into: "))) == -1) {
					printf("Event with that ID does not exist!\n");
				} else {
					addRegistration(&registrations, numberOfRegistrations, allocatedRegistrations, getLine(100, "Name: "), getLine(100, "Email: "), atoi(id), registrationIncrementer);
				}
				
				free(id);
				break;
			}
			case '6': {
				char *id;
				
				if(findRegistrationById(registrations, numberOfRegistrations, atoi(id = getLine(1, "ID of the Registration to edit: "))) == -1) {
					printf("Registration with that ID does not exist!\n");
				} else {
					editRegistration(registrations, numberOfRegistrations, getLine(100, "Name: "), getLine(100, "Email: "), atoi(id));
				}
				
				free(id);
				break;
			}
			case '7': {
				char *id;
				removeRegistration(registrations, numberOfRegistrations, atoi(id = getLine(1, "ID of the registration to remove: ")));
				free(id);
			}
			default:
				break;
		}
		free(command);
		command = NULL;
	}
	
	storageFile = fopen(argv[1], "wb");
	
	writeEvents(storageFile, events, numberOfEvents, eventIncrementer);
	writeRegistrations(storageFile, registrations, numberOfRegistrations, registrationIncrementer);
	
	free(numberOfEvents);
	free(allocatedEvents);
	free(eventIncrementer);
	free(events);
	
	free(numberOfRegistrations);
	free(allocatedRegistrations);
	free(registrationIncrementer);
	free(registrations);
	
    return 0; 
}

void printMenu() {
	printf("List Events - 1\nList Registrations - 2\nAdd Event - 3\nRemove Event [with Registrations] - 4\nAdd Registration - 5\nEdit Registration - 6\nRemove Registration - 7\nQuit - 0\n");
}

char* getLine(size_t size, char *str) {
	printf("%s", str);
	
	char *result = malloc(sizeof(char)*size);
	char *walker = result;
	
	char c;
	while((c = getchar()) != '\n') {
		*walker++ = c;
	}
	*walker = '\0';
	return result;
}

//EVENT

Event* readEvents(FILE *sourceFile, int *eventsRead, int *eventsAllocated, int *eventIncrementer) {	
	fread(eventsRead, sizeof(int), 1, sourceFile);
	fread(eventIncrementer, sizeof(int), 1, sourceFile);
	
	*eventsAllocated = (*eventsRead == 0) ? 1 : (*eventsRead * 2);
	
	Event *events = malloc(2 * *eventsRead * sizeof(Event));
	
	fread(events, sizeof(Event), *eventsRead, sourceFile);
	
	return events;
}

void writeEvents(FILE *targetFile, Event *events, const int *numberOfEvents, const int *eventIncrementer) {
	fwrite(numberOfEvents, sizeof(int), 1, targetFile);
	fwrite(eventIncrementer, sizeof(int), 1, targetFile);
	fwrite(events, sizeof(Event), *numberOfEvents, targetFile);
}

void printEvents(Event *events, const int *numberOfEvents) {
	int i;
	printf("\nID\tName\n");
	for(i = 0; i<*numberOfEvents; ++i) {
		printf("%d\t%s\n", events[i].id, events[i].name);
	}
	printf("\n");
}

void addEvent(Event **events, int *numberOfEvents, int *allocatedEvents, char *name, int* incrementer) {
	(*events)[*numberOfEvents].id = (*incrementer)++;
	strcpy((*events)[*numberOfEvents].name, name);
	
	*numberOfEvents += 1;
	printf("Event registration complete -- # of events: %d [allocated: %d]\n", *numberOfEvents, *allocatedEvents);
	if(*allocatedEvents == *numberOfEvents){
		Event *newAllocation = malloc(2 * *numberOfEvents * sizeof(Event));
		*allocatedEvents = 2 * *numberOfEvents;
		
		memcpy(newAllocation, (*events), *numberOfEvents * sizeof(Event));
		
		free(*events);
		(*events) = NULL;
		(*events) = newAllocation;
	}
	
	free(name);
	name = NULL;
}

int findEventById(Event *events, const int *numberOfEvents, const int id) {
	int i;
	for(i = 0; i<*numberOfEvents; ++i) {
		if(events[i].id == id) {
			return i;
		}
	}
	return -1;
}

void removeEvent(Event *events, int *numberOfEvents, const int id, Registration *registrations, int *numberOfRegistrations) {
	int index = findEventById(events, numberOfEvents, id);
	
	if(index == -1) {
		printf("Event with that ID does not exists!\n");
		return;
	}
	
	int i;
	for(i = 0; i<*numberOfRegistrations; ++i) {
		if(registrations[i].eventID == id) {
			registrations[i--] = registrations[--(*numberOfRegistrations)];
		}
	}
	
	events[index] = events[--(*numberOfEvents)];
}

//REGISTRATION

Registration* readRegistrations(FILE *sourceFile, int *registrationsRead, int *registrationsAllocated, int *registrationIncrementer) {	
	fread(registrationsRead, sizeof(int), 1, sourceFile);
	fread(registrationIncrementer, sizeof(int), 1, sourceFile);
	
	*registrationsAllocated = (*registrationsRead == 0) ? 1 : (*registrationsRead * 2);
	
	Registration *registrations = malloc(2 * *registrationsRead * sizeof(Registration));
	
	fread(registrations, sizeof(Registration), *registrationsRead, sourceFile);
	
	return registrations;
}

void writeRegistrations(FILE *targetFile, Registration *registrations, const int *numberOfRegistrations, const int *registrationIncrementer) {
	fwrite(numberOfRegistrations, sizeof(int), 1, targetFile);
	fwrite(registrationIncrementer, sizeof(int), 1, targetFile);
	fwrite(registrations, sizeof(Registration), *numberOfRegistrations, targetFile);
}

void printRegistrations(Registration *registrations, const int *numberOfRegistrations) {
	printf("List of Registrations:\n");
	int i;
	for(i = 0; i<*numberOfRegistrations; ++i) { 
		printf("ID: %d Name: %s Email: %s Event ID: %d Time: %s\n", registrations[i].id, registrations[i].name, registrations[i].email, registrations[i].eventID, registrations[i].time);
	}
	printf("\n");
}

void addRegistration(Registration **registrations, int *numberOfRegistrations, int *allocatedRegistrations, char *name, char* email, int eventID, int *incrementer) {	
	time_t currTime = time(0);
	struct tm *time_info = localtime(&currTime);
	char timeString[20];
	
	strftime(timeString, 20, "%Y-%m-%d %H:%M:%S", time_info);
	
	(*registrations)[*numberOfRegistrations].id = (*incrementer)++;
	strcpy((*registrations)[*numberOfRegistrations].name, name);
	strcpy((*registrations)[*numberOfRegistrations].email, email);
	(*registrations)[*numberOfRegistrations].eventID = eventID;
	strcpy((*registrations)[*numberOfRegistrations].time, timeString);

	(*numberOfRegistrations)++;
	
	printf("#%d A%d\n", *numberOfRegistrations, *allocatedRegistrations);
	
	int i, counter = 0;
	for(i=0; i<*numberOfRegistrations; ++i) {
		if((*registrations)[i].eventID == eventID)
			counter++;
	}
	
	printf("Registration complete, registrated as #%d -- # of registrations: %d [allocated: %d]\n", counter, *numberOfRegistrations,  *allocatedRegistrations);
	if(*allocatedRegistrations == *numberOfRegistrations){
		Registration *newAllocation = malloc(2 * *numberOfRegistrations * sizeof(Registration));
		*allocatedRegistrations = 2 * *numberOfRegistrations;
		
		memcpy(newAllocation, (*registrations), *numberOfRegistrations * sizeof(Registration));
		
		free(*registrations);
		(*registrations) = NULL;
		(*registrations) = newAllocation;
	}
	
	
	free(name);
	free(email);
	
	name = NULL;
	email = NULL;
}

void editRegistration(Registration *registrations, const int *numberOfRegistrations, char *name, char *email, const int id) {
	int index = findRegistrationById(registrations, numberOfRegistrations, id);
	
	strcpy(registrations[index].name, name);
	strcpy(registrations[index].email, email);
	
	free(email);
	free(name);
}

int findRegistrationById(Registration *registrations, const int *numberOfRegistrations, const int id) {
	int i;
	for(i = 0; i<*numberOfRegistrations; ++i) {
		if(registrations[i].id == id) {
			return i;
		}
	}
	return -1;
}

void removeRegistration(Registration *registrations, int *numberOfRegistrations, const int idToBeRemoved) {
	int index = findRegistrationById(registrations, numberOfRegistrations, idToBeRemoved);
	if(index == -1) {
		printf("ID does not exist!\n");
		return;
	}
	
	registrations[index] = registrations[--(*numberOfRegistrations)];
}
