// 1. beadandó feladat. Beadási határidő: 2016-11-07.
// Készítsen C programot, amely rendszerhívások alkalmazásával megoldja a következő feladatot:

// Egy kiállításokat, rendezvényeket szervező társaság a "King of Stands", elektronikus látogatói nyilvántartást készített. A látogatók a nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, a rendezvény azonosító számát! Az elektronikus rendszer a jelentkezés nyugtázásaként visszaadja, hogy hányadik vendégként történt meg a regisztráció! Az alkalmazás adjon lehetőséget vendég jelentkezéshez, a jelentkezettek listázásához, egy vendég adatainak módosításához, törléséhez. Legyen lehetőségünk egy új rendezvény indításához, egy rendezvény összes adatának törléséhez! A jelentkezőknél a korábbi adatok mellett rögzítsük automatikusan a jelentkezési időt(rendszeridő) is(ezt nem kell bekérni)!

// Készítsünk C programot, amellyel a leírt tevékenység elvégezhető. Az adatokat fájlban tároljuk.

#include "visitor_list.h"
#include "event_list.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void print_menu();
void to_date(time_t raw_time, char date_buffer[]);
void add_visitor(struct visitor_node* visitors, struct event_node* events, const char* visitor_file);
void list_visitors(struct visitor_node* visitors);
void modify_visitor(struct visitor_node* visitors, const char* visitor_file);
void delete_visitor(struct visitor_node* visitors, const char* visitor_file);
void add_event(struct event_node* events, const char* event);
void delete_event(struct event_node* events, const char* event);


int main()
{
	struct visitor_node visitors;
	visitors.next = NULL;
	const char visitor_file[] = "visitors_data.bin";
	load_visitors(&visitors, visitor_file);
	
	struct event_node events;
	events.next = NULL;
	const char event_file[] = "events_data.bin";
	load_events(&events, event_file);

	int choice;
	
	do
	{
		print_menu();
		scanf("%i", &choice);
		
		switch (choice)
		{
			case 1:
				add_visitor(&visitors, &events, visitor_file);
				break;
			case 2:
				list_visitors(&visitors);
				break;
			case 3:
				modify_visitor(&visitors, visitor_file);
				break;
			case 4:
				delete_visitor(&visitors, visitor_file);
				break;
			case 5:
				add_event(&events, event_file);
				break;
			case 6:
				delete_event(&events, event_file);
				break;
			default:
				break;
		}
	}
	while (1 <= choice && choice <= 6);
}

void print_menu()
{
	printf("\nKing of Stands\n");
	printf("~~~~~~~~~~~~~~\n\n");
	printf("1 - Vendég jelentkezése\n");
	printf("2 - A jelentkezettek listázása\n");
	printf("3 - Vendég adatainak módosítása\n");
	printf("4 - Vendég törlése\n");
	printf("5 - Új rendezvény rögzítése\n");
	printf("6 - Rendezvény törlése\n");
	printf("0 - Kilépés\n\n");
	printf("Válasszon menüpontot: ");
}

void add_visitor(struct visitor_node* visitors, struct event_node* events, const char* visitor_file)
{
	char name[100], mail_address[100];
	
	printf("Név: ");
	scanf("%s", name);
	printf("E-mail cím: ");
	scanf("%s", mail_address);
	
	int event_id;
	printf("Rendezvény azonosítója: ");
	scanf("%i", &event_id);
	
	struct event* ev = find_event(events, event_id);
	
	if (ev == NULL)
	{
		printf("Még nincs rögzítve %i azonosítójú rendezvény.\n", event_id);
		return;
	}
	
	
	printf("Vendég rögzítve a rendezvényre %i.-ként.\n", ++ev->visitors);
	
	time_t register_date;
	time(&register_date);
	char date_buffer[15];
	to_date(register_date, date_buffer);
	printf("A jelentkezés dátuma: %s", date_buffer);
	
	struct visitor new_visitor;
	strcpy(new_visitor.name, name);
	strcpy(new_visitor.mail_address, mail_address);
	new_visitor.event_id = event_id;
	new_visitor.register_date = register_date;
	insert_visitor(visitors, &new_visitor);
	
	save_visitors(visitors, visitor_file);
}

void list_visitors(struct visitor_node* visitors)
{
	if (visitors->next == NULL)
	{
		puts("Nincs rögzítve vendég.");
		return;
	}
	
	struct visitor_node* p;
	for (p = visitors->next; p != NULL; p = p->next)
		print_visitor(&p->data);
}

void modify_visitor(struct visitor_node* visitors, const char* visitor_file)
{
	printf("A módosítandó vendég neve: ");
	char name[100], mail_address[100];
	scanf("%s", name);
	struct visitor* visitor_to_modify = find_visitor(visitors, name);
	
	if (visitor_to_modify == NULL)
	{
		printf("%s nem található.\n", name);
		return;
	}
	
	print_visitor(visitor_to_modify);
	
	printf("\nA vendég új neve: ");
	scanf("%s", name);
	strcpy(visitor_to_modify->name, name);
	printf("E-mail címe: ");
	scanf("%s", mail_address);
	strcpy(visitor_to_modify->mail_address, mail_address);
	
	puts("Vendég adatai módosítva.");
	save_visitors(visitors, visitor_file);
}

void delete_visitor(struct visitor_node* visitors, const char* visitor_file)
{
	printf("A törlendő vendég neve: ");
	char name[100];
	scanf("%s", name);
	if (remove_visitor(visitors, name))
	{
		puts("Vendég törölve.\n");
		save_visitors(visitors, visitor_file);
	}
	else
	{
		printf("%s nem található.\n", name);
	}
}

void add_event(struct event_node* events, const char* event_file)
{
	int id;
	printf("A rendezvény azonosítója: ");
	scanf("%i", &id);
	
	struct event new_event;
	new_event.id = id;
	new_event.visitors = 0;
	insert_event(events, &new_event);
	
	puts("Rendezvény rögzítve.");
	save_events(events, event_file);
}

void delete_event(struct event_node* events, const char* event_file)
{
	printf("A törlendő rendezvény azonosítója: ");
	int id;
	scanf("%i", &id);
	if (remove_event(events, id))
	{
		puts("Rendezvény törölve.\n");
		save_events(events, event_file);
	}
	else
	{
		puts("Nem található ilyen rendezvény.");
	}
}
