#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int bool;
#define true 1
#define false 0

typedef struct EventNode
{
	char event_name [50];
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

int main(int argc, char ** argv)
{
	e_node * e_head = NULL;
	e_head = malloc(sizeof(e_node));
	if(e_head == NULL)
	{
		return 1;
	}
	
	strcpy(e_head -> event_name, "ures");
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
		}
	}
	while(cmd != 0);
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
	
	int id;
	printf("Esemeny azonositoja: ");
	scanf("%d", &id);
	
	current->next = malloc(sizeof(e_node));
	strcpy(current->next->event_name, name);
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
