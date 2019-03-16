#include <stdio.h>

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef int bool;
enum { false, true };
int eventCount = 0;

int menu()
{
	printf("King of Stands");
	printf("1 - uj latogato hozzaadasa");
	printf("2 - latogato modositasa");
	printf("3 - latogato torlese");
	printf("4 - uj rendezveny hozzaadasa");
	printf("5 - rendezveny torlese");
	printf("6 - rendezveny resztvevoinek listazasa");
	printf("7 - programbol valo kilepes");

	char numchar[1];
	int number;
	bool valid = false;
	do
	{
		printf("A te valasztasod: ");
		scanf("%s%*c", &numchar);
		number = atoi(numchar);
		if ((number == 0 && *numchar != '0') || (number<= 0 || number > 7))
		{
			printf("Vagy nem szamot adtal meg, vagy nem a megadott intervallumban adtad meg a szamot. Probald ujra!");
			printf("\n");
		}
		else valid = true;

	} while (!valid);
	return number;
}
struct Guests
{
	char name[100];
	char email[100];
	time_t now;
};

struct Events
{
    char eventName[100];
	int guestCount;
	struct Guests* myGuests;
};
struct Events myEvents[10];

int eventMenu()
 {
     if(eventCount==0)
     {
         printf("Meg nincs letrehozva egy esemeny sem! Nyomj egy gombot a folytatashoz!");
         return -1;
     }
     printf("Az osszes esemeny listaja: ");
     int i;
     printf("azonosito\tesemeny neve\n");
     printf("\n");
     for(i=0;i<eventCount;i++)
     {
         printf("%d\t\t%s\n",i+1,myEvents[i].eventName);
     }
     printf("\n");

    char numchar[1];
    int number;
    bool valid = false;
	do
	{
		printf("A te valasztasod: ");
		scanf("%s%*c", &numchar);
		number = atoi(numchar);
		if ((number == 0 && *numchar != '0') || (number<= 0 || number > eventCount))
		{
			printf("Vagy nem szamot adtal meg, vagy nem a megadott intervallumban adtad meg a szamot. Probald ujra! ");
			printf("\n");
		}
		else valid = true;

	} while (!valid);
	return number;
 }

void addEvent() {
	if (eventCount >= 10) {
		printf("Mar nem adhato hozza tobb esemeny! Torolj egy mar meglevot!");
		return;
	}
	eventCount++;
	char eventName[100];
	printf("Esemeny neve: ");
	scanf("%s%*c",eventName);
    printf("\n");
	myEvents[eventCount-1].myGuests = (struct Guests*)malloc(10 * sizeof(struct Guests));
	strcpy(myEvents[eventCount-1].eventName, eventName);
	printf("Esemeny letrehozva!");
	printf("Azonosito: %d\n", eventCount);
	printf("Nyomj meg egy gombot a fomenube valo visszatereshez");
}

void removeEvent() {
	int id = eventMenu();
	if (id != -1)
    {
        myEvents[id-1].guestCount = 0;
        int i;
        for (i = id-1 ; i < eventCount-1; i++ )
        {
            myEvents[i] = myEvents[i+1];
        }
        eventCount--;
        printf("A torles sikeres!");
        return;
    }
	else
    {
        printf("A torles sikertelen! Nyomjon egy gombot a folytatashoz!");
    }
}
int getListOfGuests()
 {
	 int id = eventMenu();
	 if(id==-1)
     {
         return -1;
     }
     if(myEvents[id].guestCount==0)
     {
         printf("Az esemenyhez meg nincs resztvevo!");
     }
     id--;
	 if (id != -1 && myEvents[id].guestCount>0)
	 {
		 printf("Jelentkezettek a %d. azonositoju esemenyhez:\n\n",id+1);
		 int i;
		 for (i = 0; i < myEvents[id].guestCount; i++)
		 {
			 printf("%d. jelentkezo neve: %s\n", i + 1, myEvents[id].myGuests[i].name);
			 printf("%d. jelentkezo e-mail cime: %s\n", i + 1, myEvents[id].myGuests[i].email);
			 printf("%d. jelentkezo jelentkezesenek ideje: %s\n", i + 1, ctime(&myEvents[id].myGuests[i].now));
			 printf("\n");
		 }
	 }
	 return id;
 }
void removeGuest() {
    int id=getListOfGuests();
    if(id==-1)
    {
        return;
    }
    if(myEvents[id].guestCount!=0)
    {
        printf("Az esemenyhez tartozo vendegek");
        char numchar[1];
        int number;
        printf("Torolni kivant vendeg sorszama: ");
        scanf("%s%*c", &numchar);
        number = atoi(numchar);
        if ((number == 0 && *numchar != '0')|| (number > myEvents[id].guestCount)) {
            printf("Vagy nem szamot adtal meg, vagy nem a megadott intervallumban adtad meg a szamot.");
            printf("Nyomj meg egy gombot a fomenube valo visszatereshez.");
            return;
        }
        if (myEvents[id].guestCount > 1)
        {
            int i;
            for (i = number-1 ; i < myEvents[id].guestCount-1; i++ )
            myEvents[id].myGuests[i] = myEvents[id].myGuests[i+1];

        }
        printf("A torles sikeres! Nyomjon egy gombot a folytatashoz!");
        myEvents[id].guestCount--;
    }
    else
    {
        printf("Az esemenyhez meg nincs resztvevo!");
        return;
    }
}

void addGuest() {
	int id = eventMenu();
	if (id == -1) {
		return;
	}
	else
    {
        char name[100];
        char email[100];

        printf("Neve: ");
        scanf("%s",name);

        printf("E-mail cime: ");
        scanf("%s",email);

        id--;
        int allGuests = myEvents[id].guestCount++;

        strcpy(myEvents[id].myGuests[allGuests].name, name);
        strcpy(myEvents[id].myGuests[allGuests].email, email);
        myEvents[id].myGuests[allGuests].now = time(0);
        printf("A jelentezes megtortent a %d. helyen\n",allGuests+1);
        printf("Nyomj meg egy gombot a fomenube valo visszatereshez!");
    }
}
void editGuest() {
    int id=getListOfGuests();
    if(myEvents[id].guestCount!=0)
    {
        printf("Az esemenyhez tartozo vendegek");
        char numchar[1];
        int number;
        printf("Modositani kivant vendeg sorszama: ");
        scanf("%s%*c", &numchar);
        number = atoi(numchar);
        if (number == 0) {
            printf("Nem szamot irtal be!");
            return;
        }
        if (myEvents[id].guestCount > 0)
        {
            number--;
            char name[100];
            char email[100];

            printf("Neve: ");
            scanf("%s",name);

            printf("E-mail cime: ");
            scanf("%s",email);

            strcpy(myEvents[id].myGuests[number].name, name);
            strcpy(myEvents[id].myGuests[number].email, email);

            printf("Modositas sikeres! Nyomjon egy gombot a folytatashoz!");
        }
    }
}

 void fromFile()
 {
	 FILE* file=fopen("savepoint.txt", "r");
	 if (file == NULL)
	 {
		 printf("Hiba tortent a betoltesnel!");
		 return;
	 }
	 fread(&eventCount, sizeof(int), 1, file);
	 int i;
	 for (i = 0; i < eventCount; i++)
	 {
		 fread(&myEvents[i].guestCount, sizeof(int), 1, file);
		 fread(&myEvents[i].eventName, 100,1,file);
		 myEvents[i].myGuests = (struct Guests*)malloc(10 * sizeof(struct Guests));
		 fread(myEvents[i].myGuests, sizeof(*myEvents[i].myGuests), myEvents[i].guestCount, file);
		 fscanf(file,"\n");
	 }
	 fclose(file);
 }
 void toFile()
 {
	 FILE* file = fopen("savepoint.txt", "w");
	 if (file == NULL)
	 {
		 printf("Hiba tortent a mentesnel!");
		 return;
	 }
	 fwrite(&eventCount, sizeof(int), 1, file);
	 int i;
	 for (i=0; i < eventCount; i++)
	 {
		 fwrite(&myEvents[i].guestCount, sizeof(int), 1, file);
		 fwrite(&myEvents[i].eventName, 100,1,file);
		 fwrite(myEvents[i].myGuests, sizeof(*myEvents[i].myGuests), myEvents[i].guestCount, file);
		 fprintf(file, "\n");
	 }
	 fclose(file);
 }

int main()
{
	int option;
	do
	{
		system("cls");
		option = menu();
		fromFile();
		switch (option)
		{
		case 1:
			system("cls");
			printf("Látogato felvetele\n");
			addGuest();
			break;
		case 2:
			system("cls");
			printf("Latogato modositasa\n");
            editGuest();
			break;
		case 3:
			system("cls");
			printf("Latogato torlese\n");
            removeGuest();
			break;
		case 4:
			system("cls");
			printf("Esemeny letrehozasa\n");
			addEvent();
			break;
		case 5:
			system("cls");
			printf("Esemeny torlese\n");
			removeEvent();
			break;
		case 6:
			system("cls");
			printf("Esemeny latogatoinak listazasa\n");
			getListOfGuests();
			printf("A folytatashoz nyomj meg egy gombot!");
			break;
		case 7:
			exit(0);
		}
		//_getch();
		toFile();
	} while (option!=7);
    return 0;
}
