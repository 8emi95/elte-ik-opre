#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,create
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

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
}

void insert_person(const char* fname, char* name, char* email, int eventID)
{
	// Minden sor így néz ki a fájlban: Rendszeridő (stringként) FirstnameLastname Email EventID
	
	FILE * f;
	f=fopen(fname,"w");
	if (f == NULL){ perror("\nFile opening error\n"); exit(1); }
	
	char line[150];
	char** elements;
	bool found = false;
	int count = 0; // a jelentkezo sorszama
	while (!feof(f) && !found)
	{
		fgets(line,sizeof(line),f);
		elements = Split(line);
		if (atoi(elements[3]) == eventID) {
			++count;
			// ha már benne van, nem szúrjuk bele újra
			found = (strcmp(elements[1],name) != 0 && strcmp(elements[2], email) != 0);
		}
	}
	
	if(!found)
	{
		time_t my_time;
		time(&my_time);
		fprintf(f, "\n%s %s %s %d", ctime(&my_time), name, email, eventID);
		printf("\nPerson inserted to the event.\n");
		printf("\nYou are the %d. in the queue for this event.\n");
		
	} else {
		perror("\nThere is already a person in the database with the same parameters\n");
		exit(2);
	}

	fclose(f);
}

void modify_person(const char* fname, char* name, char* email, int eventID)
{
	FILE * f;
	f=fopen(fname,"w");
	if (f == NULL){ perror("\nFile opening error\n"); exit(1); }
	
	char line[150];
	char** elements;
	bool found = false; 
	while (!feof(f) && !found){
		fgets(line,sizeof(line),f);
		elements = Split(line);
		if (strcmp(elements[1],name) != 0 && atoi(elements[3]) == eventID)
			found = true;
	}
	
	if(found)
	{
		time_t my_time;
		time(&my_time);
		elements[0] = ctime(&my_time);
		elements[2] = email;
		printf("\nPerson's email modified.\n");
	} else {
		perror("\nThere is no such person with these parameters in this event\n");
		exit(2);
	}

	fclose(f);
}

void delete_person(const char* fname, char* name, int eventID)
{
	FILE * f;
	f = fopen(fname,"r");
	if (f == NULL){ perror("\nFile opening error\n"); exit(1); }
	
	char line[150];
	char lastline[150];
	char** elements;
	int i = 0;
	bool found = false;
	while (!feof(f) && !found){
		fgets(line,sizeof(line),f);
		elements = Split(line);
		++i;
		if (strcmp(elements[1],name) != 0 && atoi(elements[3]) == eventID)
			found = true;
	}
	
	if(found)
	{
		char ch;
		int linecounter = 1;
		
		FILE* result;
		result = fopen("replica.txt", "w");
		
		ch = getc(f);
		while (ch != EOF)
		{
			ch = getc(f);
			if (ch == '\n')
				++linecounter;
			
			//except the line to be deleted
			if (linecounter != i)
			{
				//copy all lines into replica.txt
				putc(ch, result);
			}
		}
		fclose(f);
		fclose(result);
		remove(fname); // törölni a régi fájlt
		rename("replica.txt", fname); //rename replica.txt to original name
		printf("\nPerson deleted from this event.");
		
		printf("\nThe contents of file after being modified are as follows:\n");
		result = fopen(fname, "r");
		char line[150];
		while (!feof(f)){
			fgets(line,sizeof(line),f);
			printf("%s",line);
		}
		printf("\n");
		
		fclose(result);
		
	} else {
		fclose(f);
		perror("\nThere is no such person with these parameters in this event\n");
		exit(2);
	}
}

void delete_event(const char* fname, int eventID)
{
	FILE * f;
	f = fopen(fname,"r");
	if (f == NULL){ perror("\nFile opening error\n"); exit(1); }
	
	char line[150];
	char lastline[150];
	char** elements;
	int i = 0;
	// elsőre megszámoljuk, hány sort kell majd törölni
	while (!feof(f)){
		fgets(line,sizeof(line),f);
		elements = Split(line);
		if (atoi(elements[3]) == eventID)
			++i;
	}
	
	if(i > 0) // ha legalább 1 sort kell törölni
	{
		int deleterows[i]; // törlendő sorok sorszámainak tömbje
		
		// újra átolvassuk a fájlt és meghatározzuk, mely sort kell törölni
		int j=0; // tömb aktuális eleme
		i=0; // aktuális sor
		while (!feof(f)){
			fgets(line,sizeof(line),f);
			elements = Split(line);
			++i;
			if (atoi(elements[3]) == eventID)
				deleterows[j++]=i;
		}
		
		//eztán végrehajtjuk a törlést a kigyűjtött sorokra
		char ch;
		int linecounter = 0;
		bool wrongrow = false;
		bool linecounterchanged = false;
		
		FILE* result;
		result = fopen("replica.txt", "w");
		
		ch = getc(f);
		while (ch != EOF)
		{
			ch = getc(f);
			if (ch == '\n') {
				++linecounter;
				linecounterchanged = true;
			} else {
				linecounterchanged = false;
			}
			
			if(linecounterchanged) // ha változott a sorszámunk, felülvizsgáljuk törlendő-e a sor
			{
				wrongrow = false; // törlendő sorban állunk-e
				j = 0;
				while (j < i && !wrongrow) {
					wrongrow = (linecounter == deleterows[j]);
					++j;
				}
			}
			
			if(!wrongrow)
				putc(ch, result);
		}

		fclose(f);
		fclose(result);
		remove(fname); // törölni a régi fájlt
		rename("replica.txt", fname); //rename replica.txt to original name
		printf("\nPerson deleted from this event.");
		
		printf("\nThe contents of file after being modified are as follows:\n");
		result = fopen(fname, "r");
		char line[150];
		while (!feof(f)){
			fgets(line,sizeof(line),f);
			printf("%s",line);
		}
		printf("\n");
		
		fclose(result);
		
	} else {
		fclose(f);
		perror("\nThere is no such person with these parameters in this event\n");
		exit(2);
	}
}

void list_event(const char* fname, int eventID) 
{
	FILE * f;
	f=fopen(fname,"r");
	if (f == NULL){ perror("\nFile opening error\n"); exit(1); }
	
	char line[150];
	char** elements;
	while (!feof(f)){
		fgets(line,sizeof(line),f);
		elements = Split(line);
		//if (atoi(elements[strlen(elements)-1]) == eventID)
		if (atoi(elements[3]) == eventID)
			printf("%s",line);
	} 
	printf("\n");
	
	fclose(f);
}

void list_all_events(const char* fname)
{
	FILE * f;
	f=fopen(fname,"r");
	if (f == NULL){ perror("\nFile opening error\n"); exit(1); }
	
	char line[150];
	while (!feof(f)){
		fgets(line,sizeof(line),f);
		printf("%s",line);
	}
	printf("\n");
	
	fclose(f);
}

// argc - Argomentum Counter, argv - Argomentum Value
int main(int argc,char** argv){
	// argv: 0 - Filename, 1 - Order [2.. - Actual parameters]
	
	if(argc == 0) {perror("No arguments\nGive a filename, an order and parameters\n"); exit(1);}
	if (!access(argv[0],F_OK) == 0){perror("The file is not exist!\n"); exit(1);}

	if(strcmp(argv[1],"insertPerson") != 0)
		insert_person(argv[0], argv[2], argv[3], atoi(argv[4])); // FirstnameLastname Email EventID
	
	else if(strcmp(argv[1],"modifyPerson") != 0)
		modify_person(argv[0], argv[2], argv[3], atoi(argv[4])); // FirstnameLastname Email EventID
	
	else if(strcmp(argv[1],"deletePerson") != 0)
		delete_person(argv[0], argv[2], atoi(argv[3])); // FirstnameLastname EventID
	
	else if(strcmp(argv[1],"deleteEvent") != 0)
		delete_event(argv[0], atoi(argv[2])); // EventID
	
	else if(strcmp(argv[1],"listEvent") != 0)
		list_event(argv[0],atoi(argv[2])); // FileName EventID
	
	else if(strcmp(argv[1],"listAllEvents") != 0)
		list_all_events(argv[0]); // FileName
	
	else
		perror("Select a valid order: insertPerson, modifyPerson, deletePerson, deleteEvent, listEvent, listAllEvents");

	return 0;
 
}