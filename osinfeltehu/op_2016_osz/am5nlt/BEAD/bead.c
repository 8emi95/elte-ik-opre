/*
* 	Klemanovits Ádám [AM5NLT]
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void getListGuests(const int id)
{
    FILE *guests = fopen("guest.txt", "r");
    char line[512];

    int i = 0;
    while (fgets(line, sizeof(line), guests) != NULL) {
        int eventId;
        char lastName[50];
        char firstName[50];
        char email[80];
        time_t registrationTime;
        int rankingNumber;

        sscanf(line, "%d %s %s %s %d %d", &eventId, lastName, firstName, email, &registrationTime, &rankingNumber);

        if (id == eventId)
        {
            i++;
            printf("%d.)\n", i);
            printf("\tAzonosito - %d\n", eventId);
            printf("\tVezeteknev - %s\n", lastName);
            printf("\tKeresztnev - %s\n", firstName, email, ctime(&registrationTime), rankingNumber);
            printf("\tE-mail cim - %s\n", email);
            printf("\tRegisztracio idopontja - %s", ctime(&registrationTime));
            printf("\tSorszam - %d\n", rankingNumber);
        }
    }

    fclose(guests);
}

void getList()
{
    FILE *events = fopen("event.txt", "r");
    char line[512];

    printf("Jelentkezesek listazasa\n");

    while (fgets(line, sizeof(line), events) != NULL) {
        int id;
        char name[100];
		
		sscanf(line, "%d %s", &id, name);
		printf("%d %s\n", id, name);
			
		getListGuests(id);
    }
    
    fclose(events);
}

void registration()
{
    FILE *events = fopen("event.txt", "r");
    int isFound = 0;
    char line[512];

    printf("Regisztacio\n");

    int eventId;
    printf("Esemeny azonositoja: ");
    scanf("%d", &eventId);



    while (fgets(line, sizeof(line), events) != NULL) {
        int searchingEventId;
        sscanf(line,"%d",&searchingEventId);

        if (searchingEventId == eventId) {
            isFound = 1;
            break;
        }
    }
    
    fclose(events);

    if (!isFound) {
        printf("Nem talalhato ilyen esemeny!\n");
    } else {
        FILE *guests = fopen("guest.txt", "a+");

        char lastName[50];
        char firstName[50];
        char email[80];
        time_t registrationTime;
        int rankingNumber;

        printf("Vezetekneve: ");
        scanf("%s", lastName);
        printf("Keresztneve: ");
        scanf("%s", firstName);
        printf("E-mail cime: ");
        scanf("%s", email);
        time(&registrationTime);
        printf("Regisztracio idopontja: %s", ctime(&registrationTime));
        rankingNumber = countRanking(eventId);
        printf("Sorszam: %d\n", rankingNumber);
        
        fprintf(guests, "%d %s %s %s %d %d\n", eventId, lastName, firstName, email, registrationTime, rankingNumber);
        fclose(guests);
    }
}

void copyFileGuest(const char *from, const char *to)
{
    FILE *fr = fopen(from,"r");
    FILE *t = fopen(to,"w");
    char line[512];
    int eventId;
    char lastName[50];
    char firstName[50];
    char email[80];
    time_t registrationTime;
    int rankingNumber;

    while(fgets(line, sizeof(line), fr) != NULL)
    {
        sscanf(line, "%d %s %s %s %d %d", &eventId, lastName, firstName, email, &registrationTime, &rankingNumber);
        fprintf(t, "%d %s %s %s %d %d\n", eventId, lastName, firstName, email, registrationTime, rankingNumber);
    }
    
    fclose(fr);
    fclose(t);
    
    remove("helpfileGuest.txt");
}

void modifyGuest()
{
    FILE *guests = fopen("guest.txt","r");
    FILE *helpfileGuest = fopen("helpfileGuest.txt","w");
    char line[512];
    int isFound = 0;
	int searchingEventId;
    char searchingLastName[50];
    char searchingFirstName[50];
	
    printf("Vendeg adatainak modositasa\n");
    printf("\n\n");
    printf("Esemeny azonositoja: ");
    scanf("%d", &searchingEventId);
    printf("Vezetekneve: ");
    scanf("%s", searchingLastName);
    printf("Keresztneve: ");
    scanf("%s", searchingFirstName);

    while (fgets(line, sizeof(line), guests) != NULL) {
        int eventId;
        char lastName[50];
        char firstName[50];
        char email[80];
        time_t registrationTime;
        int rankingNumber;

        sscanf(line, "%d %s %s %s %d %d", &eventId, lastName, firstName, email, &registrationTime, &rankingNumber);  

        if (eventId == searchingEventId && strcmp(lastName, searchingLastName) == 0 && strcmp(firstName, searchingFirstName) == 0) {
            isFound = 1;
            int menu;
            
            printf("Mit szeretnel modositani?\n");
            printf("1. Vezetekneve\n");
            printf("2. Keresztneve\n");
            printf("3. E-mail cime\n");
            
            printf("Kerem valasszon egy menupontot(1-3): ");
            scanf("%d", &menu);
            
            char newLastName[50];
            char newFirstName[50];
            char newEmail[80];

            switch (menu) {
                case 1:
                    printf("Uj vezeteknev: ");
                    scanf("%s",newLastName);
                    fprintf(helpfileGuest, "%d %s %s %s %d %d\n", eventId, newLastName, firstName, email, registrationTime, rankingNumber);
                break;
                case 2:
                    printf("Uj keresztnev: ");
                    scanf("%s",newFirstName);
                    fprintf(helpfileGuest, "%d %s %s %s %d %d\n", eventId, lastName, newFirstName, email, registrationTime, rankingNumber);
                break;
                case 3:
                    printf("Uj e-mail cim: ");
                    scanf("%s",newEmail);
                    fprintf(helpfileGuest, "%d %s %s %s %d %d\n", eventId, lastName, firstName, newEmail, registrationTime, rankingNumber);
                break;
                default:
                    printf("Nincs ilyen menupont!\n");
                break;
            }
        } else {
            fputs(line, helpfileGuest);            
        }
    }

    if (isFound) {
        printf("Sikeres modositas!\n");
    } else {
        printf("Nem talalhato ilyen vendeg!\n");
    }

    fclose(guests);
    fclose(helpfileGuest);

    copyFileGuest("helpfileGuest.txt","guest.txt");
}

void deleteGuest()
{
    FILE *guests = fopen("guest.txt","r");
    FILE *helpfileGuest = fopen("helpfileGuest.txt","w");
    char line[512];
    int isFound = 0;
	int eventId;
    char lastName[50];
    char firstName[50];
	
    printf("Vendeg torlese\n");
    printf("\n\n");
    printf("Esemeny azonositoja: ");
    scanf("%d", &eventId);
    printf("Vezetekneve: ");
    scanf("%s", lastName);
    printf("Keresztneve: ");
    scanf("%s", firstName);

    while (fgets(line, sizeof(line), guests) != NULL) {
        int delEventId;
        char delLastName[50];
        char delFirstName[50];
        sscanf(line,"%d %s %s",&delEventId, delLastName, delFirstName);

        if (delEventId == eventId && strcmp(delLastName, lastName) == 0 && strcmp(delFirstName, firstName) == 0) {
            isFound = 1;
        }

        if (delEventId != eventId || strcmp(delLastName, lastName) != 0 || strcmp(delFirstName, firstName) != 0) {
            fputs(line, helpfileGuest);
        }
    }

    if (isFound) {
        printf("Sikeres torles!\n");
    } else {
		printf("Nem talalhato ilyen vendeg!\n");
    }
    
    fclose(guests);
    fclose(helpfileGuest);

    copyFileGuest("helpfileGuest.txt","guest.txt");
}

void createEvent()
{
    FILE *events = fopen("event.txt", "a+");
    char line[512];
    int isFound = 0;
	int id;
    char name[100];
	
	printf("Uj esemeny\n");
    printf("\n\n");
    printf("Az esemeny azonositoja: ");
    scanf("%d", &id);

    while (fgets(line, sizeof(line), events) != NULL) {
        int searchingEventId;
        sscanf(line,"%d", &searchingEventId);

        if (searchingEventId == id) {
            isFound = 1;
        }
    }

    if (isFound)
    {
        printf("Ilyen azonositoval mar lett letrehozva esemeny!\n");
    } else {
        printf("Az esemeny neve: ");
        scanf("%s", name);
        fprintf(events, "%d %s\n", id, name);    
    }

    fclose(events);
}

void copyFileEvent(const char *from, const char *to)
{
    FILE *fr = fopen(from,"r");
    FILE *t = fopen(to,"w");
    char line[512];
    int eventId;
    char name[100];

    while(fgets(line, sizeof(line), fr) != NULL)
    {
        sscanf(line, "%d %s", &eventId, name);
        fprintf(t, "%d %s\n", eventId, name);
    }
    
    fclose(fr);
    fclose(t);
    
    remove("helpfileEvent.txt");
}

void deleteEventGuests(const int eventId)
{
    FILE *guests = fopen("guest.txt","r");
    FILE *helpfileGuest = fopen("helpfileGuest.txt","w");
    char line[512];

    while (fgets(line, sizeof(line), guests) != NULL) {
        int delEventId;
        sscanf(line,"%d",&delEventId);

        if (delEventId != eventId) {
            fputs(line, helpfileGuest);
        }
    }
    
    fclose(guests);
    fclose(helpfileGuest);

    copyFileGuest("helpfileGuest.txt","guest.txt");
}

void deleteEvent()
{
    FILE *events = fopen("event.txt","r");
    FILE *helpfileEvent = fopen("helpfileEvent.txt","w");
    char line[512];
    int isFound = 0;
	int eventId;

	printf("Esemeny torlese\n");
    printf("\n\n");
    printf("Esemeny azonositoja: ");
    scanf("%d", &eventId);

    while (fgets(line, sizeof(line), events) != NULL) {
        int delEventId;
        sscanf(line,"%d", &delEventId);

        if (delEventId == eventId) {
            isFound = 1;
        }

        if (delEventId != eventId) {
            fputs(line, helpfileEvent);
        }
    }

    deleteEventGuests(eventId);

    if (isFound) {
		printf("Sikeres torles!\n");
    } else {
        printf("Nem talalhato ilyen esemeny!\n");
    }

    fclose(events);
    fclose(helpfileEvent);

    copyFileEvent("helpfileEvent.txt","event.txt");
}

int countRanking(const int searchingEventId)
{
    FILE *guests = fopen("guest.txt","r");
    char line[512];
    int rankingNumber = 1;

    while (fgets(line, sizeof(line), guests) != NULL) {
        int eventId;
        sscanf(line,"%d",&eventId);

        if (searchingEventId == eventId) {
            rankingNumber++;
        }
    }

    return rankingNumber;
}

void menu() 
{
    int isEnd;

    do {
		printf("--\n");
        printf("Kings of Stands - Kiallitas\n");
        printf("\n");
        printf("1. - Jelentkezesek listazasa\n");
        printf("2. - Vendeg jelentkezese\n");
        printf("3. - Vendeg adatainak modositasa\n");
        printf("4. - Vendeg torlese\n");
        printf("5. - Uj rendezveny inditasa\n");
        printf("6. - Rendezveny torlese\n");
        printf("0. - Kilepes\n");
        printf("\n");
        printf("Kerem valasszon egy menupontot(0-6): ");
        scanf("%d", &isEnd);

        switch (isEnd) {
            case 1:
                getList();
            break;
            case 2:
                registration();
            break;
            case 3:
                modifyGuest();
            break;
            case 4:
                deleteGuest();
            break;
            case 5:
                createEvent();
            break;
            case 6:
                deleteEvent();
            break;
            case 0:
            break;
            default:
                printf("Nincs ilyen menupont!\n");
            break;
        }
    } while (isEnd != 0);
}

int main(int argc, char const *argv[])
{
    menu();

    return 0;
}