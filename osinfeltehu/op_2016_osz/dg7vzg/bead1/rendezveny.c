#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef int bool;
#define true 1
#define false 0

struct Personstr
{
    char name[500];
    char email[500];
    time_t time;
};
typedef struct Personstr Person;

struct Eventstr
{
    char name[500];
    int id;
    int guestCount;
    Person guests[1000];
};
typedef struct Eventstr Event;

struct Databasestr
{
    int eventCounter;
    Event events[1000];  
};
typedef struct Databasestr Database;

Database datab;

void Load()
{
    FILE* f;
    f = fopen("jelentkezesek.txt", "r");

    if (f == NULL){
        printf("Hiba a betoltesnel");
        return;
    }

    fread( &datab.eventCounter, sizeof(int), 1, f);
printf("%d", datab.eventCounter);
    int i;
    for (i = 0; i < datab.eventCounter; i++) {
        fread( &datab.events[i].name,  sizeof(char) , strlen(datab.events[i].name) , f); 
        fread( &datab.events[i].id, sizeof(int), 1, f);      
        fread( &datab.events[i].guestCount, sizeof(int), 1, f);
        int j;
        for( j = 0; j < datab.events[i].guestCount; ++j )
        {
            fread( &datab.events[i].guests[j].name, sizeof( char) ,strlen(datab.events[i].guests[j].name) , f);
            fread( &datab.events[i].guests[j].email, sizeof( char ) ,strlen(datab.events[i].guests[j].email) , f);
            fread(&datab.events[i].guests[j].time , sizeof(time_t), 1, f);
        }
    }

}

void Save()
{
    FILE* f;
    f = fopen("jelentkezesek.txt", "w");

    if (f == NULL){
        printf("Hiba a mentesnel");
        return;
    }

    fwrite( &datab.eventCounter, sizeof(int), 1, f);

    int i;
    for (i = 0; i < datab.eventCounter; i++) {
        fwrite( &datab.events[i].name,  sizeof(char) , strlen(datab.events[i].name) , f); 
        fwrite( &datab.events[i].id, sizeof(int), 1, f);      
        fwrite( &datab.events[i].guestCount, sizeof(int), 1, f);
        int j;
        for( j = 0; j < datab.events[i].guestCount; ++j )
        {
            fwrite(&datab.events[i].guests[j].name, sizeof( char) ,strlen(datab.events[i].guests[j].name) , f);
            fwrite(&datab.events[i].guests[j].email, sizeof( char ) ,strlen(datab.events[i].guests[j].email) , f);
            fwrite(&datab.events[i].guests[j].time , sizeof(time_t), 1, f);
        }
    }

    fclose(f);

}


int getEventId()
{
    bool correct;
    int id;
    char ids[500];

    do 
    {
        correct = true;
        printf("\n Rendezveny azonosito: ");
        scanf("%s",&ids);

        id = atoi(ids);

        if ( id == 0 && *ids != '0')
            correct = false;

    } while ( correct == false );
    return id;
}

bool checkId( int id)
{   
    if ( id > datab.eventCounter )
    {
        printf ( "Nincs ilyen esemeny. \n" );
        return false;
    }
    if ( datab.events[id].id == -1 )
    {
        printf("Az esemeny mar torolve lett. \n");
        return false;
    }  
    return true;
}

int FindApply( char* name, char* email, Event* actEv )
{
    int i;
    for ( i= 0; i < actEv->guestCount; i++)
    {
        if (    strcmp (actEv->guests[i].name, name) == 0 &&  
                strcmp (actEv->guests[i].email, email) == 0 )
        {
            return i;
        }
    }
        return -1;
}


void newEvent()
{
    if (datab.eventCounter >= 1000)
    {
        printf("Betelt a terem, nincs hely tobb rendezvenynek.\n");
        return;
    }

    char eventname[500];
    printf("Kerem adja meg a rendezveny kivant nevet: (szokoz nelkul) ");
    scanf("%s", &eventname);
    datab.eventCounter++;
    strcpy (datab.events[datab.eventCounter - 1].name , eventname);
    datab.events[datab.eventCounter - 1].id = datab.eventCounter - 1;
    datab.events[datab.eventCounter - 1].guestCount = 0;

    printf("\n Rendezveny azonositoja: '%d' \n" , datab.events[datab.eventCounter - 1].id);
}
void deleteEvent()
{
    int id = getEventId();
    if (checkId(id)  == false)
        return;

    datab.events[id].id = -1;
    datab.events[id].guestCount = 0;
}
void newApply()
{
    char name[500];
    char email[500];

    printf("Nev: (szokoz nelkul)");
    scanf("%s", &name);
    printf("\n e-mail cim:");
    scanf("%s", &email);
    printf("\n");
    int id = getEventId();
    if (checkId(id)  == false)
        return;

    if (datab.events[id].guestCount >= 1000)
    {
        printf("A rendezvenyre mar beteltek a heyek. \n");
        return;
    }
    datab.events[id].guestCount++;
    int actGuestId = datab.events[id].guestCount;
    strcpy (datab.events[id].guests[actGuestId-1].name, name);
    strcpy (datab.events[id].guests[actGuestId-1].email, email);
    datab.events[id].guests[actGuestId-1].time = time(NULL);
    printf("A rendezvenyre '%d' . kent jelentkezett \n", actGuestId);
}
void listApplies()
{
    int id = getEventId();
    if (checkId(id)  == false)
        return;
    
    int i;
    for ( i = 0; i < datab.events[id].guestCount; i++)
        printf( " Nev: '%s'    Email: '%s'  Jelentkezes ideje: '%s' \n", 
                datab.events[id].guests[i].name, 
                datab.events[id].guests[i].email,
                ctime(&datab.events[id].guests[i].time));
}
void changeApply()
{
    char name[500];
    char email[500];
    
    printf("Nev: (szokoz nelkul)");
    scanf("%s", &name);
    printf("\n e-mail cim:");
    scanf("%s", &email);
    int id = getEventId();
    if (checkId(id)  == false)
        return;

    Event *actEv = & datab.events[id];

    int actApplyId = FindApply( name, email, actEv );

    if ( actApplyId == -1)
    {
        printf( "Nincs ilyen jelentkezes. \n" );
        return;
    }

    char newname[500];
    char newemail[500];
    
    printf("Uj nev: (szokoz nelkul)");
    scanf("%s", &newname);
    printf("\n Uj e-mail cim:");
    scanf("%s", &newemail);

    strcpy (actEv->guests[actApplyId].name, newname);
    strcpy (actEv->guests[actApplyId].email, newemail); 

    printf("\n Sikeres modositas \n");
}
void deleteApply()
{
    char name[500];
    char email[500];
    
    printf("Nev: (szokoz nelkul)");
    scanf("%s", &name);
    printf("\n e-mail cim:");
    scanf("%s", &email);
    int id = getEventId();
    if (checkId(id)  == false)
        return;

    Event *actEv = & datab.events[id];

    int actApplyId = FindApply( name, email, actEv );

    if ( actApplyId == -1)
    {
        printf( "Nincs ilyen jelentkezes. \n " );
        return;
    }

    actEv->guests[actApplyId] = actEv->guests[ actEv->guestCount - 1];
    actEv->guestCount--;
    printf(" Sikeres torles. \n");
}

void writeMenu()
{
    printf ("\n \n Valasszon menupontot:\n \n");
    printf("Uj jelentkezes [1] \n");
    printf("Uj rendezveny [2] \n");
    printf("Jelentkezettek listazasa [3] \n");
    printf("Jelentkezes adatainak modosítasa [4] \n");
    printf("Jelentkezes adatainak torles [5] \n");
    printf("Rendezveny adatainak torlese [6] \n");
    printf("Kilepes [0] \n");
}

void switchMenu(int menupoint)
{
   switch(menupoint)
   {
   case(1):
        newApply();
        Save();
        break; 
   case(2):
        newEvent();
        Save();
        break; 
   case(3):
        listApplies();
        Save();
        break; 
   case(4):
        changeApply();
        Save();
        break;   
   case(5):
        deleteApply();
        Save();
        break; 
   case(6):
        deleteEvent();
        Save();
        break;
    case(0):
        break;
   default:
        printf("Nincs ilyen menupont  \n");
        break;
    }
}

int main()
{
    int menupoint;
    Load();
    do
    {
        writeMenu();
        scanf("%d",&menupoint);
        switchMenu(menupoint);
    }
    while ( menupoint != 0);
    
    Save();

    return(0);
}