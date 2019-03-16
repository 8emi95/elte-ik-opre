/*
Egy kiállításokat, rendezvényeket szervezõ társaság a "King of Stands",
elektronikus látogatói nyilvántartást készített. A látogatók a nyilvántartásba
jelentkezéskor megadják nevüket, mail címüket, a rendezvény azonosító számát!
Az elektronikus rendszer a jelentkezés nyugtázásaként visszaadja, hogy hányadik vendégként történt meg
a regisztráció!
	Az alkalmazás adjon lehetõséget
+ 1.  - vendég jelentkezéshez,
+ 2.  - a jelentkezettek listázásához,
+ 3.  - egy vendég adatainak módosításához,
+ 4.  - törléséhez.
+ 5.  - Legyen lehetõségünk egy új rendezvény indításához,
+ 6.  - egy rendezvény összes adatának törléséhez!
+ 7. - Kilepes
 A jelentkezõknél a korábbi adatok mellett rögzítsük automatikusan a jelentkezési idõt(rendszeridõ)
 is(ezt nem kell bekérni)!
 
 2. Beadandohoz:
 + 8. - Rendezveny lebonyolitasa
*/

#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>


struct guest {
	char name[100];
    char email[100];
    char event_id[100];
    char timestamp[100];
};


// 0-at ad vissza, ha üres a file,
// 1-et, ha nem
int is_empty(char file_name[])
{
    struct stat st;
    stat(file_name, &st);
    int size = st.st_size;
    if (size < 3)
        return 0;
    else
        return 1;
}

void listEvents()
{
    FILE *events_file = fopen("events.txt", "r");
    if (events_file == NULL )
    {
        printf("Hiba az adatfajl megnyitasa kozben \n");
        return;
    }


    char buf[1000];
    char name[100];
    int event_id;

    while (fgets(buf,1000, events_file)!=NULL)
    {
        sscanf(buf, "%s %d",name,&event_id);
        printf("nev: %s ; azonositoszam:  %d \n",name,event_id);
    }
    fclose(events_file);
}


// -1-et ad vissza, ha nincs ilyen id,
// ha van, akkor annak a sornak a számát, amelyikben található.
int isLegalEventId(char id[])
{
    int b = -1;
    FILE *events_file = fopen("events.txt", "r");
    if (events_file == NULL )
    {
        printf("Hiba az adatfajl megnyitása közben \n");
        return -2;
    }

    char buf[1000];
    char name[100];
    char event_id[100];

    int i = 0;
	//printf("\n IS LEGAL?? \n");
    while (fgets(buf,1000, events_file)!=NULL)
    {
        sscanf(buf, "%s %s",name,event_id);
		//printf("\n NEZI: ###%s###  \n", event_id);
		//printf("\n KENE: ###%s###  \n", id);
        if(strcmp(event_id,id) == 0)
        {
            //printf("\n MEGVAN A B: - %i  \n", i);
            fclose(events_file);
            return b = i;
        }
        i++;
    }
    fclose(events_file);
    return b;
}



// -1-et ad vissza, ha nincs ilyen email,
// ha van, akkor annak a sornak a szamat, amelyikben van
int isLegalEmail(char input_email[])
{
    int b = -1;
    FILE *guest_file = fopen("guests.txt", "r");
    if (guest_file == NULL )
    {
        printf("Hiba az adatfájl megnyitása közben \n");
        return 1;
    }

    char buf[1000];
    char name[100];
    char email[100];
    char event_id[100];
    char timestamp[100];

    int i = 0;
    while (fgets(buf,1000, guest_file)!=NULL)
    {
        sscanf(buf, "%s %s %s %s",name,email,event_id,timestamp);
        if(strcmp(email,input_email) == 0)
        {
             fclose(guest_file);
            return b = i;
        }
        i++;
    }
    fclose(guest_file);
    return b;
}


void menu()
{
    char menu_choice_char[50];
	while(1)
	{
	    printf("\n --------------------- \n");
		printf("1 - Vendeg jelentkezese \n");
		printf("2 - Jelentkezett vendegek listazasa \n");
		printf("3 - Jelentkezett vendegek adatainak modositasa \n");
		printf("4 - Jelentkezett vendeg adatainak torlese \n");
		printf("5 - Uj rendezveny inditasa \n");
		printf("6 - Rendezveny osszes adatanak torlese \n");
		printf("7 - Kilepes \n");
		printf("8 - Rendezveny lebonyolitasa - MASODIK BEADANDO \n");
		printf("- -------------------- \n");


		scanf("%s",menu_choice_char);

		int menu_choice = atoi(menu_choice_char);
		if (menu_choice == 0){
            printf("Ervenytelen menupont! \n");
            menu();
		}

		switch (menu_choice){
			case 1: newGuest();
                break;
            case 2: listGuests(1);
                break;
            case 3: modifyGuestData();
                break;
            case 4: deleteGuestData();
                break;
			case 5: newEvent();
                break;
            case 6: deleteEventData();
				break;
			case 7: exit(0);
				break;
			case 8: event();
				break;
            default: printf("Ervenytelen menupont! \n");

		}
	}
}

//1

void newGuest()
{

    FILE *guests_file = fopen("guests.txt", "a");
    if (guests_file == NULL)
    {
        printf("Hiba az adatfájl megnyitása közben \n");
        return;
    }

    FILE *events_file = fopen("events.txt", "r");
    if (events_file == NULL)
    {
        printf("\n Nincsen rendezveny, amire regisztralhatna! \n");
        fclose(events_file);
        return;
    }
     if (is_empty("events.txt") == 0 )
    {
        printf("\n Nincsen rendezveny, amire regisztralhatna! \n");
        fclose(events_file);
        return;
    }


    char name[100];
    char email[100];
    char event_id[100];
    char timestamp[100];

    printf("A vendeg neve: \n");
    scanf("%s", name);
    printf("A vendeg email-cime: \n");
    scanf("%s", email);

    printf("\n A rendezveny azonositoszama: ");
    scanf("%s", event_id);


    while (isLegalEventId(event_id) == -1)
    {
        printf("\n A rendezveny azonositoszama nincs a listaban. A kovetkezo rendezvenyek kozul valaszthat: \n ");
        listEvents();
        printf("\n A rendezveny azonositoszama: ");
        scanf("%s", event_id);
    }

    time_t timer;
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(timestamp, 50, "%Y-%m-%d--%H:%M:%S", tm_info);
    //printf("%s \n", timestamp);

    fprintf(guests_file, "%s %s %s %s \n", name, email, event_id, timestamp);
    fclose(guests_file);


    printf("\n A jelentkezes sikeresen elmentve! \n ");
    menu();
}


//2
void listGuests(int kell_e_menu)
{
    FILE *guests_file = fopen("guests.txt", "r");
    if (guests_file == NULL )
    {
        printf("Nincsenek regisztralt vendegek! \n");
        fclose(guests_file);
        return;
    }
     if (is_empty("guests.txt") == 0 )
    {
        printf("\n Nincsenek regisztralt vendegek! \n");
        fclose(guests_file);
        return;
    }

    char buf[1000];
    char name[100];
    char email[100];
    int event_id;
    char timestamp[100];

    while (fgets(buf,1000, guests_file)!=NULL)
    {
        sscanf(buf, "%s %s %d %s",name,email,&event_id,timestamp);
        printf("nev: %s -- email: %s -- azonositoszam:  %d -- jelentkezes idopontja: %s \n",name,email,event_id,timestamp);
    }
    fclose(guests_file);

    if (kell_e_menu == 1)
        menu();
}


//3
void modifyGuestData()
{
    FILE *guests_file = fopen("guests.txt", "r");
    if (guests_file == NULL)
    {
        printf("\n Nincsenek regisztralt vendegek! \n");
        fclose(guests_file);
        return;
    }
     if (is_empty("guests.txt") == 0 )
    {
        printf("\n Nincsenek regisztralt vendegek! \n");
        fclose(guests_file);
        return;
    }

    char new_email[100];
    char new_name[100];
    char new_event_id[100];

    printf("\n Adja meg annak a vendegnek az email-cimet, akinek az adatait modositani szeretne: ");
    scanf("%s", new_email);

    while (isLegalEmail(new_email) == -1)
    {
        printf("\n A megadott email-cim nincs a listaban. A kovetkezo email-cimek kozul valaszthat: \n ");
        listGuests(0);
        printf("\n A valasztott email-cim: ");
        scanf("%s", new_email);
    }
    int row_number = isLegalEmail(new_email);



    printf("A vendeghez tartozo uj nev: \n");
    scanf("%s", new_name);
    printf("A vendeghez tartozo uj email-cim: \n");
    scanf("%s", new_email);

    printf("\n A vendeghez tartozo rendezveny uj azonositoszama: ");
    scanf("%s", new_event_id);

    while (isLegalEventId(new_event_id) == -1)
    {
        printf("\n A rendezveny azonositoszama nincs a listaban. A kovetkezo rendezvenyek kozul valaszthat: \n ");
        listEvents();
        printf("\n A rendezveny azonositoszama: ");
        scanf("%s", new_event_id);
    }

    FILE *temp_file = fopen("temp.txt", "w+");
    if (temp_file == NULL)
    {
        printf("Hiba az adatfájl megnyitása közben \n");
        return;
    }


    char buf[1000];
    char name[100];
    char email[100];
    int event_id;
    char timestamp[100];

     int i = 0;

    while (fgets(buf,1000, guests_file)!=NULL)
    {
        sscanf(buf, "%s %s %d %s",name,email,&event_id,timestamp);
        //printf("nev: %s -- email: %s -- azonositoszam:  %d \n",name,email,event_id);
        if (i != row_number)
        {
            fprintf(temp_file, "%s %s %d %s \n", name, email, event_id, timestamp);
        }
        else
        {
            fprintf(temp_file, "%s %s %s %s \n", new_name, new_email, new_event_id, timestamp);
        }

        i++;
    }
    fclose(guests_file);
    fclose(temp_file);

    remove("guests.txt");
    rename("temp.txt", "guests.txt");

    printf("\n A vendeg adatai sikeresen modositva! \n");
    menu();
}



//4
void deleteGuestData()
{
    FILE *guests_file = fopen("guests.txt", "r");
    if (guests_file == NULL)
    {
        printf("\n Nincsenek regisztralt vendegek! \n");
        fclose(guests_file);
        return;
    }
     if (is_empty("guests.txt") == 0 )
    {
        printf("\n Nincsenek regisztralt vendegek! \n");
        fclose(guests_file);
        return;
    }

    char new_email[100];

    printf("\n Adja meg annak a vendegnek az email-cimet, akinek az adatait torolni szeretne: ");
    scanf("%s", new_email);

    while (isLegalEmail(new_email) == -1)
    {
        printf("\n A megadott email-cim nincs a listaban. A kovetkezo email-cimek kozul valaszthat: \n ");
        listGuests(0);
        printf("\n A valasztott email-cim: ");
        scanf("%s", new_email);
    }
    int row_number = isLegalEmail(new_email);



    FILE *temp_file = fopen("temp.txt", "w+");
    if (temp_file == NULL)
    {
        printf("Hiba az adatfájl megnyitása közben \n");
        return;
    }

    char buf[1000];
    char name[100];
    char email[100];
    int event_id;
    char timestamp[100];

    int i = 0;
    //printf("row_num: %d \n", row_number);
    while (fgets(buf,1000, guests_file)!=NULL)
    {
        sscanf(buf, "%s %s %d %s ",name,email,&event_id,timestamp);

        if (i != row_number)
        {
            fprintf(temp_file, "%s %s %d %s \n", name, email, event_id, timestamp);
        }


        i++;
    }
    fclose(guests_file);
    fclose(temp_file);

    remove("guests.txt");
    rename("temp.txt", "guests.txt");

    printf("\n A vendeg adatai sikeresen torolve! \n");
    menu();
}


//5
void newEvent()
{

    FILE *events_file = fopen("events.txt", "a");
    if (events_file == NULL)
    {
        printf("Hiba az adatfájl megnyitása közben \n");
        return;
    }

    char name[100];
    char id[100];

    printf("Az uj rendezveny neve: ");
    scanf("%s", name);

    int isDigit = 1;
    while(isDigit != 0)
    {
        isDigit = 0;
        printf("\n Az uj rendezveny azonositoszama: ");
        scanf("%s", id);

        int j = 0;
        while(j < strlen(id) && isDigit == 0){
          if(isdigit(id[j]) == 0)
            {
                isDigit = 1;
                printf("\n A rendezveny azonositoja csak szamokat tartalmazhat! ");
            }
          j++;
        }
    }

    fprintf(events_file, "%s %s \n", name, id);
    fclose(events_file);
    printf("\n Az uj rendezveny sikeresen elmentve! \n ");

    menu();
}



//6
void deleteEventData()
{
    FILE *events_file = fopen("events.txt", "r");
    if (events_file == NULL)
    {
        printf("Nincs rendezveny, amit torolhetne! \n");
        return;
    }
     if (is_empty("events.txt") == 0 )
    {
        printf("\n Nincs torolheto esemeny! \n");
        fclose("events.txt");
        return;
    }

    char event_id_to_delete[100];

    printf("\n Adja meg annak az esemenynek az azonositoszamat, aminek az adatait torolni szeretne: ");
    scanf("%s", event_id_to_delete);

    while (isLegalEventId(event_id_to_delete) == -1)
    {
        printf("\n A megadott azonositoszam nincs a listaban. A kovetkezo esemenyek kozul valaszthat: \n ");
        listEvents();
        printf("\n A valasztott azonositoszam: ");
        scanf("%s", event_id_to_delete);
    }
    int row_number = isLegalEventId(event_id_to_delete);


    FILE *temp_file = fopen("temp.txt", "w+");
    if (temp_file == NULL)
    {
        printf("Hiba az adatfájl megnyitása közben \n");
        return;
    }
    char buf[1000];
    char name[100];
    char event_id[100];

    int i = 0;
    //printf("row_num: %d \n", row_number);
    while (fgets(buf,1000, events_file)!=NULL)
    {
        sscanf(buf, "%s %s",name,event_id);
        //printf("nev: %s -- email: %s -- azonositoszam:  %d \n",name,email,event_id);
        if (i != row_number)
        {
            fprintf(temp_file, "%s %s \n", name, event_id);
        }
        i++;
    }
    fclose(events_file);
    fclose(temp_file);

    remove("events.txt");
    rename("temp.txt", "events.txt");

    FILE *guests_file = fopen("guests.txt", "r");
    temp_file = fopen("temp.txt", "w+");

    if (guests_file == NULL || temp_file == NULL)
    {
        printf("Hiba az adatfájl megnyitása közben \n");
        return;
    }

    char email[100];
    char timestamp[100];
//printf("\n DEBUG \n");

    while (fgets(buf,1000, guests_file)!=NULL)
    {
        sscanf(buf, "%s %s %s %s",name,email,event_id,timestamp);
        if(strcmp(event_id,event_id_to_delete) != 0)
        {
            fprintf(temp_file, "%s %s %s %s \n", name, email, event_id, timestamp);
        }
    }
    fclose(guests_file);
    fclose(temp_file);

    remove("guests.txt");
    rename("temp.txt", "guests.txt");


    printf("\n A rendezveny adatai sikeresen torolve! \n");
    menu();
}



void deleteEventDataByParent(char event_id_to_delete[])
{
    FILE *events_file = fopen("events.txt", "r");
    if (events_file == NULL)
    {
        printf("Nincs rendezveny, amit torolhetne! \n");
        return;
    }
     if (is_empty("events.txt") == 0 )
    {
        printf("\n Nincs torolheto esemeny! \n");
        fclose("events.txt");
        return;
    }

    
    int row_number = isLegalEventId(event_id_to_delete);


    FILE *temp_file = fopen("temp.txt", "w+");
    if (temp_file == NULL)
    {
        printf("Hiba az adatfájl megnyitása közben \n");
        return;
    }
    char buf[1000];
    char name[100];
    char event_id[100];

    int i = 0;
    //printf("row_num: %d \n", row_number);
    while (fgets(buf,1000, events_file)!=NULL)
    {
        sscanf(buf, "%s %s",name,event_id);
        //printf("nev: %s -- email: %s -- azonositoszam:  %d \n",name,email,event_id);
        if (i != row_number)
        {
            fprintf(temp_file, "%s %s \n", name, event_id);
        }
        i++;
    }
    fclose(events_file);
    fclose(temp_file);

    remove("events.txt");
    rename("temp.txt", "events.txt");

    FILE *guests_file = fopen("guests.txt", "r");
    temp_file = fopen("temp.txt", "w+");

    if (guests_file == NULL || temp_file == NULL)
    {
        printf("Hiba az adatfájl megnyitása közben \n");
        return;
    }

    char email[100];
    char timestamp[100];
//printf("\n DEBUG \n");

    while (fgets(buf,1000, guests_file)!=NULL)
    {
        sscanf(buf, "%s %s %s %s",name,email,event_id,timestamp);
        if(strcmp(event_id,event_id_to_delete) != 0)
        {
            fprintf(temp_file, "%s %s %s %s \n", name, email, event_id, timestamp);
        }
    }
    fclose(guests_file);
    fclose(temp_file);

    remove("guests.txt");
    rename("temp.txt", "guests.txt");

	printf("[szulo] Sikerese lezartuk a rendezvenyt!!! \n");
    printf("\n A rendezveny adatai sikeresen torolve! \n");
    menu();
}

// MASODIK BEADANDO

/*
void handler(int signumber){
  printf("SZULO - Signal with number %i has arrived\n",signumber);
 }
*/

// 8
void event()
{
	FILE *events_file = fopen("events.txt", "r");
    if (events_file == NULL)
    {
        printf("Nincs rendezveny, amit lebonyolithato! \n");
        menu();
		return;
    }
     if (is_empty("events.txt") == 0 )
    {
        printf("\n Nincs rendezveny, amit lebonyolithato! \n");
        fclose("events.txt");
        menu();
		return;
    }
	
	printf("\n A kovetkezo rendezvenyek aktivak: \n ");
    listEvents();
		
	char buf[1000];
    char name[100];
    char event_id[100];
    fgets(buf,1000, events_file);
    sscanf(buf, "%s %s",name,event_id);
	fclose(events_file);
	
	printf("\n Ezek kozul az elsot fogja lebonyolitani a child-process. Azonositoja:  %s \n \n", event_id);
	
	// Kezdõdik a többszálazás
	
	int     fd[2], gd[2], nbytes;
        pid_t   childpid;
        char    childString1[] = "Johet a vendeglista! \n";
		//char    childString2[] = "<< gy -> sz 2 >> \n";
		//char    parentString1[] = "<< sz -> gy 1 >> \n";
		//char    parentString2[] = "<< sz -> gy 2 >> \n";
        char    readbuffer[80];

        pipe(fd);
		pipe(gd);

        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }

		
		
        if(childpid == 0)
        {
			
			// A GYEREK íR
				//sleep(1);
                nbytes = read(gd[0], readbuffer, sizeof(readbuffer));		
                printf("[gyerek] megkaptam az azonositot:  %s \n", readbuffer);
				sleep(3);
				
				
                close(fd[0]);
			
                write(fd[1], childString1, (strlen(childString1)+1));
				
				printf("[gyerek] Johet a vendeglista!  \n");				
				
				nbytes = read(gd[0], readbuffer, sizeof(readbuffer));
				sleep(1);
				char guests[1000];
				char missed[100];
				char separator[] = ", ";
				strcpy(guests, readbuffer);
                printf("[gyerek] megkaptam a vendegek email-cimeit, intezem a lebonyolitast: \n");
				
				int random;
				const char s[2] = "###";
				   char *token;
				   
				   /* get the first token */
				   token = strtok(guests, s);
				   				   
				   /* walk through other tokens */
				   srand(time(NULL));
				   int rank = rand() % 10;
				   sprintf(missed, " %d", rank);
				   				   
				   strcat(missed, separator);	
				   char lucky[50];
				   
				   while( token != NULL ) 
				   {
					  strcpy(lucky, token);
					  printf( "- %s \n", token );					
					  token = strtok(NULL, s);
					  
					  int random = rand() % 10;
					  if (random == 7){
						strcat(missed, lucky);
						strcat(missed, separator);	
						continue;
					  }
				   }
				   
				   memmove(missed, missed+1, strlen(missed));
				   
				printf("[gyerek] Kuldom, hogy kik nem jottek el! \n");
				
				
				// A vendegeket az emal-cimuk alapjan azonositom, mivel az egyedi, a nevekkel ellentetben				
				
				write(fd[1], missed, (strlen(missed)+1));
				
				//kill(childpid, SIGUSR1);
                exit(0);
        }
        else // SZULO
        {
                				
				close(gd[0]);
				write(gd[1], event_id, (strlen(event_id)+1));
				printf("[szulo] kuldom a rendezveny azonositojat \n");
				sleep(3);
				
				sleep(1);
				nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
                printf("[szulo] Oke, mindjart kuldom \n");
				sleep(3);
												
					printf("\n A [szulo] most keresi a vendegeket \n");			
					FILE *guests_file = fopen("guests.txt", "r");
					if (guests_file == NULL )
					{
						printf("Nincsenek regisztralt vendegek! \n");
						fclose(guests_file);
						menu();
						//return;
					}
					 if (is_empty("guests.txt") == 0 )
					{
						printf("\n Nincsenek regisztralt vendegek! \n");
						fclose(guests_file);
						menu();
						//return;
					}
					
					char buf[1000];
					char name[100];
					char email[100];
					char event_id_to_read[100];
					char timestamp[100];					
					char guest_emails[1000];
					char separator[3];
					strcpy(separator, "###");
					int i = 0;
					
					while (fgets(buf,1000, guests_file)!=NULL)
					{
						//printf("\n ez egy vendeg \n");						
						sscanf(buf, "%s %s %s %s",name,email,event_id_to_read,timestamp);
						if (strcmp(event_id, event_id_to_read) == 0)
						{
							//printf("\n ez egy TALALAT \n");		
							//printf("nev: %s -- email: %s -- azonositoszam:  %s -- jelentkezes idopontja: %s \n",name,email,event_id_to_read,timestamp);
							strcat(guest_emails, email);
							strcat(guest_emails, separator);
						}				
					}
					
					//printf("[szulo] A vendeg: %s \n", guest_emails);
				//
				fclose(guests_file);
				memmove(guest_emails, guest_emails+4, strlen(guest_emails));
				write(gd[1], guest_emails, (strlen(guest_emails)+1));
				printf("\n [szulo] kuldom a vendegek email cimeit! \n");
				
				sleep(3);
				nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
                printf("[szulo] A rendezveny ertekelese es az el nem jott vendegek listaja: %s \n", readbuffer);
				sleep(3);
				
				
				
				
				// A RENDEZVENY ADATAINAK TORLESE:
				deleteEventDataByParent(event_id);
				
				
				
				
				
				
				
				
        } // szülõ vége
		
		
		sleep(3);
		//return;
		menu();
	
}




int main()
{
	
	menu();
	return 0;
}
