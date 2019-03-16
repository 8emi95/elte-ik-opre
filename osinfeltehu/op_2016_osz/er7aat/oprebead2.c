#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

typedef int bool;
#define true 1
#define false 0

typedef struct EventNode
{
	char event_name [50];
	char event_place [50];
	int event_id;
	int guest_number;
	
	struct EventNode * next;
} e_node;

typedef struct GuestNode
{
	char guest_name [50];
	char guest_email [50];
	int event_id;
	struct tm * log_time;
	
	struct GuestNode * next;
} g_node;

void NewEvent(e_node * head); //done
void RSVP(g_node * g_head, e_node * e_head); //done, time still needed
void EventList(e_node * head); //done
void ModifyGuestData(g_node * head, e_node * e_head);
void DeleteGuestData(g_node * g_head, e_node * e_head);
void CancelEvent(e_node * head); //done, need to refine, make foolproof
void ListAllGuests(g_node * head); //done
void EventGuestList(e_node * e_head, g_node * g_head); //done
void WriteToFile(e_node * e_head, g_node * g_head);
int RandomGenerator();
void SignalHandler(int signum);
void ManageExhibition(e_node * e_head, g_node * g_head);

int main(int argc, char ** argv)
{
	e_node * e_head = NULL;
	e_head = malloc(sizeof(e_node));
	if(e_head == NULL)
	{
		return 1;
	}
	
	strcpy(e_head -> event_name, "ures");
	strcpy(e_head->event_place, "ures");
	e_head -> event_id = -1;
	e_head -> guest_number = -1;
	
	g_node * g_head = NULL;
	g_head = malloc(sizeof(g_node));
	if(g_head == NULL)
	{
		return 1;
	}
	
	strcpy(g_head->guest_name, "ures");
	strcpy(g_head->guest_email, "ures");
	g_head->event_id = -1;
	
	srand((unsigned int)time(NULL));
	
	int cmd;
	do{
		printf("%s\n", "MENU");
		printf("%s\n", "Uj esemeny letrehozasa: 1");
		printf("%s\n", "Esemeny torlese: 2");
		printf("%s\n", "Vendeg jelentkezese: 3");
		printf("%s\n", "Vendeg adatainak modositasa: 4");
		printf("%s\n", "Jelentkezes torlese: 5");
		printf("%s\n", "Minden vendeg listazasa: 6");
		printf("%s\n", "Egy kiallitasra jelentkezettek listazasa: 7");
		printf("%s\n", "Kiallitasok listazasa: 8");
		printf("%s\n", "Adatok fajlba irasa: 9");
		printf("%s\n", "Rendezvények feldolgozása: 10");
		printf("%s\n", "Kilepes: 0");
		
		scanf("%d", &cmd);
		switch(cmd)
		{
			case 1:
			NewEvent(e_head);
			break;
			
			case 2:
			CancelEvent(e_head);
			break;
			
			case 3:
			RSVP(g_head, e_head);
			break;
			
			case 4:
			ModifyGuestData(g_head, e_head);
			break;
			
			case 5:
			DeleteGuestData(g_head, e_head);
			break;
			
			case 6:
			ListAllGuests(g_head);
			break;
			
			case 7:
			EventGuestList(e_head, g_head);
			break;
			
			case 8:
			EventList(e_head);
			break;
			
			case 9:
			WriteToFile(e_head, g_head);
			break;
			
			case 0:
			exit(0);
			break;
			
			case 10:
			ManageExhibition(e_head, g_head);
			break;
		}
	}
	while(cmd != 10);
}

void NewEvent(e_node * head) //ok
{
	e_node * current = head;
    while (current->next != NULL) {
        current = current->next;
    }
	
	char name[50];
	printf("Esemeny neve: ");
	scanf("%49s", name);
	
	char place [50];
	printf("Esemeny helyszine: ");
	scanf("%49s", place);
	
	int id;
	printf("Esemeny azonositoja: ");
	scanf("%d", &id);
	
	current->next = malloc(sizeof(e_node));
	strcpy(current->next->event_name, name);
	strcpy(current->next->event_place, place);
	current->next->event_id = id;
	current->next->guest_number = 0;
}

void RSVP(g_node * g_head, e_node * e_head) //ok
{
	char name[50];
	printf("Vendég neve: ");
	scanf("%49s", name);
	
	char email[50];
	printf("Vendég email címe: ");
	scanf("%49s", email);
	
	int id;
	printf("Esemeny azonositoja: ");
	scanf("%d", &id);
	
	e_node * current_event = e_head;
	while(current_event->event_id != id)
	{
		if(current_event->next == NULL && current_event->event_id != id)
		{
			printf("%s\n", "Nincs ilyen azonositoju rendezveny!");
			return;
		}
		current_event = current_event->next;
	}
	
	g_node * current_guest = g_head;
    while (current_guest->next != NULL) {
        current_guest = current_guest->next;
    }
	
	current_guest->next = malloc(sizeof(g_node));
	strcpy(current_guest->next->guest_name, name);
	stpcpy(current_guest->next->guest_email, email);
	current_guest->next->event_id = id;
	
	time_t rawtime;
	time (&rawtime);
	current_guest->next->log_time = localtime(&rawtime);
	
	current_event->guest_number++;
	printf("Sikeres jelentkezes! A vendeg %d.-ként regisztrált a kiallitasra!\n", current_event->guest_number);
}

void CancelEvent(e_node * head)
{
	int id;
	printf("Torlendo esemeny azonositoja: ");
	scanf("%d", &id);
	
    e_node * current = head;
    e_node * temp_node;

    while(current->next != NULL && current->next->event_id != id)
	{
		current = current->next;
	}
	
	if(current->next == NULL && current->event_id != id)
	{
		printf("%s\n", "Nincs ilyen azonositoju rendezveny!");
		return;
	}
	
	temp_node = current->next;
    current->next = temp_node->next;
	free(temp_node);
}

void EventList(e_node * head) //ok
{
	e_node * current = head->next;
	while(current != NULL)
	{
		printf("%s\n", current->event_name);
		printf("%d\n", current->event_id);
		printf("%d\n", current->guest_number);
		current = current->next;
	}
}

void ListAllGuests(g_node * head) //ok
{
	g_node * current = head->next;
	while(current != NULL)
	{
		printf("%s\n", current->guest_name);
		printf("%s\n", current->guest_email);
		printf("%d\n", current->event_id);
		printf("%s\n", asctime(current->log_time));
		current = current->next;
	}
}

void EventGuestList(e_node * e_head, g_node * g_head) //ok
{
	e_node * current_event = e_head;
	g_node * current_guest = g_head->next;

	int id;
	printf("Listazando esemeny azonositoja: ");
	scanf("%d", &id);
	
	while(current_event->event_id != id)
	{
		if(current_event->next == NULL && current_event->event_id != id)
		{
			printf("%s\n", "Nincs ilyen azonositoju rendezveny!");
			return;
		}
		current_event = current_event->next;
	}
	
	while(current_guest != NULL)
	{
		if(current_guest->event_id == current_event->event_id)
		{
			printf("%s\n", current_guest->guest_name);
			printf("%s\n", current_guest->guest_email);
			printf("%s\n", asctime(current_guest->log_time));
		}
		current_guest = current_guest->next;
	}
}

void DeleteGuestData(g_node * g_head, e_node * e_head) //ok
{
	char name [50];
	printf("Torlendo vendeg neve: ");
	scanf("%49s", name);

    g_node * current_guest = g_head;
    g_node * temp_node = NULL;

    while(current_guest->next != NULL && strcmp(current_guest->next->guest_name, name) != 0)
	{
		current_guest = current_guest->next;
	}
	
	if(current_guest->next == NULL && ((strcmp(current_guest->guest_name, name)) != 0))
	{
		printf("%s\n", "Nincs ilyen nevu vendeg!");
		return;
	}
	
	e_node * current_event = e_head;
	
	temp_node = current_guest->next;
	while(current_event->event_id != temp_node->event_id)
	{
		current_event = current_event->next;
	}
	current_event->guest_number--;
	
    current_guest->next = temp_node->next;
	free(temp_node);
}

void ModifyGuestData(g_node * g_head, e_node * e_head) //ok
{
	char name [50];
	printf("Modositando vendeg neve: ");
	scanf("%49s", name);
	
	g_node * current_guest = g_head;
    g_node * temp_node = NULL;

    while(strcmp(current_guest->guest_name, name) != 0)
	{
		if(((strcmp(current_guest->guest_name, name)) != 0) && current_guest->next == NULL)
		{
			printf("%s\n", "Nincs ilyen nevu vendeg!");
			return;
		}
		current_guest = current_guest->next;
	}
	
	printf("Uj nev: ");
	scanf("%49s", name);
	strcpy(current_guest->guest_name, name);
	
	char email [50];
	printf("Uj email cim: ");
	scanf("%49s", email);
	strcpy(current_guest->guest_email, email);
	
	int id;
	printf("Uj esemeny azonositoja: ");
	scanf("%d", &id);

	e_node * current_event = e_head;
	while(current_event->event_id != id)
	{
		if(current_event->next == NULL && current_event->event_id != id)
		{
			printf("%s\n", "Nincs ilyen azonositoju rendezveny!");
			return;
		}
		current_event = current_event->next;
	}
	
	current_guest->event_id = id;
}

void WriteToFile(e_node * e_head, g_node * g_head) //date?
{
	FILE *f = NULL;
	f = fopen("file.txt", "w+");
	
	e_node * current_event = e_head->next;
	if(current_event != NULL)
	{
		while(current_event->next != NULL)
		{
			fprintf(f, "%s %d %d", current_event->event_name, current_event->event_id, current_event->guest_number);
			fprintf(f, "\n");
			current_event = current_event->next;
		}
		fprintf(f, "%s %d %d", current_event->event_name, current_event->event_id, current_event->guest_number);
		fprintf(f, "\n");
		fprintf(f, "\n");
	}
	
	g_node * current_guest = g_head->next;
	if(current_guest != NULL)
	{
		while(current_event->next != NULL)
		{
			fprintf(f, "%s %s %d %s", current_guest->guest_name, current_guest->guest_email, current_guest->event_id, asctime(current_guest->log_time));
			fprintf(f, "\n");
			current_event = current_event->next;
		}
		fprintf(f, "%s %s %d", current_guest->guest_name, current_guest->guest_email, current_guest->event_id);
	}
	
	fclose(f);
}

int RandomGenerator()
{
	return rand() % 100;
}

void SignalHandler(int signum)
{
	if(signum == SIGUSR1)
	{
		printf("Partner: Megérkeztem a helyszínre!\n");
	}
}

void ManageExhibition(e_node * e_head, g_node * g_head)
{
	e_node * current_event = e_head->next;
	signal(SIGUSR1, SignalHandler);
	
	while(current_event != NULL)
	{
		g_node * current_guest = g_head->next;
		pid_t pid;
		pid_t ppid;
		ppid = getpid();
		char helyszin[100];
		char nev[50];
		int vendegek;
		
		int fd[2];
		if (pipe(fd) == -1) 
		{
			perror("pipe failure");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		
		if (pid == -1) 
		{
			perror("Fork failure");
			exit(EXIT_FAILURE);
		}
	
		if(pid == 0) //child
		{
			close(fd[1]); //close writing end
			if(read(fd[0],&helyszin,sizeof(helyszin)) != sizeof(helyszin))
			{
				perror("reading place");
				exit(EXIT_FAILURE);
			}
			if(read(fd[0],&vendegek,sizeof(vendegek)) != sizeof(vendegek))
			{
				perror("reading number of guests");
				exit(EXIT_FAILURE);
			}
			kill(getppid(), SIGUSR1);
			sleep(1);
			
			if(vendegek > 0)
			{
				char * vendeglista[vendegek];
				int i = 0;
				printf("Vendegek:\n");
				while(read(fd[0],&nev,sizeof(nev)) == sizeof(nev))
				{
					printf("%s\n", nev);
					vendeglista[i] = nev;
					i++;
				}
				int j;
				i = 0;
				for(i; i < vendegek; i++)
				{
					j = RandomGenerator();
					if(j < 10)
					{
						printf("%s nem jelent meg, %d.\n", vendeglista[i],j);
					}
				}
				printf("A rendezvény %d százalék sikerrel zárult.\n", RandomGenerator());
				printf("\n");
			}
			else
			{
				printf("Az esemény érdeklődés hiányában elmarad.\n");
			}
			close(fd[0]);
		}
		else if(pid > 0) //parent
		{
			close(fd[0]); //close reading end
			memset(helyszin,'\0', sizeof(helyszin));
			strcpy(helyszin, current_event->event_place);
			vendegek = current_event->guest_number;
			printf("A kovetkezo rendezveny itt lesz: %s\n", helyszin);
			
			if(write(fd[1], &helyszin, sizeof(helyszin)) != sizeof(helyszin))
			{
				perror("writing place");
				exit(EXIT_FAILURE);
			}
			if(write(fd[1], &vendegek, sizeof(vendegek)) != sizeof(vendegek))
			{
				perror("writing number of guests");
				exit(EXIT_FAILURE);
			}
			sleep(1);
			
			memset(nev,'\0',sizeof(nev));
			while(current_guest != NULL)
			{
				if(current_guest->event_id == current_event->event_id)
				{
					strcpy(nev, current_guest->guest_name);
					if(write(fd[1], &nev, sizeof(nev)) != sizeof(nev))
					{
						perror("writing name of guest");
						exit(EXIT_FAILURE);
					}
					sleep(2);
				}
				current_guest = current_guest->next;
			}
			close(fd[1]);
			sleep(2);
			kill(pid, SIGTERM);
			wait(NULL);
		}
		sleep(1);
		
		sleep(1);
		current_event = current_event->next;	
	}
	
}
