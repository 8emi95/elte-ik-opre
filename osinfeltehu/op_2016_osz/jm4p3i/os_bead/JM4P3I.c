#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define NAME_LENGTH   50
#define EMAIL_LENGTH  50
#define DATE_LENGTH  10

typedef enum { false, true } bool;
typedef enum { LIST, NEW_USER, MODIFY_USER, DELETE_USER, NEW_EVENT, DELETE_EVENT } actions;

typedef struct {
	int user_id;
    char name[NAME_LENGTH];
    char email[EMAIL_LENGTH];
	int event_id;
} user_t;

typedef struct {
  	int index;
 	user_t user;
  	struct userListElement *next;
} userListElement;

typedef struct {
  	int count;
  	userListElement *first;
  	userListElement *current;
} userList;

typedef struct {
	char name[NAME_LENGTH];
	int id;
	userList users;
} event_t;

typedef struct {
  	int index;
  	event_t event;
	struct eventListElement *next;
} eventListElement;

typedef struct {
	int count;
  	eventListElement *first;
  	eventListElement *current;
} eventList;


/*General functions*/
void menu(eventList *events);
/* Menu actions*/
void register_user();
void modify_user_data();
void add_event();
void delete_user();
/*Console dialog*/
user_t request_registration();
event_t request_new_event();
user_t request_user_modification();
/*Console scan helpers*/
void read_text(char *text, char *label, int maxlength);
void read_integer(int *num, char *label);
/*Actions*/
int insertNewUser(user_t user);
int insertNewEvent(event_t event);
int removeUser(int id);
/*File IO helpers*/
int getLastUserId(FILE * fp);
int getLastEventId(FILE * fp);
/* Read list from file, list manipulation*/
void read_file(eventList *events);
void addEventToList(eventList *list, event_t *event);
void addUserToList(eventList *list, user_t *user);
void displayList(eventList *list);
user_t getUserData(int id);
/* Debug helper */
void print_users();

void menu(eventList *events) {
    int choice;

    do {
		printf("Kérem válasszon az alábbi menüpontok közül\n\n");
		printf("1. Jelentkezés\n");
		printf("2. Listázás\n");
		printf("3. Jelentkezett adatainak módosítása\n");
		printf("4. Jelentkezett törlése\n");
		printf("5. Új rendezvény indítása\n");
		/*printf("6. Rendezvény törlése\n");*/
		printf("7. Kilépés\n");
		
		read_integer(&choice, "Válasszon az alábbi lehetőségek közül:");
		
		switch(choice) {
		  case 1:
			register_user();
			break;
		  case 2:
		  	read_file(events);
			break;
		  case 3:
			modify_user_data();
			break;
		  case 4:
			delete_user();
			break;
		  case 5:
			add_event();
			break;
		  /*case 6:
			delete_event();
			break;*/
		  case 111:
		  	print_users();
		  	break;
		  case 7:
			exit(0);
			break;
		  default:
			printf("Nem létező menüpont!\n");
			break;
		}
    } while ( choice != 7);
}
/* Functions */
void register_user() {
	user_t user;	
	user = request_registration();
	int id = insertNewUser(user);
	if(id >= 0) printf("Az regisztráció azonosítója: %d\n\n", id);
}

void modify_user_data() {
	user_t user;
	user = request_user_modification();
	if(user.user_id >= 0) {
		int id = editUserData(user);
		if(id >= 0) printf("A módosított regisztráció azonosítója: %d\n\n", id);
	}
}

void delete_user() {
	int user_id;
	read_integer(&user_id, "\nKérem adja a törölni kívánt felhasználó azonosítóját:");
	user_id = removeUser(user_id);
	if(user_id>=0)	printf("ID: %d felhasználó törölve\n\n", user_id);
}

void add_event() {
	event_t event;	
	event = request_new_event();
	int id = insertNewEvent(event);	
	if(id >= 0) printf("Az esemény azonosítója: %d\n\n", id);
}

/* Console interaction queries */
user_t request_registration() {
  char label[100];
  user_t user;

  sprintf(label, "\nKérem adja meg a nevét (max %d karakter):", NAME_LENGTH);
  read_text(user.name, label, (int)NAME_LENGTH);
  
  sprintf(label, "\nKérem adja meg az e-mail címét (max %d karakter):", EMAIL_LENGTH);
  read_text(user.email, label, (int)EMAIL_LENGTH);

  read_integer(&user.event_id, "\nKérem adja a rendezvény azonosítóját:");
  
  printf("\n");
  return user;
}

user_t request_user_modification() {
	user_t user;

	int user_id;
	read_integer(&user_id, "\nKérem adja meg a módosítani kívánt regisztráció azonosítóját:");
	printf("\n");

	user = getUserData(user_id);
	if(user.user_id <= 0) {
		printf("A keresett regisztráció nem található az adatbázisban!\n");
		return user;
	} else {
		printf("A regisztráció adatai: \n");
		printf("Azonosító: %d\n",user.user_id);
		printf("Név: %s\n",user.name);
		printf("E-mail cím: %s\n",user.email);
		printf("Esemény azonosítója: %d\n",user.event_id);

		user_t tmp;
		tmp = request_registration();
		tmp.user_id = user_id;
		return tmp;
	}

}

event_t request_new_event() {
	char label[100];
  	event_t event;	

    sprintf(label, "\nKérem adja meg a rendezvény nevét (max %d karakter):", NAME_LENGTH);
  	read_text(event.name, label, (int)NAME_LENGTH);	

  	printf("\n");
  	return event;
}

/* Console scanner helpers */
void read_text(char *text, char *label, int maxlength) {	
	char c;	
	int cnt = 0;
	bool ws = false;
  
	printf("%s", label);
	scanf( "%s",  text);
  
	while( (c=getchar())!=(int)'\n' && c != EOF ) {
		if (c == ' ')
			ws = true;
		if (ws == false && cnt < sizeof(text)) {    
			text[cnt] = c;
			cnt++;
		}
	}
  
	if (maxlength != 0 && (int)strlen(text) > maxlength) {
		printf("Hiba: A szöveg hossza maximum %d karakter lehet \n", maxlength);
		read_text(text, label, maxlength);
	}
}

void read_integer(int *num, char *label) {
	printf("%s", label);

    if ( scanf("%d", num) == 1 ) {
       return;
	} else {
        scanf("%*s");
        printf("Hiba: Nem megfelelő számformátum!\n");
        read_integer(num, label);
    }
}

void print_users() {
	int pos = 2;
	FILE * fp_user;
	fp_user = fopen("users.db","rb+");
	
	if (fp_user == NULL) {
		printf("A users.db fájl megnyitása sikertelen!\n\n");
	} else {
		user_t user;
		fseek(fp_user, sizeof(user_t), SEEK_SET);
		while ( fread(&user, sizeof(user_t), 1, fp_user) == 1 ) {
			fseek(fp_user, pos*sizeof(user_t),SEEK_SET);
			pos++;
			//fseek(fp_user, (pos+1)*sizeof(user_t),SEEK_SET);
			printf("%d. %s (%s)\n", user.user_id, user.name, user.email);
		}
		fclose(fp_user);
	}
}

void read_file(eventList *events) {
	events->first = NULL;
  	events->current = NULL;
  	events->count = 0;
	//Read events
	FILE * fp_event;
	fp_event = fopen("events.db","rb+");
	int pos = 1;
	if (fp_event == NULL) {
		printf("Az events.db fájl megnyitása sikertelen!\n\n");
	} else {
		event_t event;
		fseek(fp_event , sizeof(event_t), SEEK_SET);
		while ( fread(&event, sizeof(event_t), 1, fp_event) == 1 ) {
			fseek(fp_event, pos*sizeof(event_t),SEEK_SET);
			pos++;
			fseek(fp_event, (pos+1)*sizeof(event_t),SEEK_SET);
			addEventToList(events, &event);
		}
		fclose(fp_event);
	}

	//Reading users
	pos = 1;
	FILE * fp_user;
	fp_user = fopen("users.db","rb+");
	
	if (fp_user == NULL) {
		printf("A users.db fájl megnyitása sikertelen!\n\n");
	} else {
		user_t user;
		fseek(fp_user, sizeof(user_t), SEEK_SET);
		while ( fread(&user, sizeof(user_t), 1, fp_user) == 1 ) {
			fseek(fp_user, pos*sizeof(user_t),SEEK_SET);
			pos++;
			fseek(fp_user, (pos+1)*sizeof(user_t),SEEK_SET);
			addUserToList(events, &user);
		}
		fclose(fp_user);
	}
	displayList(events);
}

user_t getUserData(int id) {
	int pos = 1;
	FILE * fp_user;
	fp_user = fopen("users.db","rb+");
	user_t user;
	if (fp_user == NULL) {
		printf("A users.db fájl megnyitása sikertelen!\n\n");
	} else {
		
		fseek(fp_user, sizeof(user_t), SEEK_SET);
		while ( fread(&user, sizeof(user_t), 1, fp_user) == 1 ) {
			if(user.user_id == id) {
				fclose(fp_user);
				return user;
			}
			fseek(fp_user, pos*sizeof(user_t),SEEK_SET);
			pos++;
			fseek(fp_user, (pos+1)*sizeof(user_t),SEEK_SET);
		}
		fclose(fp_user);
	}
	user.user_id = -1;
	return user;
}

void addEventToList(eventList *list, event_t *event) {	
  	eventListElement *element = (eventListElement*)malloc(sizeof(eventListElement));

  	if (element == NULL) {
		printf("Memórifoglalás során hiba lépett fel!\n");
		exit(1);
  	}
  
  	if (list->first == NULL) {
		list->first = element;
		element->next = NULL;
    } else {
	    list->current->next = (struct eventListElement*)element;
	    list->current = element;
    }
  
    list->count++;
    list->current = element;

    element->event=*event;
    element->index=list->count;

    userList users;
    users.first = NULL;
    users.current = NULL;
    users.count = 0;
    element->event.users = users;
}

void addUserToList(eventList *list, user_t *user) {
	//Find the event with the correct id 
	eventListElement *tmp = list->first;
	while(tmp != NULL) {
		if(tmp->event.id == user->event_id) {
			//Correct id
			userListElement *element = (userListElement*)malloc(sizeof(userListElement));
			if (element == NULL) {
				printf("Memórifoglalás során hiba lépett fel!\n");
				exit(1);
		  	}

			if(tmp->event.users.first == NULL) {
				tmp->event.users.current = NULL;
				tmp->event.users.count = 0;

				tmp->event.users.first = element;
				element->next = NULL;
			} else {
				tmp->event.users.current->next = (struct userListElement*)element;
			}
			tmp->event.users.count++;
			tmp->event.users.current = element;
		  	
		  	element->user=*user;
		  	element->index=tmp->event.users.count;
		  	break;
		} else {
			tmp = (eventListElement*)tmp->next;
		}
	}
}

void displayList(eventList *list) {
	eventListElement *element = list->first;
	if(element == NULL) {
		printf("Nincsenek tárolt események!");
	} else {
		system("clear");
		printf("*************\n");
		printf("* Események *\n");
		printf("*************\n\n");
	}
	
	while(element != NULL) {
		printf("ID: %d\n", element->event.id);
		printf("Esemény neve: %s\n", element->event.name);
		/*loop users if exist*/
		if(element->event.users.first != NULL) {
			userListElement *user = element->event.users.first;
			while(user != NULL) {
				printf("\tNév: %s (ID: %d)\n", user->user.name, user->user.user_id);
				printf("\tE-mail: %s\n", user->user.email);
				printf("\t------------------------------\n");
				user = (userListElement*)user->next;
			}
		}
		printf("\n\n");
		element = (eventListElement*)element->next;
	}
	
	printf("\n");
}

/* File IO helper functions */
int getLastUserId(FILE * fp) {
	user_t tmp_user;
	//Beginning of the file
	fseek(fp, sizeof(tmp_user), SEEK_SET);
	int pos = 1;
	int lastId = -1;
	while(fread(&tmp_user, sizeof(tmp_user), 1, fp) == 1) {
		fseek(fp, pos*sizeof(tmp_user),SEEK_SET);
		pos++;
		fseek(fp, (pos+1)*sizeof(tmp_user), SEEK_SET);
		lastId = tmp_user.user_id;
	}
	return lastId;
}

int getLastEventId(FILE * fp) {
	event_t tmp_event;
	//Beginning of the file
	fseek(fp, sizeof(tmp_event), SEEK_SET);
	int pos = 1;
	int lastId = -1;
	while(fread(&tmp_event, sizeof(tmp_event), 1, fp) == 1) {
		fseek(fp, pos*sizeof(tmp_event),SEEK_SET);
		pos++;
		fseek(fp, (pos+1)*sizeof(tmp_event), SEEK_SET);
		lastId = tmp_event.id;
	}
	return lastId;
}


/* File IO actions */
int insertNewUser(user_t user) {
	FILE * fp_user;
	
	fp_user = fopen("users.db","r+b");
	
	int lastUserId = -1;
	if (fp_user == NULL) {
		printf("A users.db fájl megnyitása sikertelen!\n\n");
	} else {
		lastUserId = getLastUserId(fp_user);
		user.user_id = ++lastUserId;
		fwrite(&user, sizeof(user), 1, fp_user);
		fclose(fp_user);
		printf("Új regisztráció mentése sikeres!\n");
	}
	
	return lastUserId;
}

int insertNewEvent(event_t event) {
	FILE * fp_event;
	
	fp_event = fopen("events.db","r+b");
	
	int lastEventId = -1;
	if (fp_event == NULL) {
		printf("Az events.db fájl megnyitása sikertelen!\n\n");
	} else {
		lastEventId = getLastEventId(fp_event);
		event.id = ++lastEventId;
		fwrite(&event, sizeof(event), 1, fp_event);
		fclose(fp_event);
		printf("Új Esemény mentése sikeres!\n");
	}
	
	return lastEventId;
}


int removeUser(int id) {
	FILE * fp_user;

	fp_user = fopen("users.db", "r+b");
	int lastId = -1;

	if (fp_user == NULL) {
		printf("A users.db fájl megnyitása sikertelen!\n\n");
	} else {
		user_t tmp_user;
		//Beginning of the file
		fseek(fp_user, sizeof(user_t), SEEK_SET);
		int pos = 1;
		
		FILE * fp_tmp;
		fp_tmp = fopen("users_tmp.db", "w");

		if (fp_tmp == NULL) {
			printf("Új ideiglenes fájl létrehozása sikertelen!\n\n");
			fclose(fp_user);
		} else {
			//fseek(fp_tmp, sizeof(user_t), SEEK_SET);
			while(fread(&tmp_user, sizeof(user_t), 1, fp_user) == 1) {
				fseek(fp_user, pos*sizeof(user_t), SEEK_SET);
				
				if(tmp_user.user_id == id) {
					lastId = tmp_user.user_id;
				} else {
					printf("%s\n", tmp_user.name);
					fwrite(&tmp_user, sizeof(user_t), 1, fp_tmp);
				}

				pos++;
				//fseek(fp_user, (pos+1)*sizeof(tmp_user), SEEK_SET);
			}
			//truncate("users.db", sizeof(tmp_user)*pos);
			fclose(fp_tmp);
			fclose(fp_user);
			remove("users.db");
			rename("users_tmp.db", "users.db");
		}		
	}

	return lastId;
}

int editUserData(user_t user) {
	FILE * fp_user;

	fp_user = fopen("users.db", "r+b");
	int lastId = -1;

	if (fp_user == NULL) {
		printf("A users.db fájl megnyitása sikertelen!\n\n");
	} else {
		user_t tmp_user;
		//Beginning of the file
		fseek(fp_user, sizeof(tmp_user), SEEK_SET);
		int pos = 0;
		while(fread(&tmp_user, sizeof(user_t), 1, fp_user) == 1) {
			fseek(fp_user, pos*sizeof(user_t), SEEK_SET);
			if(tmp_user.user_id == user.user_id) {
				lastId = tmp_user.user_id;
				fwrite(&user, sizeof(user_t), 1, fp_user);
			} else {
				fwrite(&tmp_user, sizeof(user_t), 1, fp_user);
			}
			pos++;
			fseek(fp_user, (pos+1)*sizeof(user_t), SEEK_SET);
		}
		fclose(fp_user);
		truncate("users.db", sizeof(user_t)*pos);
		
	}
	return lastId;
}
/*void write_file(actions action, user_t user, int id) {
    FILE * fp_user;
    FILE * fp_event;
  
    if (action == NEW_USER) {
		fp_user = fopen("users.db","r+b");
		
		if (fp_user == NULL) {
			printf("A users.db fájl megnyitása sikertelen!\n\n");
	    } else {
			int lastUserId = getLastUserId(&fp_user);
			fwrite(&user, sizeof(user), 1, fp_user);
			fclose(fp_user);
			printf("Új regisztráció mentése sikeres!\n");
		}
	} else if(action == NEW_EVENT)  {
		fp_event = fopen("events.db","a+b");
		
		if (fp_event == NULL) {
			printf("Az events.db fájl megnyitása sikertelen!\n\n");
	    } else {
			fwrite(&event, sizeof(event_t), 1, fp_user);
			fclose(fp_event);
			printf("Új esemény mentése sikeres!\n");
		}
	} else if(action == MODIFY_USER) {
		fp_user  = fopen("users.db" ,"a+b");
		//fp_event = fopen("events.db","r+b");
		
		if (fp_event == NULL) {
			printf("Az events.db fájl megnyitása sikertelen!\n\n");
	    } else if(fp_user == NULL) {
			printf("A users.db fájl megnyitása sikertelen!\n\n");
		} else {
			//TODO: check if event id exists, step-bye-step instead of seek
			//modify user
			if (fseek( fp_user, (id - 1)*sizeof(user), SEEK_SET ) == 0) {
				fwrite(&user, sizeof(user), 1, fp_user);
				fflush(fp_user);
				
				fclose(fp_user);
				fclose(fp_event);
				printf("Regisztráció módosítása sikeres!\n");
			} else {
				printf("A módosítandó regisztráció nem található!");
			}
		}
	} else if(action == DELETE_USER) {
		fp_user  = fopen("users.db" ,"r+b");
		
		if(fp_user == NULL) {
			printf("A users.db fájl megnyitása sikertelen!\n\n");
		} else {
			int pos = id - 1;
			user_t tmp_user;
			fseek(fp_user, id*sizeof(tmp_user), SEEK_SET);
			while(fread(&tmp_user, sizeof(tmp_user), 1, fp_user) == 1) {
				fseek(fp_user, pos*sizeof(tmp_user),SEEK_SET);
				fwrite(&tmp_user, sizeof(tmp_user),1, fp_user);
				pos++;
				fseek(fp_user, (pos+1)*sizeof(tmp_user), SEEK_SET);		
			}
			truncate("users.db", sizeof(tmp_user)*pos);
			
			fclose(fp_user);
			printf("Regisztráció törlése sikeres!\n");
		}
	} else if(action == DELETE_EVENT) {
		fp_user  = fopen("users.db" ,"r+b");
		fp_event = fopen("events.db","r+b");
		
		if (fp_event == NULL) {
			printf("Az events.db fájl megnyitása sikertelen!\n\n");
	    } else if(fp_user == NULL) {
			printf("A users.db fájl megnyitása sikertelen!\n\n");
		} else {
			//TODO fseek es pos helyett, leptetes + id
			
			
			int user_pos = user_id - 1;
			int event_pos = event_id - 1;
			
			
			//Find and remove event
			event_t tmp_event;
			
			fseek(fp_event, id*sizeof(event_t), SEEK_SET);
			while(fread(&tmp_event, sizeof(tmp_event), 1, fp_event) == 1) {
				fseek(fp_event, pos*sizeof(tmp_event),SEEK_SET); //fseek helyett ellenorzes + leptetes
				fwrite(&tmp_event, sizeof(tmp_event),1, fp_event);
				event_pos++;
				fseek(fp_event, (event_pos+1)*sizeof(tmp_event), SEEK_SET);		
			}
			truncate("events.db", sizeof(tmp_event)*event_pos);
			fclose(fp_event);
			
			//Remove registrations that belonged to the event
			user_t tmp_user;
			
			fseek(fp_user, id*sizeof(user_t), SEEK_SET);
			while(fread(&tmp_user, sizeof(tmp_user), 1, fp_user) == 1) {
				fseek(fp_user, user_pos*sizeof(tmp_user),SEEK_SET); //fseek helyett ellenorzes + leptetes
				fwrite(&tmp_user, sizeof(tmp_user),1, fp_user);
				user_pos++;
				fseek(fp_user, (user_pos+1)*sizeof(tmp_user), SEEK_SET);		
			}
			truncate("users.db", sizeof(tmp_user)*user_pos);
			fclose(fp_user);
			
			printf("Esemény törlése sikeres!\n");
		}
	}
}*/

int main() {
  system("clear");

  eventList events;
  events.first = NULL;
  events.current = NULL;
  events.count = 0;
  menu(&events);
  
  return 0;
}