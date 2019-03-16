#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

/* Define party type with constructor */
struct objParty {
	int ID;
	char* Name;
};
typedef struct objParty Party;
Party* ConsParty(int id, char* name) {
	Party* p = malloc(sizeof(Party));
	p->ID = id;
	*p->Name = *name;
	return p;
};
/*Define participant type*/
struct objParticipant {
	int Nr;
	int PartyId;
	char* Name;
};
typedef struct objParticipant Participant;
Participant* ConsParticipant(int nr, int id, char* name) {
	Participant* p = malloc(sizeof(Participant));
	p->Nr = nr;
	p->PartyId = id;
	*p->Name = *name;
	return p;
};

void universalSleep(int seconds) {
	#ifdef _WIN32
	Sleep(seconds);
	#else
	sleep(seconds);
	#endif
};

void universalClear() {
	#ifdef _WIN32
	system("cls");
	#else
	system("clear");
	#endif
}

void menuPrinter (char* str, char mk) {
	universalClear();
	printf("Kong of Stands\n");
	printf("Avaiable menu points:\n");	
	printf("1 - New user registration\n");
	printf("2 - List registered users\n");
	printf("3 - Modify registration\n");
	printf("4 - Delete registration\n");
	printf("5 - New party\n");
	printf("6 - Delete party\n");
	printf("0 - Exit the application\n");
};

void newUserRegistration (char mk) {
	menuPrinter("", mk);
	printf("New User Registration\n");
};

void listRegisteredUsers (char mk) {
	menuPrinter("", mk);
	printf("List Registered Users\n");
};

void keyReader (char* menuKey) {
	scanf(" %c", menuKey);
};

void exitApplication () {
	printf("Exiting.");
	fflush(stdout);
	universalSleep(1);
	printf(".");
	fflush(stdout);
	universalSleep(1);
	printf(".\n");
	universalSleep(1);
};

void readPartyFile () {};
void readUserFile () {};

int main() {	
	char menuKey;
	menuPrinter("", menuKey);
	keyReader(&menuKey);
	universalSleep(1);
	while (menuKey != '0') {
		switch(menuKey) {
			case '1':
				newUserRegistration(menuKey);
				keyReader(&menuKey);
				break;
			case '2':
				listRegisteredUsers(menuKey);
				keyReader(&menuKey);
				break;
			default:
				menuPrinter("Not handled input!", menuKey);
				keyReader(&menuKey);
				break;
		}
	}
	exitApplication();
};
