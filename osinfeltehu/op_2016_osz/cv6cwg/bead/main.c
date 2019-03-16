//scp main.c cv6cwg@os.inf.elte.hu:/home/op/cv6cwg/bead
/*
Egy kiállításokat, rendezvényeket szervező társaság a "King of Stands", elektronikus látogatói nyilvántartást készített. A látogatók a nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, a rendezvény azonosító számát! Az elektronikus rendszer a jelentkezés nyugtázásaként visszaadja, hogy hányadik vendégként történt meg a regisztráció! Az alkalmazás adjon lehetőséget vendég jelentkezéshez, a jelentkezettek listázásához, egy vendég adatainak módosításához, törléséhez. Legyen lehetőségünk egy új rendezvény indításához, egy rendezvény összes adatának törléséhez! A jelentkezőknél a korábbi adatok mellett rögzítsük automatikusan a jelentkezési időt(rendszeridő) is(ezt nem kell bekérni)!
*/

#include <stdio.h>          //standard output
#include <unistd.h>         // write system call
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

typedef struct Event
{
	char _name[32];		// name
	char _email[32];		// email
	int _id;				// identify
} Event;

int counter = 0; 	// guest counter

void apply();			            // apply for Event
void display_guests();              // display guests
void modify_guest();                // modify guest's data
void delete_guest();                 // delete guest's data
void create_new_event();         // new Event
void delete_event();             // delete Event
void menu();                        // starting menu

//helper functions
Event *create_event(char name[32], char email[32], int id);

// MAIN function
int main(int argc, char const *argv[])
{
    while( 1 )
        menu();

    return 0;
}

void menu()
{
    printf("Welcome to King of Stands!\n");
    printf("Please choose from option 1-6!\n");
    printf("For applying please choose 1\n");
    printf("For display guests choose 2\n");
    printf("For modify guest choose 3\n");
    printf("For delete guest choose 4\n");
    printf("For create new event choose 5\n");
    printf("For delete event choose 6\n");
    printf("For exit press CTRL+C\n");

    int option;
    scanf("%d",&option);   
    
    switch(option)
    {
        case 1: 
            apply();
            break;
        case 2: 
            display_guests();
            break;
        case 3: 
            modify_guest();
            break;
        case 4: 
            delete_guest();
            break;
        case 5: 
            create_new_event();
            break;
        case 6: 
            delete_event();
            break;
        default: 
            printf("Something went wrong! Please re-enter a number 1-6\n");
            menu();
    }
    
}


void apply()
{   
    //name,email.id
    char name[32];
    char email[32];
    int id;

    printf("Your name:\n");
    scanf("%s",name);
    printf("Your email:\n");
    scanf("%s",email);
    printf("Your event id:\n");
    scanf("%d",&id);

    struct Event *event = create_event(name,email,id);
    
    // so we can differ the guests
    char filename[6];
    sprintf(filename, "event%d", id);
    
    // open our file to write into
    // write and append and set rights for owner read & write as well
    int filedesc = open(filename, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);         

    if (filedesc < 0) {
        printf("Error! filedesc < 0\n");
    }
 
    if (write(filedesc, &event, sizeof( struct Event)) 
        != sizeof( struct Event)) 
    {
    //  write(2, "There was an error writing to testfile.txt\n", 43);
        printf("There was an error writng to %s file\n", filename);
        
    }
    else
    {
        printf("written datas:\n");
        printf("Name:%s\n", event->_name);
        printf("email:%s\n", event->_email);
        printf("id:%d\n", event->_id);
    }
    
    free(event);
    close(filedesc);

    ++counter;
    printf("Thank you for your apllying you are the %d guest\n\n", counter);
}                       
void display_guests()
{
    printf("Please give an event numver!\n");
    int id;
    scanf("%d", &id);

    char filename[6];
    sprintf(filename, "event%d", id);
    int filedesc = open(filename, O_RDONLY);
    if( filedesc < 0)
    {
        perror("Error at opening the file\n");
    }
    
    struct Event *event_out = malloc(sizeof(struct Event));
    

    while (read(filedesc,&event_out,sizeof(struct Event)))
    { 
        printf("Name: %s\temail: %s\t id: %d", &event_out->_name, &event_out->_email, &event_out->_id); //we prints out the content of the file on the screen
    } 



    int bytes = read(filedesc, &event_out, sizeof(Event));
    printf("bytes_read: %d\n", bytes);
    bytes = sizeof(Event);
    printf("event size: %d\n", bytes);
    free(event_out);
    

    close(filedesc);

}
void modify_guest(){}                
void delete_guest(){}               
void create_new_event(){}         
void delete_event(){}     

Event *create_event(char name[32], char email[32], int id)
{
    struct Event* event = malloc(sizeof( struct Event));
    strcpy(event->_name, name);
    strcpy(event->_email , email);
    event->_id = id;
    return event;
}
