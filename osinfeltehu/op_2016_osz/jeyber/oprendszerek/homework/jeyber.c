#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <errno.h>

struct Event {
	char id[100];
	char name[100];
	char location[100];
};

struct Guest {
	char name[100];
	char email[100];
	char eventId[100];
	char date[100];
};

void displayUI() {
	printf("\n1 - New guest\n");
	printf("2 - Modify guest\n");
	printf("3 - Delete guest\n");
	printf("4 - New event\n");
	printf("5 - List event details\n");
	printf("6 - Delete event\n");
	printf("7 - Exit\n");
	
	printf("Choose one of the above options: ");
}

void createWorkingFilesIfNotExists(const char * eventFileName, const char * guestFileName) {
	int isEventFileOpened, isGuestFileOpened;
	
	isEventFileOpened = open(eventFileName,O_CREAT,S_IRUSR|S_IWUSR);
	if(isEventFileOpened < 0) {
		perror("\nUnable to open the file that contains events\n");
		exit(1);
	}
	isGuestFileOpened = open(guestFileName,O_CREAT,S_IRUSR|S_IWUSR);
	if(isGuestFileOpened < 0) {
		perror("\nUnable to open the file that contains guests\n");
		exit(1);
	}
	close(isEventFileOpened);
	close(isGuestFileOpened);
}

int readMenuOption() {
	int option;
	bool isCorrect = false;
	
	while(!isCorrect) {
		scanf("%i",&option);
		if(option < 1 || option > 7) {
			printf("\nInvalid option! Please try again: ");
		} else {
			isCorrect = true;
		}
	}
	
	return option;
}

int isEventExists(struct Event e, const char * filename) {
	struct Event chk;
	char s[250];
	FILE * f;
	char * token;
	int check = 0;
	
	f = fopen(filename,"r");
	if(f == NULL) {
		perror("\nUnable to open file\n");
	}
	
	while(fgets(s,sizeof(char)*(1 + 100 * 2),f)) {
		read(f,&s,sizeof(s));
		token = strtok(s, "}");
		strcpy(chk.id,token);
		token = strtok(NULL, "}");
		strcpy(chk.name,token);
		
		if(strcmp(e.id,chk.id) == 0) {
			check++;
		}
	};
	
	fclose(f);
	
	return check;
}

int isGuestExists(struct Guest g, const char * filename) {
	struct Guest chk;
	char s[500];
	FILE * f;
	char * token;
	int check = 0;
	
	f = fopen(filename,"r");
	if(f == NULL) {
		perror("\nUnable to open file\n");
	}
	
	while(fgets(s,sizeof(char)*(3 + 100 * 4),f)) {
		read(f,&s,sizeof(s));
		token = strtok(s, "}");
		strcpy(chk.name,token);
		token = strtok(NULL, "}");
		strcpy(chk.email,token);
		token = strtok(NULL, "}");
		strcpy(chk.eventId,token);
		token = strtok(NULL, "}");
		strcpy(chk.date,token);
		
		if(strcmp(g.email,chk.email) == 0 && strcmp(g.eventId,chk.eventId) == 0) {
			check++;
		}
	};
	
	fclose(f);
	
	return check;
}

struct Event createEvent(char mod) {
	struct Event event;
	
	printf("Event ID: ");
	scanf("%s",event.id);
	
	if(mod != 'P') {
		printf("Event name: ");
		scanf("%s",event.name);
		printf("Event location: ");
		scanf("%s",event.location);

	} else {
		strcpy(event.name,"X");
		strcpy(event.location,"X");
	}
	
	return event;
}

struct Guest createGuest(char mod) {
	struct Guest guest;
	time_t current_time;
	struct tm * time_info;
	
	if(mod != 'P') {
		printf("Guest name: ");
		scanf("%s",guest.name);
	} else {
		strcpy(guest.name,"X");
	}
	
	printf("E mail: ");
	scanf("%s",guest.email);
	
	printf("Event ID: ");
	scanf("%s",guest.eventId);
	
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(guest.date, 20, "%Y.%m.%d %H:%M:%S", time_info);
	
	return guest;
}

void storeEvent(struct Event e, const char * filename) {
	char * s = (char *) malloc(sizeof(char) * (1 + strlen(e.id) + strlen(e.name)));
	strcpy(s,e.id);
    strcat(s,"}");
	strcat(s,e.name);
	strcat(s,"}");
	strcat(s,e.location);
	
	
	FILE * f;
	f = fopen(filename,"a");
	if(f == NULL) {
		perror("\nUnable to open file\n");
	}
	fputs(s,f);
	fputs("\n",f);
	
	fclose(f);
}

void storeGuest(struct Guest g, const char * filename) {
	char * s = (char *) malloc(sizeof(char) * (3 + strlen(g.name) + strlen(g.email) + strlen(g.eventId) + strlen(g.date)));
	strcpy(s,g.name);
	strcat(s,"}");
	strcat(s,g.email);
	strcat(s,"}");
	strcat(s,g.eventId);
	strcat(s,"}");
	strcat(s,g.date);
	
	FILE * f;
	f = fopen(filename,"a");
	if(f == NULL) {
		perror("\nUnable to open file\n");
	}
	fputs(s,f);
	fputs("\n",f);
	
	fclose(f);
}

void listGuests(struct Event e, const char * filename) {
	struct Guest chk;
	char s[500];
	FILE * f;
	char * token;
	int check = 0;
	
	f = fopen(filename,"r");
	if(f == NULL) {
		perror("\nUnable to open file\n");
	}
	
	while(fgets(s,sizeof(char)*(3 + 100 * 4),f)) {
		read(f,&s,sizeof(s));
		token = strtok(s, "}");
		strcpy(chk.name,token);
		token = strtok(NULL, "}");
		strcpy(chk.email,token);
		token = strtok(NULL, "}");
		strcpy(chk.eventId,token);
		token = strtok(NULL, "}");
		strcpy(chk.date,token);
		
		if(strcmp(e.id,chk.eventId) == 0) {
			check++;
			printf("Guest %i:",check);
			printf("\nName: %s\nEmail: %s\nTimestamp: %s\n",chk.name,chk.email,chk.date);
		}
	};
	
	if(check == 0) {
		printf("This event is empty!");
	}
	
	fclose(f);
}

int countGuests(struct Event e, const char * filename) {
	struct Guest chk;
	char s[500];
	FILE * f;
	char * token;
	int cnt = 0;
	
	f = fopen(filename,"r");
	if(f == NULL) {
		perror("\nUnable to open file\n");
	}
	
	while(fgets(s,sizeof(char)*(3 + 100 * 4),f)) {
		read(f,&s,sizeof(s));
		token = strtok(s, "}");
		strcpy(chk.name,token);
		token = strtok(NULL, "}");
		strcpy(chk.email,token);
		token = strtok(NULL, "}");
		strcpy(chk.eventId,token);
		token = strtok(NULL, "}");
		strcpy(chk.date,token);
		
		if(strcmp(e.id,chk.eventId) == 0) {
			cnt++;
		}
	};
	
	fclose(f);
	
	return cnt;
}

void deleteEvent(struct Event e, const char * filename) {
	struct Event tmp;
	char s[250];
	char p[250];
	FILE * f;
	FILE * h;
	char * token;
	int t;
	const char tmp_file[100] = "tmp_event.txt";
	
	t = open(tmp_file,O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(t < 0) {
		perror("\nUnable to open file\n");
		exit(1);
	}
	close(t);
	
	f = fopen(filename,"a+");
	h = fopen(tmp_file,"a+");
	if(f == NULL || h == NULL) {
		perror("\nUnable to open file\n");
	}

	while(fgets(s,sizeof(char)*(1 + 100 * 2),f)) {
		read(f,&s,sizeof(s));
		strcpy(p,s);
		token = strtok(s, "}");
		strcpy(tmp.id,token);
		token = strtok(NULL, "}");
		strcpy(tmp.name,token);
		
		if(strcmp(e.id,tmp.id) != 0) {
			fputs(p,h);
		}
	};
	
	fclose(f);
	fclose(h);
	
	remove(filename);
	rename(tmp_file,filename);
}

void deleteGuest(struct Guest g, const char * filename) {
	struct Guest tmp;
	char s[500];
	char p[500];
	FILE * f;
	FILE * h;
	char * token;
	int t;
	const char tmp_file[100] = "tmp_guest.txt";
	
	t = open(tmp_file,O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(t < 0) {
		perror("\nUnable to open file\n");
		exit(1);
	}
	close(t);
	
	f = fopen(filename,"a+");
	h = fopen(tmp_file,"a+");
	if(f == NULL || h == NULL) {
		perror("\nUnable to open file\n");
	}
	
	while(fgets(s,sizeof(char)*(3 + 100 * 4),f)) {
		read(f,&s,sizeof(s));
		strcpy(p,s);
		token = strtok(s, "}");
		strcpy(tmp.name,token);
		token = strtok(NULL, "}");
		strcpy(tmp.email,token);
		token = strtok(NULL, "}");
		strcpy(tmp.eventId,token);
		token = strtok(NULL, "}");
		strcpy(tmp.date,token);
		
		if(!(strcmp(g.email,tmp.email) == 0 && strcmp(g.eventId,tmp.eventId) == 0)) {
			fputs(p,h);
		}
	};
	
	fclose(f);
	fclose(h);
	
	remove(filename);
	rename(tmp_file,filename);
}

void deleteAllGuests(struct Event e, const char * filename) {
	struct Guest tmp;
	char s[500];
	char p[500];
	FILE * f;
	FILE * h;
	char * token;
	int t;
	const char tmp_file[100] = "tmp_guest.txt";
	
	t = open(tmp_file,O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	if(t < 0) {
		perror("\nUnable to open file\n");
		exit(1);
	}
	close(t);
	
	f = fopen(filename,"a+");
	h = fopen(tmp_file,"a+");
	if(f == NULL || h == NULL) {
		perror("\nUnable to open file\n");
	}
	
	while(fgets(s,sizeof(char)*(3 + 100 * 4),f)) {
		read(f,&s,sizeof(s));
		strcpy(p,s);
		token = strtok(s, "}");
		strcpy(tmp.name,token);
		token = strtok(NULL, "}");
		strcpy(tmp.email,token);
		token = strtok(NULL, "}");
		strcpy(tmp.eventId,token);
		token = strtok(NULL, "}");
		strcpy(tmp.date,token);
		
		if(strcmp(e.id,tmp.eventId) != 0) {
			fputs(p,h);
		}
	};
	
	fclose(f);
	fclose(h);
	
	remove(filename);
	rename(tmp_file,filename);
}

int main(int argc,char ** argv)
{
	const char * eventFileName = "event.txt";
	const char * guestFileName = "guest.txt";
	int menuOption;
	int check;
	int cnt;
	struct Event event;
	struct Guest guest;
	struct Guest guestM;
	
	system("clear");
	createWorkingFilesIfNotExists(eventFileName,guestFileName);
	
	while(menuOption != 7) {
		displayUI();
		menuOption = readMenuOption();
		
		switch (menuOption) {
			
			case 1:
				guest = createGuest('X');
				strcpy(event.id,guest.eventId);
				strcpy(event.name,"X");
				if(isEventExists(event,eventFileName) == 0) {
					printf("\nThis event does not exist!");
				} else {
					if(isGuestExists(guest,guestFileName) != 0) {
						printf("\nThis email adress is already in use!");
					} else {
						storeGuest(guest,guestFileName);
						cnt = countGuests(event,guestFileName);
						printf("\nSuccessful registration on the %i. place",cnt);
					}
				}
				break;
				
			case 2:
				guest = createGuest('P');
				if(isGuestExists(guest,guestFileName) == 0) {
					printf("\nEmail address not found!");
				} else {
					printf("\nModify registration: \n");
					guestM = createGuest('X');
					strcpy(event.id,guestM.eventId);
					strcpy(event.name,"X");
					if(isEventExists(event,eventFileName) == 0) {
						printf("\nEvent not found!");
					} else {
						deleteGuest(guest,guestFileName);
						storeGuest(guestM,guestFileName);
						printf("\nSuccessful modification!");
					}
				}
				break;
				
			case 3:
				guest = createGuest('P');
				if(isGuestExists(guest,guestFileName) == 0) {
					printf("\nEmail in this event is not found!");
				} else {
					deleteGuest(guest,guestFileName);
					printf("\nSuccessful deletion!");
				}
				
				break;
				
			case 4:
				event = createEvent('X');
				if(isEventExists(event,eventFileName) != 0) {
					printf("\nThis ID is already taken!");
				} else {
					storeEvent(event,eventFileName);
					printf("\nSuccessful creation!");
				}
				
				break;
				
			case 5:
				event = createEvent('P');
				if(isEventExists(event,eventFileName) == 0) {
					printf("\nThis ID does not exist!");
				} else {
					printf("\nRegistrations:\n");
					listGuests(event,guestFileName);
				}
				break;
				
			case 6:
				event = createEvent('P');
				if(isEventExists(event,eventFileName) == 0) {
					printf("\nThis ID does nt exist!");
				} else {
					deleteEvent(event,eventFileName);
					deleteAllGuests(event,guestFileName);
					printf("\nSuccessful deletion!");
				}
				break;
				
			case 7:
				break;
				
			default:
				break;
		}
		
	};
	
	return 0;
}
