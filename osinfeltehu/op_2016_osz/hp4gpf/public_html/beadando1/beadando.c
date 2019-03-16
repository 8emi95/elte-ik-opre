#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
struct event{
    int id;
    char name[30];
};

struct visitor{
	int eventId;
    int id; 
    char name[30];
    char email[50];
    char date[30];
};

int stringLength(char myString[50])
{
    int i;
    int l = 0;
    for(i = 0; myString[i] != '\0'; ++i);
    return i;
}

int getNextEventId()
{
    int evNum; 
    int id = 0; 
    int one = 1;
    int i;
    evNum = open("eventAI.dat", O_RDONLY);
    i =  read(evNum, &id, sizeof(int));
    close(evNum);
    evNum = open("eventAI.dat",O_WRONLY | O_CREAT, 0644);
    if(i <= 0){
        write(evNum, &one , sizeof(int));
    }else{
        id++;
        write(evNum, &id, sizeof(int));
    }
    close(evNum);
    return id;
}

void printEvents()
{
    int etxt;
    etxt = open("events.dat", O_RDONLY);
    struct event temp;
    if(etxt > 0){
        printf("Id\tEventName\n");
        while(read(etxt, &temp, sizeof(struct event))) {
            printf("%i\t%s\n", temp.id, temp.name);
        }
    }else{
        printf("Meg nincsenek regisztalt rendezvenyek.\n");       
    }
    close(etxt);
}

void appendEvents()
{
  struct event newEvent; 
	int events;
  int w = 0;
	events = open("events.dat",O_WRONLY | O_APPEND, 0644); //O_WRONLY|O_CREAT|O_TRUNC
	newEvent.id = getNextEventId();
	printf("Mi a rendezveny neve? : ");
	scanf("%s", &newEvent.name); 
	if(write(events, &newEvent, sizeof(struct event)) != sizeof(struct event)){
		perror("There is a mistake in writing\n");
	}
	close(events);	
}

void deleteEvent(int deleteId)
{
	int etxt;
    int temp;
    int i;
    etxt = open("events.dat", O_RDONLY);
    temp = open("temp.dat",O_WRONLY|O_APPEND|O_TRUNC, 0644);
    struct event tempEvent; 
    while(read(etxt, &tempEvent, sizeof(struct event))) {
        if(deleteId != tempEvent.id){
            write(temp, &tempEvent,sizeof(struct event));
        }else{
            printf("DELETED: \n");
            printf("%i\t%s\n", tempEvent.id, tempEvent.name);
        }
    }
    close(etxt);
    close(temp);
    remove("events.dat");
    rename("temp.dat", "events.dat");    
    deleteUsers(deleteId, 0);
}

void deleteUsers(int eventId, int userId)
{
    int vtxt;
    int temp;
    vtxt=open("visitors.dat",O_RDONLY);
    if(vtxt == 0){
      printf("valami nem jó");
        return 1;
    }
    temp = open("temp.dat",O_WRONLY|O_APPEND|O_TRUNC | O_CREAT, 0644);
    struct visitor person;
    while(read(vtxt, &person, sizeof(struct visitor))){
        if((person.eventId == eventId && userId == 0) || (person.eventId == eventId && person.id == userId)) {
            printf("Toroltuk: %i;%i;%s;%s\n",person.eventId,person.id, person.name,person.email);
        }else{
            write(temp, &person, sizeof(struct visitor));
        }
    }
    close(vtxt);
    close(temp);
    remove("visitors.dat");
    rename("temp.dat", "visitors.dat"); 
}

void printUsers(int eventId){
    int vtxt;
    vtxt=open("visitors.dat",O_RDONLY);
    if(vtxt == 0){
        return 1;
    }
    struct visitor person;
    printf("EventID\tUserID\tName\tE-mail\tRegDate\n");
    while(read(vtxt, &person, sizeof(struct visitor))){
        if(person.eventId == eventId) {
            printf("%i\t%i\t%s\t%s\t%s\n", person.eventId, person.id, person.name, person.email, person.date);
        }
    }
    close(vtxt);
}

void editUser(int eventId, int userId){
    int vtxt;
    int temp;
    vtxt=open("visitors.dat",O_RDONLY);
    if(vtxt == 0){
        printf("Nincsenek latogatok");
    }else{
        temp = open("temp.dat",O_WRONLY|O_CREAT|O_TRUNC, 0644);
        struct visitor person;
        while(read(vtxt, &person, sizeof(struct visitor))){
            if(person.eventId == eventId && person.id == userId) {
                printf("Jelenlegi neve: %s\nUj neve: ", person.name);
                scanf("%s", &person.name);
                printf("Jelenlegi e-mail: %s\nUj e-mail cime: ", person.email);
                scanf("%s",&person.email);
            }
            write(temp, &person, sizeof(struct visitor));
        }
    }
    close(vtxt);
    close(temp);
    remove("visitors.dat");
    rename("temp.dat", "visitors.dat");
}

int getNextUserId(int searchEventId)
{
    int newUserId = 1;
    int vtxt;
    vtxt=open("visitors.dat",O_RDONLY);
    struct visitor person;
    if(vtxt > 0){
        while(read(vtxt, &person, sizeof(struct visitor))){
            if(person.eventId == searchEventId){
                if(newUserId <= person.id){
                    newUserId = person.id +1;
                }
            }
        } 
    }
    close(vtxt);
    return newUserId;
}

void userAttendToEvent(int eventId)
{
    struct visitor newPerson;
    time_t rawtime;
    int vtxt;
    newPerson.id = getNextUserId(eventId);
    newPerson.eventId = eventId;
    vtxt=open("visitors.dat",O_WRONLY | O_APPEND | O_CREAT , 0644);
    printf("Neve: ");
    scanf("%s", &newPerson.name);
    printf("E-mail cime: ");
    scanf("%s",&newPerson.email);
    time(&rawtime);
    strcpy(newPerson.date,ctime (&rawtime));
    if(write(vtxt, &newPerson, sizeof(struct visitor)) != sizeof(struct visitor)){
        perror("There is a mistake in writing\n");
    }
    close(vtxt);
    printf("On a %i. felhasznalo, aki regisztralt.\n", newPerson.id);
}

void printUserList(int searchEventId)
{
    int vtxt;
    vtxt=open("visitors.dat",O_RDONLY, 0644);
    struct visitor person;
    if(vtxt > 0){
        printf("EventID\tUserID\tName\tE-mail\t\tRegDate\n");
        while(read(vtxt, &person, sizeof(struct visitor))){
            if(person.eventId == searchEventId){
                printf("%i\t%i\t%s\t%s\t%s\n", person.eventId, person.id, person.name, person.email, person.date);
            }
        }
    }else{
        printf("Meg nincsenek regisztralt latogatok.\n");
    }
    close(vtxt);
}

int main()
{
	int menu = 100;
	int deleteId = 0;
	int eventId;
	int userId;
	//int autoincrement = getNonUsedId();
	while(menu > 0){
		switch(menu){
			case 100:
				break;
			case 1:
				printEvents();
				printf("\n");
				break;
			case 2:
				appendEvents();
				break;
			case 3:
				printf("Melyik rendezvenyt szeretne torolni? \nAzonosito:");
				scanf("%i", &deleteId);
				deleteEvent(deleteId);
        break;
			case 4:
				printEvents();
				printf("Melyik rendezvenyen szeretne resztvenni?\nAzonosito:");
				scanf("%i",&eventId);
                printf("mi a szosz");
				userAttendToEvent(eventId);
        break;
			case 5:
				printEvents();
				printf("Melyik rendezveny latogatoira kivancsi?\nAzonosito:");
				scanf("%i",&eventId);
				printUserList(eventId);
        break;
			case 6: 
				printEvents();
				printf("Melyik rendezveny latogatojat szeretne szerkeszteni? \nAzonosito:");
				scanf("%i",&eventId);
                printUserList(eventId);
				printf("Melyik felhasznalot szeretne szerkeszteni? \nAzonosito:");
				scanf("%i", &userId);
				editUser(eventId, userId);
        break;
			case 7:
                printEvents();
                printf("Melyik rendezveny latogatojat szeretne torolni? \nAzonosito:");
                scanf("%i",&eventId);
                printUserList(eventId);
                printf("Melyik felhasznalot szeretne torolni? \nAzonosito:");
				scanf("%i", &userId);
				deleteUsers(eventId,userId);
        		break;
			default:
				printf("Nincs ilyen menupont.");
				break;
		}
    	printf("King of Stands - rendezveny nyilvantarto\n\n");
		printf("1 - Rendezvenyek listaja,\n");
		printf("2 - Uj rendezveny letrehozasa\n");
		printf("3 - Rendezveny torlese\n");
		printf("4 - Jelentkezes rendezvenyre\n");	
		printf("5 - Latogatok listaja\n");
		printf("6 - Latogato szerkesztese\n");
		printf("7 - Latogato torlese\n");
		printf("0 - Kilepes\n\n");  
		printf("Valasztas: ");
		scanf ("%i",&menu);
	}
  
	return 0;

}