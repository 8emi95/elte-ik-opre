#include <stdio.h>
#include <time.h>

struct event
{
   int id;
   char name[30];
   char date[10];
};
/*
1 - printEvents
2 - deleteEvent
*/
void handleEventAction(int handleAction, int deleteId)
{
	FILE *etxt;
    FILE *temp;
   	etxt = fopen("events.txt", "rb+");
    if(handleAction == 2){
        temp = fopen("temp.txt","w+");
    }
   	char c; 
   	int i = 0;
   	int delimiterCounter = 0;
   	int nameCounter = 0;
   	int dateCounter = 0;
   	int id= 0;
   	int idpart = 0;
   	char name[30];
   	name[0] = 0;
   	char date[10];
   	date[0] = 0;
   	printf("Id\tEvent\tDate\n");
   	fread(&c, sizeof(char),1,etxt);
   	while(!feof(etxt)){
	    while(c != '\n'){
	        if(c == ';'){
	            delimiterCounter++;
	        }else{
		        if(delimiterCounter == 0){
		         	idpart = c - '0';
		         	id = id * 10 + idpart; 
		        }else if(delimiterCounter == 1){
		            name[nameCounter] = c;
		            nameCounter++;
		        }else if(delimiterCounter == 2){
		            date[dateCounter] = c;
		            dateCounter++;
		        }
	        }
         	fread(&c, sizeof(char),1,etxt);
      	}
      	//A veget le kell nullazni!!
      	name[nameCounter] = 0;
   		date[dateCounter] = 0;
        if(handleAction == 1){
            printf("%i\t%s\t%s\n", id, name,date);    
        }else if(handleAction == 2){
            if(deleteId != id){
                fprintf(temp, "%i;%s;%s\n",id, name,date);
            }else{
                printf("DELETED: \n");
                printf("%i\t%s\t%s\n", id, name,date);
            }
        }
     	
		delimiterCounter = 0;
		nameCounter = 0;
		dateCounter = 0;
        name[0] = 0;
        date[0] = 0;
		id = 0;
		fread(&c, sizeof(char),1,etxt);
    }
    fclose(etxt);
    if(handleAction == 2){
        fclose(temp);
        remove("events.txt");
        rename("temp.txt", "events.txt");    
    }
}
void printEvents()
{
    handleEventAction(1, 0);
}

void appendEvents()
{
	FILE *events;
	int id; //ezt még nem tudjuk, ki kéne deríteni
	char name[30];
	int year;
	int month;
	int day;
	events=fopen("events.txt","a+");
	printf("Rendezveny azonositoja: ");
	scanf("%i", &id);
	printf("Mi a rendezveny neve? : ");
	scanf("%s", &name);
	printf("Melyik evben lesz? : ");
	scanf("%i",&year);
	printf("Melyik honapban lesz? : ");
	scanf("%i",&month);
	printf("Melyik napon lesz? : ");
	scanf("%i",&day);
	if(month < 10 && day < 10){
		printf("%i;%s;%i-0%i-0%i\n",id, name,year,month,day);	
	}else if(month < 10 && day >= 10){
		printf("%i;%s;%i-0%i-%i\n",id, name,year,month,day);	
	}else if(month >=10 && day < 10){
		printf("%i;%s;%i-%i-0%i\n",id, name,year,month,day);	
	}else{
		printf("%i;%s;%i-%i-%i\n",id, name,year,month,day);	
	}
	fprintf(events, "%i;%s;%i-%i-%i\n",id, name,year,month,day);
	fclose(events);	
}

void deleteEvent(int deleteId)
{
	handleEventAction(2, deleteId);
}
/*
1 - getNextUserId
2 - printUserList
3 - editUser
4 - deleteUser
*/
int handleUserAction(int handleAction, int searchEventId, int searchUserId)
{
    int newUserId = 1;
    FILE * vtxt;
    FILE * temp;
    vtxt=fopen("visitors.txt","rb+");
    if(handleAction == 3 || handleAction == 4){
        temp = fopen("temp.txt","w+");
    }
    char c; 
    int delimiterCounter = 0;
    int nameCounter = 0;
    int mailCounter = 0;
    int regTimeCounter = 0;
    int eventId = 0;
    int userId = 0;
    int idpartEvent = 0;
    int idpartUser = 0;
    char name[30];
    name[0] = 0;
    char newname[30];
    newname[0] = 0;
    char mail[50];
    mail[0] = 0;
    char newmail[30];
    newmail[0] = 0;
    char regTime[50]; 
    regTime[0] = 0;
    if(handleAction == 2){
        printf("EventID\tUserID\tName\tE-mail\n");
    }
    fread(&c, sizeof(char),1,vtxt);
    while(!feof(vtxt)){
        while(c != '\n'){
            if(c == ';'){
                delimiterCounter++;
            }else{
                if(delimiterCounter == 0){ 
                    idpartEvent = c - '0';
                    eventId = eventId * 10 + idpartEvent;
                }else if(delimiterCounter == 1){
                    idpartUser = c - '0';
                    userId = userId * 10 + idpartUser;
                }else if(delimiterCounter == 2){
                    name[nameCounter] = c;
                    nameCounter++;
                }else if(delimiterCounter == 3){
                    mail[mailCounter] = c;
                    mailCounter++;
                }else if(delimiterCounter == 4){
                    regTime[regTimeCounter] = c;
                    regTimeCounter++;
                }
            }
            fread(&c, sizeof(char),1,vtxt);
        }
        //A veget le kell nullazni!!
        name[nameCounter] = 0;
        mail[mailCounter] = 0;
        regTime[regTimeCounter] = 0;
        //GetNextUserId
        if(handleAction == 1){
            if(eventId == searchEventId){
                if(newUserId <= userId){
                    newUserId = userId +1;
                }
            }
        //printUserList
        }else if(handleAction == 2){
            if(eventId == searchEventId){
                printf("%i\t%i\t%s\t%s\t%s\n", eventId, userId, name, mail, regTime);
            }
        //editUser
        }else if(handleAction == 3){
            if(searchEventId == eventId && searchUserId == userId){
                printf("Jelenlegi neve: %s\nUj neve: ", name);
                scanf("%s", &newname);
                printf("Jelenlegi e-mail: %s\nUj e-mail cime: ", mail);
                scanf("%s",&newmail);
                fprintf(temp, "%i;%i;%s;%s;%s\n",eventId,userId, newname,newmail, regTime);
            }else{
                fprintf(temp, "%i;%i;%s;%s;%s\n",eventId,userId, name,mail, regTime);
            }
        //deleteUser
        }else if(handleAction == 4){
            if(searchEventId == eventId && searchUserId == userId){
                printf("Toroltuk: %i;%i;%s;%s;%s\n",eventId,userId, name,mail, regTime);
            }else{
                fprintf(temp, "%i;%i;%s;%s;%s\n",eventId,userId, name,mail, regTime);
            }
        }
        delimiterCounter = 0;
        nameCounter = 0;
        mailCounter = 0;
        regTimeCounter = 0;
        userId = 0;
        eventId = 0;
        fread(&c, sizeof(char),1,vtxt);
    }
    fclose(vtxt);
    if(handleAction == 1){
        return newUserId;    
    }else if(handleAction == 3 || handleAction == 4){
        fclose(temp);
        remove("visitors.txt");
        rename("temp.txt", "visitors.txt"); 
    }
    return 1;
}

int getNextUserId(int searchEventId)
{
    return handleUserAction(1, searchEventId, 0);
}

int userAttendToEvent(int eventId)
{
	char name[30];
	char mail[50];
	int userId;
	FILE * vtxt;
	time_t puretime;
    char regTime[50];
    strcpy(regTime, ctime(&puretime));
    userId = getNextUserId(eventId);
	vtxt=fopen("visitors.txt","a+");
	printf("Neve: ");
	scanf("%s", &name);
	printf("E-mail cime: ");
	scanf("%s",&mail);

	fprintf(vtxt, "%i;%i;%s;%s;%s\n",eventId,userId, name,mail, regTime);
	fclose(vtxt);
}
void printUserList(int searchEventId)
{
    handleUserAction(2, searchEventId, 0);
}
void editUser(int searchEventId, int searchUserId)
{
    handleUserAction(3, searchEventId, searchUserId); 
}
void deleteUser(int searchEventId, int searchUserId)
{
    handleUserAction(4, searchEventId, searchUserId);
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
				printf("Melyik rendezvenyen szeretne resztvenni?");
				scanf("%i",&eventId);
				userAttendToEvent(eventId);
        break;
			case 5:
				printEvents();
				printf("Melyik rendezveny latogatoira kivancsi?");
				scanf("%i",&eventId);
				printUserList(eventId);
        break;
			case 6: 
				printEvents();
				printf("Melyik rendezveny latogatojat szeretne szerkeszteni? ");
				scanf("%i",&eventId);
                printUserList(eventId);
				printf("Melyik felhasznalot szeretne szerkeszteni? ");
				scanf("%i", &userId);
				editUser(eventId, userId);
        break;
			case 7:
                printEvents();
                printf("Melyik rendezveny latogatojat szeretne torolni? ");
                scanf("%i",&eventId);
                printUserList(eventId);
                printf("Melyik felhasznalot szeretne torolni? ");
				scanf("%i", &userId);
				deleteUser(eventId,userId);
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