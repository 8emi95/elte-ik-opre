#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h> //open,create
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> // perror, errno
#include <unistd.h> // pipe
#include <stdbool.h>

/*
int digit_to_int(char c)
{
	char str[2];
	str[0] = c;
	str[1] = '\0';
	return atoi(str);
}

// splits a string by spaces into tokens and gives back a string of tokens
char** Split(char* str)
{
	char ** result = NULL;
	char *  p = strtok(str, " "); // p-be beleteszi az elso tokent
	int n_spaces = 0;
	
	while (p) {
		result = realloc (result, sizeof (char*) * ++n_spaces);

		if (result == NULL)
			exit (-1); // memory allocation failed

		result[n_spaces-1] = p;

		p = strtok (NULL, " ");
	}
	
	//a nullt a végére külön tesszük bele
	result = realloc (result, sizeof (char*) * (n_spaces+1));
	result[n_spaces] = 0;
	
	return result;
}*/

// exit(1) -> file error, exit(2) -> pipe error

struct Person {
	char name[60];
	char email[60];
	int personID;
	char time[60];
};

struct Event {
	char name[60];
	int eventID;
};

void insertPerson(int* counter) {
	struct Person person;
	printf("Name: "); scanf("%s", person.name);
	printf("E-mail: "); scanf("%s", person.email);
	printf("Person ID: "); scanf("%d", & person.personID);
	while (getchar() != '\n');

	time_t t;
	time(&t);
	struct tm* tt;
	tt = localtime(&t);
	strftime(person.time, 60, "%c", tt);

	FILE* f = fopen("persons.txt", "ab");
	if (f == NULL) { perror("File error\n"); exit(1); }

	fwrite( &person, sizeof(person), 1, f);
	++( *counter);
	printf("Your number in the queue: %d\n", *counter);
	fclose(f);
}

int readFile(bool write) {
	FILE* f;
	int counter = 0;
	f = fopen("persons.txt", "rb");
	if (f == NULL) { return counter; }

	struct Person person;
	while (fread(&person, sizeof(person), 1, f)) {
	++counter;
	if (write) 
		printf("%s %s %s %d\n", person.time, person.name, person.email, person.personID);
	}
	fclose(f);
	return counter;
}

void deletePerson() {
	int counter = readFile(true);
	int deleteID = 0;

	printf("Person ID: "); scanf("%d", &deleteID);
	while (getchar() != '\n');

	FILE* f = fopen("persons.txt", "rb");
	if (f == NULL) { perror("File error\n"); exit(1); }
	FILE* replica;
	replica = fopen("replica.txt", "wb");

	struct Person person;
	int i = 0;
	while (fread(&person, sizeof(person), 1, f)) {
		++i;
		if (person.personID != deleteID) { 
			fwrite(&person, sizeof(person), 1, replica);
		}
	}

	fclose(f);
	fclose(replica);

	FILE* h = fopen("persons.txt", "wb");
	FILE* g = fopen("replica.txt", "rb");
	if(h == NULL || g == NULL) { perror("File error\n"); exit(1); }

	while (fread(&person, sizeof(person), 1, g)) {
		fwrite(&person, sizeof(person), 1, h);
	}

	fclose(h);
	fclose(g);
	remove("replica.txt");
}

void modifyPerson() {
	int modID = 0;

	printf("Persons ID: ");
	scanf("%d", &modID);
	while (getchar() != '\n');

	FILE* f;
	f = fopen("persons.txt", "rb");
	if (f == NULL) { perror("File error\n"); exit(1); }
	FILE* replica = fopen("replica.txt", "wb");

	struct Person mPerson;
	printf("Name: "); scanf("%s", mPerson.name);
	printf("E-mail: "); scanf("%s", mPerson.email);
	printf("EventID: "); scanf("%d", &mPerson.personID);
	while (getchar() != '\n');

	struct Person person;
	int i = 0;
	while (fread(&person, sizeof(person), 1, f)) {
		++i;
		if (person.personID == modID)
		{
		  strcpy(mPerson.time, person.time);
		  person = mPerson;
		}
		fwrite(&person, sizeof(person), 1, replica);
	}
	
	fclose(f);
	fclose(replica);

	FILE* g = fopen("replica.txt", "rb");
	FILE* h = fopen("persons.txt", "wb");
	if(h == NULL || g == NULL) { perror("File error\n"); exit(1); }

	while (fread(&person, sizeof(person), 1, g)) {
		fwrite(&person, sizeof(person), 1, h);
	}

	fclose(h);
	fclose(g);
	remove("replica.txt");
}

void createEvent() {
	struct Event event;
	printf("Name: "); scanf("%s", event.name);
	printf("Event ID: "); scanf("%d", &event.eventID);
	while (getchar() != '\n');

	FILE* f = fopen("events.txt", "ab");
	if (f == NULL) {  perror("File error\n"); exit(1); }

	fwrite(&event, sizeof(event), 1, f);
	fclose(f);
}

void listAllEvents()
{
	FILE* f = fopen("events.txt", "rb");
	if (f == NULL) { perror("File error\n"); exit(1); }

	struct Event event;
	while (fread(&event, sizeof(event), 1, f)) {
		printf("%s %d\n", event.name, event.eventID);
	}
	fclose(f);
}

void deleteEvent() {
	int deleteID = 0;
	printf("EventID: ");
	scanf("%d", &deleteID);
	while (getchar() != '\n');

	FILE* f;
	f = fopen("persons.txt", "rb");
	if (f == NULL) { perror("File error\n"); exit(1); }

	FILE* replica;
	replica = fopen("replica.txt", "wb");

	struct Person person;
	while (fread(&person, sizeof(person), 1, f)) {
		if (person.personID != deleteID) fwrite(&person, sizeof(person), 1, replica);
	}
	fclose(f);
	fclose(replica);

	FILE* h = fopen("persons.txt", "wb");
	FILE* g = fopen("replica.txt", "rb");
	if(h == NULL || g == NULL) { perror("File error\n"); exit(1); }

	while (fread(&person, sizeof(person), 1, g)) {
		fwrite(&person, sizeof(person), 1, h);
	}

	fclose(h);
	fclose(g);
	remove("replica.txt");

	f = fopen("events.txt", "rb");
	if (f == NULL) { perror("File error\n"); exit(1); }
	replica = fopen("replica.txt", "wb");

	struct Event event;
	while (fread(&event, sizeof(event), 1, f)) {
		if (event.eventID != deleteID) 
			fwrite(&event, sizeof(event), 1, replica);
	}
	
	fclose(f);
	fclose(replica);

	h = fopen("events.txt", "wb");
	g = fopen("replica.txt", "rb");
	if(h == NULL || g == NULL) { perror("File error\n"); exit(1); }

	while (fread(&event, sizeof(event), 1, g)) {
		fwrite(&event, sizeof(event), 1, h);
	}
	
	fclose(h);
	fclose(g);
	remove("replica.txt");
}

void runEvent(int personNumber, int eventID)
{	
	int arriveNumber = 0; // megerkezettek szama
	int notArriveNumber = 0;
	int i; // iterátor a ciklusokban
	
	int randomNumber = rand() % 100 + 1; // random szám
	struct Person person;
	int pipeline1[2], pipeline2[2];
	
	size_t personSize = sizeof(struct Person);
	
	pid_t pid;
	if(pipe(pipeline1) == -1 || pipe(pipeline2) == -1) { perror("Pipe hiba!"); exit(2); }
	
	pid = fork();
	
	if (pid == 0)
    {
		//printf("debug: child start");
		write(pipeline2[1],"Jelen!",sizeof("Jelen!"));
		read(pipeline1[0], &arriveNumber, sizeof(arriveNumber));
		printf("Maximalis letszam: %d\n", arriveNumber);
		
		struct Person notArrived[arriveNumber];

		for(i = 0; i < arriveNumber; ++i)
		{
			read(pipeline1[0], &person, personSize);
			randomNumber = rand() % 100 + 1;
			
			if(randomNumber <= 10)
			{
				notArrived[notArriveNumber] = person;
				++notArriveNumber;
			} 
			else 
			{
				printf("vendeg megerkezett: %s\n", person.name);
			}
		}
		randomNumber  = rand()%101;
		
		write(pipeline2[1], &notArriveNumber, sizeof(notArriveNumber));
		write(pipeline2[1], &randomNumber, sizeof(randomNumber));
		for(i = 0; i < notArriveNumber; ++i) {
			write(pipeline2[1], &notArrived[i], personSize);
		}

		printf("Gyerek lefektetve aludni\n");
		exit(EXIT_SUCCESS);
    }
    else
    {
		//printf("debug: parent start");
		char* message;
		read(pipeline2[0],&message, 500);
		printf("Latogatok kuldese\n");
		struct Person persons[personNumber];
		
		
		arriveNumber = 0;
		FILE * f;
		f = fopen("persons.txt", "rb");
		if (f == NULL) { perror("File hiba"); exit(1); }
		
		while (fread( & person, personSize, 1, f)) 
		{
			if(person.personID == eventID){
				persons[arriveNumber] = person;
				++arriveNumber;	
			}
		}
		fclose(f);

		write(pipeline1[1], &arriveNumber, sizeof(arriveNumber));
		for(i = 0; i < arriveNumber; ++i)
		{
			write(pipeline1[1], &persons[i], personSize);
		}
		
		wait();
		read(pipeline2[0], &notArriveNumber, sizeof(notArriveNumber));
		read(pipeline2[0], &randomNumber, sizeof(randomNumber));
		printf("Ennyi vendeg nem erkezett meg: %d\n", notArriveNumber);
		printf("A rendezmeny ilyen sikeres volt: %d\n", randomNumber);
		
		if(notArriveNumber > 0)
		{
			printf("\n");
			for(i = 0; i < notArriveNumber; ++i)
			{
				read(pipeline2[0], &person, personSize);
				printf("Resztvevo neve: %s\n", person.name);
			}
		}
		printf("Joejt, a szulo is aludni megy\n\n");
    }
}

void foreachEvent(int counter){
	//printf("debug: foreachEvent");
	FILE* f;
	f = fopen("events.txt", "rb");
	if (f == NULL) { perror("File error\n"); exit(1); }

	struct Event event;
	while(fread(&event, sizeof(event), 1, f))
	{
		//printf("debug: before runEvent");
		runEvent(counter, event.eventID);
	}
	fclose(f);
}

int main() {
	srand(time(NULL));
	char command;
	int counter = readFile(false);


	do {
		rand();
		printf("\n0 - Kilepes\n");
		printf("1 - Add new person\n");
		printf("2 - Create new event\n");
		printf("3 - Modify persons\n");
		printf("4 - List persons\n");
		printf("5 - List events\n");
		printf("6 - Delete persons\n");
		printf("7 - Delete events\n");
		printf("8 - Run events\n\n");
		scanf("%c", &command);
		while (getchar() != '\n');
		printf("\n");

		if(command == '1')
			insertPerson(&counter);
		else if(command == '2')
			createEvent();
		else if(command == '3')
			modifyPerson();
		else if(command == '4')
			readFile(true);
		else if(command == '5')
			listAllEvents();
		else if(command == '6') {
			deletePerson();
			counter = readFile(false); // watch if number of members has been changed
		}
		else if(command == '7')
			deleteEvent();
		else if(command == '8') {
			//printf("debug: command8, counter: %d", counter);
			counter = readFile(false);
			foreachEvent(counter);
		}
	} while (command != '0');

	return 0;
}