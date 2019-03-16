#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h> //open,creat
#include <unistd.h>
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include "datatypes.h"

#define VERT_SPACER "-------------------------------\n"

// helper functions
int is_guest_list_empty(guest_list* g_list)
{
	return g_list->head->next == NULL;
}

int is_event_list_empty(event_list* ev_list)
{
	return ev_list->head->next == NULL;
}

int is_event_exist(size_t id, event_list* ev_list)
{
	event_node* ev_node = ev_list->head->next;

	while (ev_node != NULL && ev_node->event.id != id)
	{
		ev_node = ev_node->next;
	}

	return ev_node != NULL;
}

void read_guest_data(guest_data* guest, event_list* ev_list)
{
	// vendeg adatainak bekerese
	printf("\nAdd meg a nevet: ");
	scanf("%[^\n]s", &guest->name);
	getchar();

	printf("\nAdd meg az e-mail cimet: ");
	scanf("%s", &guest->email);
	getchar();

	do
	{
		printf("\nAdd meg a rendezveny azonositojat: ");
		scanf("%zu", &guest->event_id);
		getchar();
	} 
	while (is_event_exist(guest->event_id, ev_list) == 0);
	
}

void read_event_data(event_data* event, event_list* ev_list)
{
	do
	{
		printf("\nAdd meg a rendezveny azonositojat: ");
		scanf("%zu", &event->id);
		getchar();
	}
	while (is_event_exist(event->id, ev_list) == 1);

	printf("\nAdd meg a rendezveny nevet: ");
	scanf("%[^\n]s", &event->name);
	getchar();

	printf("\nAdd meg a rendezveny helyszinet: ");
	scanf("%[^\n]s", &event->place);
	getchar();
}

guest_data* find_guest(size_t id, guest_list* g_list)
{
	guest_node* result = g_list->head;

	while (result != NULL && id != result->guest.id)
	{
		result = result->next;
	}

	return &(result->guest);
}

// printer functions
void print_menu()
{
	printf("1 - vendegek listazasa\n");
	printf("2 - vendeg jelentkezese\n");
	printf("3 - vendeg adatainak modositasa\n");
	printf("4 - vendeg adatainak torlese\n");
	printf("5 - rendezveny inditasa\n");
	printf("6 - rendezveny adatainak listazasa\n");
	printf("7 - rendezveny torlese\n");
	printf("M - adatok mentese\n");
	printf("ESC - kilepes\n");
}

void print_guest(guest_data* g)
{
	printf("azon: %zu | nev: %s | e-mail: %s | rend: %zu | reg_ido: %s", g->id, g->name, g->email, g->event_id, ctime(&g->reg_time));
}

void print_event(event_data* e)
{
	printf("azon: %zu | nev: %s | helyszin: %s\n", e->id, e->name, e->place);
}
// list functions
void list_events(event_list* ev_list)
{
	if (is_event_list_empty(ev_list))
	{
		printf("A rendezvenyek listaja ures.\n");
		return;
	}
	
	event_node* curr_ev = ev_list->head->next;

	printf(VERT_SPACER);
	printf("Rendezvenyek listaja: \n");

	while (curr_ev != NULL)
	{
		print_event(&curr_ev->event);
		curr_ev = curr_ev->next;
	}
	
}

// save functions
void save_guests(size_t guest_id, guest_list* g_list)
{
	int guest_file = open("guests.dat", O_CREAT | O_WRONLY | O_TRUNC);
	guest_node* curr_guest = g_list->head->next;
	
	write(guest_file, &guest_id, sizeof(guest_id));
	while (curr_guest != NULL)
	{
		write(guest_file, &curr_guest->guest, sizeof(guest_data));
		curr_guest = curr_guest->next;
	}

	close(guest_file);
}

void save_events(event_list* ev_list)
{
	int event_file = open("events.dat", O_CREAT | O_WRONLY | O_TRUNC);
	event_node* curr_event = ev_list->head->next;

	while (curr_event != NULL)
	{
		write(event_file, &curr_event->event, sizeof(event_data));
		curr_event = curr_event->next;
	}

	close(event_file);
}

// load functions
void load_guests(size_t* guest_id, guest_list* g_list)
{
	g_list->head = (guest_node*)malloc(sizeof(guest_node*));
	g_list->last = g_list->head;
	g_list->head->next = NULL;
	
	guest_data guest;
	int guest_file = open("guests.dat", O_RDONLY);

	// ha ures a fajl, akkor 0-ra allitjuk a kioszthato vendegazonositot
	if (read(guest_file, guest_id, sizeof(*guest_id)) != sizeof(*guest_id))
	{
		*guest_id = 0;
		return;
	}
	
	while (read(guest_file, &guest, sizeof(guest)))
	{
		guest_node* p = (guest_node*)malloc(sizeof(guest_node));
		p->guest.id = guest.id;
		strcpy(p->guest.name, guest.name);
		strcpy(p->guest.email, guest.email);
		p->guest.event_id = guest.event_id;
		p->guest.reg_time = guest.reg_time;
		p->next = NULL;

		g_list->last->next = p;
		g_list->last = p;
	}
	
	close(guest_file);
}

void load_events(event_list* ev_list)
{
	ev_list->head = (event_node*)malloc(sizeof(event_node*));
	ev_list->last = ev_list->head;
	ev_list->head->next = NULL;

	event_data event;
	int ev_file = open("events.dat", O_RDONLY);
	while (read(ev_file, &event, sizeof(event)))
	{
		event_node* p = (event_node*)malloc(sizeof(event_node));
		p->event.id = event.id;
		strcpy(p->event.name, event.name);
		strcpy(p->event.place, event.place);
		p->next = NULL;

		ev_list->last->next = p;
		ev_list->last = p;
	}

	close(ev_file);
}

// menu functions
void list_all_guests(guest_list* g_list)
{
	printf(VERT_SPACER);

	if (is_guest_list_empty(g_list))
	{	
		printf("A vendegek listaja ures.\n");
		return;
	}
	
	guest_node* curr_guest = g_list->head->next;
	printf("Vendegek listaja:\n\n");

	while (curr_guest != NULL)
	{
		print_guest(&curr_guest->guest);
		curr_guest = curr_guest->next;
	}
}

void registrate_guest(size_t* guest_id, guest_list* g_list, event_list* ev_list)
{
	printf(VERT_SPACER);
	if (ev_list->head->next == NULL)
	{
		printf("A rendezvenyek listaja ures.\n");
		return;
	}
	
	guest_node* new_guest = (guest_node*)malloc(sizeof(guest_node));
	new_guest->next = NULL;

	printf("Uj vendeg felvetele:\n");

	// vendeg adatainak bekerese
	read_guest_data(&new_guest->guest, ev_list);

	(*guest_id)++;
	new_guest->guest.id = *guest_id;
	new_guest->guest.reg_time = time(NULL);

	// vendeg befuzese
	g_list->last->next = new_guest;
	g_list->last = new_guest;

	printf("\n%zu. vendeg sikeresen regisztralva\n", *guest_id);
}

void modify_guest_data(guest_list* g_list, event_list* ev_list)
{	
	if (is_guest_list_empty(g_list))
	{
		printf(VERT_SPACER);
		printf("A vendegek listaja ures.\n");
		return;
	}
	
	list_all_guests(g_list);

	size_t id;
	printf("\nAdd meg modositani kivant vendeg azonositojat: ");
	scanf("%zu", &id);
	getchar();

	guest_data* guest = find_guest(id, g_list);

	if (guest != NULL)
	{
		read_guest_data(guest, ev_list);
		printf("Adatok modositva.\n");
	}
	else
	{
		printf("Nincs ilyen azonositoju vendeg!\n");
	}
}

void delete_guest(guest_list* g_list)
{
	if (is_guest_list_empty(g_list))
	{
		printf(VERT_SPACER);
		printf("A vendegek listaja ures.\n");
		return;
	}
	
	list_all_guests(g_list);

	size_t id;
	printf("\nAdd meg torolni kivant vendeg azonositojat: ");
	scanf("%zu", &id);
	getchar();

	guest_node* prev_guest = g_list->head;
	guest_node* curr_guest = prev_guest->next;

	while (curr_guest != NULL && id != curr_guest->guest.id)
	{
		prev_guest = curr_guest;
		curr_guest = curr_guest->next;
	}

	if (curr_guest != NULL)
	{
		if (g_list->last == curr_guest)
		{
			g_list->last = prev_guest;
		}

		prev_guest->next = curr_guest->next;
		free(curr_guest);
		curr_guest = NULL;
		
		printf("Vendeg torolve.\n");
	}
	else
	{
		printf("Nincs ilyen azonositoju vendeg!\n");
	}
}

void launch_event(event_list* ev_list)
{
	printf(VERT_SPACER);
	printf("Uj rendezveny inditasa:\n");

	// esemény létrehozása és bekérése
	event_node* new_event_node = (event_node*)malloc(sizeof(event_node));
	new_event_node->next = NULL;
	read_event_data(&new_event_node->event, ev_list);

	// új esemény befûzése
	ev_list->last->next = new_event_node;
	ev_list->last = new_event_node;

	printf("Rendezveny sikeresen letrehozva\n");
}

void list_event_visitors(event_list* ev_list, guest_list* g_list)
{	
	if (is_event_list_empty(ev_list))
	{
		printf(VERT_SPACER);
		printf("A rendezvenyek listaja ures.\n");
		return;
	}
	
	list_events(ev_list);

	// listazando rendezveny kivalasztasa
	size_t id;
	printf("\nAdd meg a listazando rendezveny azonositojat: ");
	scanf("%zu", &id);
	getchar();

	// vendégek adatainak kiírása
	guest_node* g_node = g_list->head;
	printf("\nVendegek listaja:\n");

	size_t guest_num = 0;
	while (g_node != NULL)
	{
		if (g_node->guest.event_id == id)
		{
			++guest_num;
			print_guest(&g_node->guest);
		}
		g_node = g_node->next;
	}

	if (guest_num == 0)
	{
		printf("A vendegek listaja ures.\n");
	}
}

void delete_event(event_list* ev_list, guest_list* g_list)
{
	if (ev_list->head->next == NULL)
	{
		printf(VERT_SPACER);
		printf("A rendezvenyek listaja ures.\n");
		return;
	}
	list_events(ev_list);

	size_t id;
	printf("Adja meg a torolni kivant rendezveny azonositojat: ");
	scanf("%zu", &id);
	getchar();

	if (is_event_exist(id, ev_list))
	{
		// rendezvenyhez tartozo vendegek regisztraciojanak torlese
		guest_node* prev_guest_node = g_list->head;
		guest_node* curr_guest_node = prev_guest_node->next;

		while (curr_guest_node != NULL)
		{
			if (curr_guest_node->guest.event_id == id)
			{
				if (g_list->last == curr_guest_node)
				{
					g_list->last = prev_guest_node;
				}
				
				guest_node* p = curr_guest_node;
				prev_guest_node->next = curr_guest_node->next;
				curr_guest_node = prev_guest_node->next;

				free(p);
				p = NULL;
			}
			else
			{
				prev_guest_node = curr_guest_node;
				curr_guest_node = curr_guest_node->next;
			}
		}

		// rendezveny torlese
		event_node* prev_event_node = ev_list->head;
		event_node* curr_event_node = prev_event_node->next;

		while (curr_event_node != NULL && curr_event_node->event.id != id)
		{
			prev_event_node = curr_event_node;
			curr_event_node = curr_event_node->next;
		}

		if (curr_event_node != NULL)
		{
			if (ev_list->last == curr_event_node)
			{
				ev_list->last = prev_event_node;
			}
			
			prev_event_node->next = curr_event_node->next;
			free(curr_event_node);
			curr_event_node = NULL;
		}

		printf("Rendezveny sikeresen torolve.\n");
	}
	else
	{
		printf("Nincs ilyen rendezveny.\n");
	}
}

int main()
{
	printf("\nKing of Stands latogatoi nyilvantartas\n");
	
	event_list ev_list;
	guest_list g_list;
	size_t guest_id = 0;;
	
	load_events(&ev_list);
	load_guests(&guest_id, &g_list);
	
	char choice;
	
	do
	{
		printf(VERT_SPACER);
		print_menu();
		printf("opcio: ");
		choice = getchar();
		getchar();

		switch (choice)
		{
			case '1' :
				list_all_guests(&g_list);
				break;
			case '2':
				registrate_guest(&guest_id, &g_list, &ev_list);
				break;
			case '3':
				modify_guest_data(&g_list, &ev_list);
				break;
			case '4':
				delete_guest(&g_list);
				break;
			case '5':
				launch_event(&ev_list);
				break;
			case '6':
				list_event_visitors(&ev_list, &g_list);
				break;
			case '7':
				delete_event(&ev_list, &g_list);
				break;
			case 'm':
			case 'M':
			{
				save_guests(guest_id, &g_list);
				save_events(&ev_list);
				break;
			}
		}
	}
	while (choice != 27);
	
	return 0;
}