#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <stdbool.h>

struct Person {
  char name[200];
  char email[150];
  int personID;
  char time[100];
};

struct Event {
  char name[200];
  int eventID;
};

int readFile(bool write);
void createEvent();
void listEvents();
void deleteEvent();
void newReg(int* counter);
void deleteReg();
void changeRegData();

int main()
{
printf("Udvozlom a 'King of Stands' elektronikus latogatoi nyilvantartasaban! \n");

int option;
int counter = readFile(false);

do
{
printf("1) Uj rendezveny. \n");
printf("2) Esemeny torlese. \n");
printf("3) Kilepes a rendszerbol. \n");
printf("4) Uj Regisztracio. \n");
printf("5) Resztvevok listajanak lekerese. \n");
printf("6) Rendezvenyek listajanak lekerese.\n");
printf("7) Regisztracio torlese. \n");
printf("8) Regisztracio modositasa. \n");

scanf("%d", &option);

if(option == 1)
{
	printf("Uj rendezveny letrehozasa.\n");
	createEvent();

}
else if(option == 2)
{
	printf("Egy esemeny torlese.\n");
	deleteEvent();

}
else if(option == 4)
{
	printf("Uj regisztracio rogzitese: \n");
	newReg(&counter);

}
else if(option == 5)
{
	printf("A regisztraciok:\n");
	readFile(true);

}
else if(option == 6)
{
	printf("Esemenyek:\n");
	listEvents();

}
else if(option == 7)
{
	printf("Regisztracio torlese. \n");
	deleteReg();

}

else if(option == 8)
{
	printf("Regisztracio modositasa. Modositani egyesevel lehetseges.\n");
	changeRegData();

}


}
while (option != 3);

printf("Koszonjuk, hogy a mi szolgaltatasunkat valasztotta, viszontlatasra!\n");

 return 0;
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

void createEvent() {
  struct Event event;
  printf("Kerem adja meg az esemeny nevet: "); scanf("%s", event.name);
  printf("Kerem adja meg az esemeny azonositojat: "); scanf("%d", &event.eventID);
  while (getchar() != '\n');

  FILE* f = fopen("events.txt", "ab");
  if (f == NULL) {  perror("File error\n"); exit(1); }
  
  fwrite(&event, sizeof(event), 1, f);
  fclose(f);
}

void listEvents() {
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

void newReg(int* counter) {
  struct Person person;
  printf("Kerem adja meg a nevet (szokoz helyett _ ): "); scanf("%s", person.name);
  printf("Kerem adja meg az email-cimet: "); scanf("%s", person.email);
  printf("Kerem adja meg szemelyi azonositojat (ID): "); scanf("%d", & person.personID);
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
  printf("On a(z) %d. regisztralo.\n", *counter);
  fclose(f);
}

void deleteReg() {
  int counter = readFile(true);
  int deleteID = 0;
  
  printf("ID: "); scanf("%d", &deleteID);
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

void changeRegData() {
  int modID = 0;
  
  printf("A modositando szemely azonositoja (ID): ");
  scanf("%d", &modID);
  while (getchar() != '\n');
  
  FILE* f;
  f = fopen("persons.txt", "rb");
  if (f == NULL) { perror("File error\n"); exit(1); }
  FILE* replica = fopen("replica.txt", "wb");
  
  struct Person mPerson;
  printf("Uj nev (szokoz helyett hasznaljon _ jelet): "); scanf("%s", mPerson.name);
  printf("Uj email: "); scanf("%s", mPerson.email);
  printf("Uj szemelyi azonosito (ID): "); scanf("%d", &mPerson.personID);
  while (getchar() != '\n');
  
  struct Person person;
  int i = 0;
  while (fread(&person, sizeof(person), 1, f)) {
    ++i;
    if (person.personID == modID) {
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