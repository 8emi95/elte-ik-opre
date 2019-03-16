#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

struct Guest
{
	char name[20];
	char email[20];
	char date[20];
};

struct Event
{
	int id;
	char name[60];
	int guestCount;
	struct Guest guestArray[20];
};

void proceedWithEnter()
{
	getchar(); //elozo enter
	char ch;
	printf("Nyomjon entert a folytatashoz! ");
	while ((ch = getchar()) != '\n'){}		
}

void deleteGuest(int* guestCount, struct Guest* guests)
{
	char toDelete[20];
	printf("Adja meg a torolni kivant vendeg nevet!\n");
	fscanf(stdin, "%s", toDelete);
	
	bool found = false;
	int i = 0;
	while(!found && i<*guestCount) 
	{
		if(strcmp(toDelete, guests->name) == 0) found = true;
		if(!found)
		{
			++guests;
			++i;
		}
	}
	
	if(found)
	{
		--(*guestCount);
		while(i<*guestCount)
		{
			*guests = *(guests+1);
			++guests;
			++i;
		}
		printf("Sikeres torles\n\n");
	}
	else
	{
		printf("Nincs ilyen vendeg\n\n");
	}
	
}

void editGuest(int* guestCount, struct Guest* guests)
{
	char toEdit[20];
	printf("Adja meg a szerkeszteni kivant vendeg nevet!\n");
	fscanf(stdin, "%s", toEdit);
	
	bool found = false;
	int i = 0;
	while(!found && i<*guestCount) 
	{
		if(strcmp(toEdit, guests->name) == 0) found = true;
		if(!found)
		{
			++guests;
			++i;
		}
	}
	
	if(found)
	{
		printf("Adja meg az uj nevet!\n");
		fscanf(stdin, "%s", guests->name);
		getchar(); //enter 
		
		printf("Adja meg az email cimet!\n");
		fscanf(stdin, "%s", guests->email);
		getchar(); //enter 
		
		time_t now = time(NULL);
		strftime(guests->date, 20, "%Y-%m-%d", gmtime(&now));
		
		printf("Sikeres modositas\n");
	}
	else
	{
		printf("Nincs ilyen vendeg\n\n");
	}
}

void readFile(int* eventCount, struct Event* events)
{
	
	char buff[255];
	FILE * f = fopen("input.txt","r");
	if (f==NULL){perror("Adathiba"); exit(1);}
	if(!f) perror("fopen");
	
	fgets(buff, 255, f);
	
	*eventCount = atoi(buff);

	for(int i=0; i<*eventCount; ++i)
	{	
		fgets(buff, 255, f);
		events->id = atoi(buff);
		fgets(events->name, 70, f);
		strtok(events->name, "\n");
		
		fgets(buff, 255, f);
		events->guestCount = atoi(buff);
		for(int i=0; i<events->guestCount; ++i)
		{
			fscanf(f, "%s", events->guestArray[i].name);
			fscanf(f, "%s", events->guestArray[i].email);
			fscanf(f, "%s", events->guestArray[i].date);
			fgets(buff, 255, f); //uj sor karakter
		}
		++events;
	}
}

void writeFile(int* eventCount, struct Event* events)
{
	FILE * f;
	char buff[255];
	f=fopen("input.txt","w");
	if (f==NULL){perror("Hiba az adatfajl mentesekor, a modositasok elvesztek"); exit(1);}
	fgets(buff, 255, f);
	
	fprintf(f, "%d", *eventCount); 
	fputc('\n', f);
	for(int i=0; i<*eventCount; ++i)
	{
		fprintf(f, "%d", events->id); 
		fputc('\n', f);
		fprintf(f, "%s", events->name); 
		fputc('\n', f);
		fprintf(f, "%d", events->guestCount); 
		fputc('\n', f);
		for(int i=0; i<events->guestCount; ++i)
		{
			fputs(events->guestArray[i].name, f);
			fputc(' ', f);
			fputs(events->guestArray[i].email, f);
			fputc(' ', f);
			fputs(events->guestArray[i].date, f);
			fputc('\n', f);
		}
		++events;
	}
	
}


void writeMainMenu()
{
	printf("\n");
	printf("Kerem adjon meg egy parancsot!\n");
	printf("1 - Esemenyek listazasa\n");
	printf("2 - Jelentkezes egy esemenyre\n");
	printf("3 - Új esemeny inditasa\n");
	printf("4 - Esemeny torlese\n");
	printf("5 - Esemeny adatainak listazasa\n");
	printf("6 - Esemeny megszervezese\n");
	printf("q - Kilepes\n");
}

void listEvents(int* eventCount, struct Event* events)
{
	
	if(*eventCount == 0) printf("Meg nincs esemeny letrehozva\n");
	
	for(int i=0; i<*eventCount; ++i)
	{
		printf("ID: %d, Nev: %s\n", events->id, events->name);
		++events;
	}
	
	proceedWithEnter();
}

void attendEvent(int* eventCount, struct Event* events)
{
	printf("Adja meg az esemeny azonositojat, amelyre jelentkezni kivan!\n");
	
	char buff[255]; int eventToAttend;
	fscanf(stdin, "%s", buff);
	eventToAttend = atoi(buff);
	
	bool found = false;
	int i = 0;
	while(!found && i<*eventCount) 
	{
		if(events->id == eventToAttend ) found = true;
		if(!found)
		{
			++events;
			++i;
		}
	}	
	
	if(found)
	{
		int guestNumber = events->guestCount;
		events->guestCount = events->guestCount + 1;
		
		printf("Adja meg a nevet!\n");
		fscanf(stdin, "%s", events->guestArray[guestNumber].name);
		getchar(); //enter 
		
		printf("Adja meg az email cimet!\n");
		fscanf(stdin, "%s", events->guestArray[guestNumber].email);
		
		
		time_t now = time(NULL);
		strftime(events->guestArray[guestNumber].date, 20, "%Y-%m-%d", gmtime(&now));
		
		printf("Sikeres regisztracio\n\n");
	}
	else
	{
		printf("Nincs ilyen esemeny\n\n");
	}
	
	proceedWithEnter();

}

void newEvent(int* eventCount, struct Event* events)
{
	events += *eventCount;
	getchar();
	printf("Leterhozando esemeny neve: ");
	fgets(events->name, 70, stdin);
	strtok(events->name, "\n");
	events->id = *eventCount;
	events->guestCount = 0;
	++(*eventCount);
	
	printf("Esemeny letrehozva\n");
}

void deleteEvent(int* eventCount, struct Event *events)
{
	
	printf("Adja meg az esemeny azonositojat, amelyet torolni kivan!\n");
	
	char buff[255]; int eventToDelte;
	fscanf(stdin, "%s", buff);
	eventToDelte = atoi(buff);
	
	bool found = false;
	int i = 0;
	while(!found && i<*eventCount) 
	{
		if(events->id == eventToDelte ) found = true;
		if(!found)
		{
			++events;
			++i;
		}
	}

	if(found)
	{
		--(*eventCount);
		while(i<*eventCount)
		{
			*events = *(events+1);
			++events;
			++i;
		}
		printf("Sikeres torles\n\n");
	}
	else
	{
		printf("Nincs ilyen esemeny\n\n");
	}
	
	proceedWithEnter();
}

void editEvent(int* eventCount, struct Event* events)
{
	printf("Adja meg az esemeny azonositojat, amelyet listazni kivan!\n");
	
	char buff[255]; int eventToEdit;
	fscanf(stdin, "%s", buff);
	eventToEdit = atoi(buff);
	
	bool found = false;
	int i = 0;
	while(!found && i<*eventCount) 
	{
		if(events->id == eventToEdit ) found = true;
		if(!found)
		{
			++events;
			++i;
		}
	}

	if(found)
	{
		printf("Az esemeny azonositoja: %d\n", events->id); ;
		printf("Az esemeny neve: %s\n", events->name); 
		printf("A vendegek listaja:\n");
		
		if(events->guestCount == 0) printf("Ezen az esemenyen nincsenek meg vendegek\n\n");
		else
		{
			for(int i=0; i<events->guestCount; ++i)
			{
				printf("A vendeg neve: %s\n", events->guestArray[i].name);
				printf("A vendeg email cime: %s\n", events->guestArray[i].email);
				printf("A vendeg jelentkezesenek datuma: %s\n\n", events->guestArray[i].date);
			}
			
			printf("Szeretne modositani a vendeglistat? (y - igen, egyeb - vissza a fomenube) ");
			getchar(); //elozo enter
			char cmd = getchar();
			if(cmd == 'y')
			{
				getchar();
				printf("\n");
				printf("1 - vendeg torlese\n");
				printf("2 - vendeg modositasa\n");
				printf("Mas - vissza a fomenubeme\n");
				cmd = getchar();
				int* gc = &(events->guestCount);
				
				switch(cmd)
				{
					case '1':
						deleteGuest(gc, events->guestArray);
						proceedWithEnter();
						break;
					case '2':
						editGuest(gc, events->guestArray);
						break;
					default:
						break;
				}
			}
		}
	}
	else
	{
		printf("Nincs ilyen esemeny\n\n");
		proceedWithEnter();
	}
	
	
}

int getEvent(int* eventCount, struct Event* events)
{
	printf("Adja meg az esemeny azonositojat, amelyet listazni kivan!\n");
	
	char buff[255]; int eventToEdit;
	fscanf(stdin, "%s", buff);
	eventToEdit = atoi(buff);
	
	bool found = false;
	int i = 0;
	while(!found && i<*eventCount) 
	{
		if(events->id == eventToEdit ) found = true;
		if(!found)
		{
			++events;
			++i;
		}
	}

	if(found) return i;
	else
	{
		printf("Nincs ilyen esemeny\n\n");
		return -1;
	}
}

int sigSent = 0;


void handler(int signumber){
	if(SIGUSR1 == signumber) printf("Partner az esmenyre ert\n");
	++sigSent;
}



int main(int argc, char** argv)
{
	//pipe
	int toChild[2]; // unnamed pipe file descriptor array
	int toParent[2];
    char msg[100];  // char array for reading from pipe
	pipe(toChild);
	pipe(toParent);
	//end pipe
	
	//signal
	
	//end signal
	
	int partner = fork();
	if(partner != 0)
	{
		int* eventCount = malloc(sizeof(*eventCount));
		struct Event events[10];
		
		readFile(eventCount, events);
		bool keepDoing = true;
		int eventIndex;
		do
		{
			writeMainMenu();
			char cmd = getchar();
			switch(cmd)
			{
				case '1':
					listEvents(eventCount, events);
					break;
				case '2':
					attendEvent(eventCount, events);
					break;
				case '3':
					newEvent(eventCount, events);
					break;
				case '4':
					deleteEvent(eventCount, events);
					break;
				case '5':
					editEvent(eventCount, events);
					break;
				case '6':
					signal(SIGUSR1,handler);
					eventIndex = getEvent(eventCount, events);
					
					if(eventIndex != -1)
					{
						close(toChild[0]);
						close(toParent[1]);
						write(toChild[1], events[eventIndex].name,strlen(events[eventIndex].name)+1);
						
						while (0 == sigSent) usleep(100);
						
						read(toParent[0], msg, sizeof(msg));
						printf("%s\n", msg);
						
						for(int i = 0; i<events[eventIndex].guestCount; ++i)
						{
							write(toChild[1], events[eventIndex].guestArray[i].name,strlen(events[eventIndex].guestArray[i].name)+1); 
							read(toParent[0], msg, sizeof(msg));
							printf("%s\n", msg);
						}
						write(toChild[1], "q", 2);
						
						sigSent = 0;
					}
					
					proceedWithEnter();
						
					break;
				case 'q':
					printf("Viszont Latasra!\n");
					keepDoing = false;
					break;
				default:
					printf("Nincs ilyen parancs!\n");
					break;
			}
		}while(keepDoing);
		
		kill(partner,SIGTERM);
		writeFile(eventCount, events);
	}else 
	{
		while(true)
		{
			close(toChild[1]);
		 close(toParent[0]);
		 read(toChild[0],msg,sizeof(msg)); // reading max 100 chars
         printf("Esemeny helyszine: %s\n",msg);
		 kill(getppid(),SIGUSR1);
		 srand(time(NULL));
		 
		 strcpy(msg, "Az esemeny sikere: ");
		 char percentage[20];
		 sprintf(percentage, "%d szazalek", rand()%101);
		 strcat(msg, percentage);
		 write(toParent[1], msg, strlen(msg)+1);
		 
		 bool keepDoing =true;
		 do
		 {
			 int len = read(toChild[0],msg,sizeof(msg));
			 msg[len] = '\0';
			 if(keepDoing = (strcmp(msg,"q") != 0))
			 {
				 int r=rand()%100;
				 strcat(msg, r>=90 ? " nem jelent meg" : " ott volt");
				 write(toParent[1], msg, strlen(msg) + 1);
			 }
		 }while(keepDoing);
		 
		}	 
	}
	
}