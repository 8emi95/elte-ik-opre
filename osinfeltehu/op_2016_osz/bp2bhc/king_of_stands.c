#include "king_of_stands.h"

int main()
{
	displayMenu();
	return 0;
}


void displayMenu()
{
	printf("Válassz egy opciót!\n\n");
	printf("1. Résztvevő hozzáadása\n");
	printf("2. Résztvevő adatainak módosítása\n");
	printf("3. Résztvevő törlése\n");
	printf("4. Rendezvény létrehozása\n");
	printf("5. Rendezvény résztvevői\n");
	printf("6. Rendezvény törlése\n");
	printf("0. Kilépés\n");


	while(1){
		long choice;
		char *ptr;
		char input[4];
		
		printf("Választott opció: ");
		
		fgets(input, 4, stdin);
		choice = strtol(input, &ptr, 10);

		switch(choice){
			case 0:
			return;
			case 1:
			addPerson();
			break;
			case 2:
			modifyPerson();
			break;
			case 3:
			removePerson();
			break;
			case 4:
			createEvent();
			break;
			case 5:
			listEvent();
			break;
			case 6:
			deleteEvent();
			break;
		}

		
	}
}

void addPerson()
{
	char* eventID;
	Person p = createPerson();
	printf("A rendezvény azonosítója: ");
	eventID = readLine(stdin);
	if(eventExists(eventID)){
		writePersonToFile(p, eventID);
		printf("%s a(z) %d. résztvevő az eseményen.\n", p.name, countPeople(eventID));
	} else{
		printf("Ilyen sorszámú esemény még nem létezik, előbb hozd létre!\n");
	}

	freePerson(p);
	free(eventID);

	return;
}

void modifyPerson()
{
	Person oldData, newData;
	printf("A rendezvény azonosítója: ");
	char *eventID = readLine(stdin);
	if(!eventExists(eventID)){
		printf("Nincs ilyen rendezvény.\n");
		return;
	}
	printf("A résztvevő régi adatai:\n");
	oldData = createPerson();
	printf("A résztvevő új adatai:\n");
	newData = createPerson();
	
	if(deleteFromFile(oldData, eventID)){
		writePersonToFile(newData, eventID);
		printf("Sikeres módosítás.\n");
	}else{
		printf("Nincs ilyen nevű résztvevő\n");
	}
	
	freePerson(oldData);
	freePerson(newData);
	free(eventID);
}

void removePerson()
{
	Person data;
	printf("A rendezvény azonosítója: ");
	char *eventID = readLine(stdin);
	if(!eventExists(eventID)){
		printf("Nincs ilyen rendezvény.\n");
		return;
	}
	printf("A résztvevő adatai:\n");
	data = createPerson();
	if(deleteFromFile(data, eventID)){
		printf("Sikeres törlés.\n");
	} else {
		printf("Nincs ilyen résztvevő.");
	}
	free(eventID);
	freePerson(data);
}

void createEvent()
{
	printf("A rendezvény azonosítója: ");
	char* eventID = readLine(stdin);
	if(eventExists(eventID)){
		printf("Ilyen sorszámú esemény már létezik. Előbb töröld.\n");
		return;
	}
	FILE* eventfile = fopen(eventID, "w");
	fclose(eventfile);
	printf("Esemény létrehozva.\n");
	free(eventID);
}

void listEvent(){
	int c;
	printf("A rendezvény azonosítója: ");
	char* eventID = readLine(stdin);
	if(!eventExists(eventID)){
		printf("Nincs ilyen rendezvény.\n");
		return;
	}
	FILE* infile = fopen(eventID, "r");
	while((c = fgetc(infile)) != EOF){
		putchar(c);
	}
	fclose(infile);
	free(eventID);
}

void deleteEvent(){
	printf("A rendezvény azonosítója: ");
	char* eventID = readLine(stdin);
	if(!eventExists(eventID)){
		printf("Nincs ilyen esemény.\n");
	} else if(remove(eventID) == 0){
		printf("Sikeres törlés.\n");
	} else{
		printf("Hiba történt.\n");
	}
	free(eventID);
}

char* readLine(FILE* stream)
{
	char *buffer;
	char *ret;
	size_t bufsize = DEFAULT_SIZE;
	size_t lineLength;
	ssize_t read;

	buffer = (char *) malloc(bufsize * sizeof(char));
	read = getline(&buffer, &bufsize, stream);
	if(read == -1){
		return NULL;
	}
	lineLength = strlen(buffer);
	ret = (char *) malloc(lineLength * sizeof(char));
	strncpy(ret, buffer, lineLength -1);
	
	return ret;
}


void freePerson(Person p)
{
	free(p.name);
	free(p.email);

}

void writePersonToFile(Person p, char* filename)
{
	FILE* outfile;
	outfile = fopen(filename, "a");
	char *line = personToLine(p);
	fprintf(outfile ,"%s;%s, %s\n", line, __DATE__, __TIME__);
	fclose(outfile);
	free(line);
}

int deleteFromFile(Person p, char* filename)
{
	FILE* infile = fopen(filename, "r");
	FILE* outfile = fopen("temp", "w");
	char* personData = personToLine(p);
	char* line;
	int found = 0;
	while((line = readLine(infile))){
		if(!strstr(line, personData)){
			fprintf(outfile, "%s\n", line);
		}else{
			found = 1;
		}
		free(line);
	}
	fclose(infile);
	fclose(outfile);
	free(personData);
	remove(filename);
	rename("temp", filename);
	return found;
}
int countPeople(char* filename)
{
	FILE* infile;
	infile = fopen(filename, "r");
	int count = 0;
	while(readLine(infile)){
		count++;
	}
	fclose(infile);
	return count;

}

int eventExists(char *eventID)
{
	glob_t globbuff;
	return glob(eventID, GLOB_NOSORT, NULL, &globbuff) != GLOB_NOMATCH;
}

char* personToLine(Person p)
{
	
	size_t lineLength = strlen(p.name) + strlen(p.email) + 2;
	char *ret;
	ret = (char *) malloc(lineLength * sizeof(char));
	strcpy(ret, p.name);
	ret[strlen(p.name)] = ';';
	strcpy(ret + strlen(p.name) + 1, p.email);
	return ret;
}

Person createPerson()
{
	Person p;
	printf("Név: ");
	p.name = readLine(stdin);
	printf("E-mail: ");
	p.email = readLine(stdin);
	return p;
}
