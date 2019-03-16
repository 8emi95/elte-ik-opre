
#include "visitor.h"
#include "visitor_list.h"
#include "event.h"
#include "event_list.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define EVENTS_FILE "events.bin"
#define VISITORS_FILE "visitors.bin"

void unchecked_print(const char*);
char get_choice();
void print_header();
void print_main_menu();
void print_main_menu_legend();
void print_main_menu_simple();
void handle_choice(char, node_t**, node_t**);
void ask_event_data(unsigned*, char*);
void ask_visitor_event_id(unsigned*);
void ask_visitor_details(char*, char*);
void ask_modify_visitor_name(char*);
void ask_modify_visitor_new_event_id(unsigned*, unsigned);
void ask_modify_visitor_new_email(char*, char*);
void ask_modify_event_id(unsigned*);
void read_bounded_string(char *, size_t);
void read_unsigned(unsigned *);

int main() {
  node_t *events = create_event_list_empty();
  node_t *visitors = create_visitor_list_empty();

  read_events_from_file(&events, EVENTS_FILE); 
  read_visitors_from_file(&visitors, VISITORS_FILE); 

  print_header();
  print_main_menu();
  print_main_menu_legend();
  print_main_menu_simple();

  char choice; 

  while ((choice = get_choice()) != 'q') {
    handle_choice(choice, &events, &visitors);
    print_main_menu_simple();
  }

  write_events_to_file(&events, EVENTS_FILE);
  write_visitors_to_file(&visitors, VISITORS_FILE);

  free_events(&events);
  free_visitors(&visitors);

  return 0;
}

void unchecked_print(const char *message) {
  write(STDOUT_FILENO, message, strlen(message) + 1);
}

char get_choice() {
  char buffer[32] = "";
  fgets(buffer, 32, stdin);
  return buffer[0];
}

void print_header() {
  const char header[] = "\
=========================================\n\
Udvozoljuk a King of Stands rendszereben!\n\
=========================================\n";
  system("clear");
  printf(header);
}


void print_main_menu() {
  const char main_options[] = "\
[1] Esemenyek listazasa\n\
[2] Resztvevok listazasa\n\
[3] Esemeny felvetele\n\
[4] Resztvevo felvetele\n\
[5] Resztvevo adatainak modositasa\n\
[6] Rendezveny torlese\n\
[h] Ez a menu\n\
[q] Kilepes\n";
  printf(main_options);
}

void print_main_menu_legend() {
  const char main_legend[] = "\n\
[LE = List Events, LV = List Visitors,\n\
 AE = Add Event, AV = Add Visitor, \n\
 MV = Modify Visitor, DE = Delete Event]\n";
  printf(main_legend);
}

void print_main_menu_simple() {
  const char simple_options[] = "\n\
[ 1 LE, 2 LV, 3 AE, 4 AV, 5 MV, 6 DE, h HELP, q QUIT ]\n";
  printf(simple_options);
}

void handle_choice(char choice, node_t** events, node_t** visitors) {
  switch (choice) {
    case '1': {
	print_events(events);
        break;
	}
    case '2': {
        print_visitors(visitors);
        break;
	}
    case '3': {
        unsigned id;
        char name[MAX_EVENT_NAME_LENGTH];
        ask_event_data(&id, name);
        event_t new_event = create_event(id, name);
        append_event(events, &new_event);
        break;
	}
    case '4': {
        unsigned event_id;
        char name[MAX_VISITOR_NAME_LENGTH];
        char email[MAX_VISITOR_EMAIL_LENGTH];

        ask_visitor_event_id(&event_id);

        node_t* event_node = find_event_by_id(events, event_id);

        if (!event_node) {
          printf("Nincs ilyen azonositoju esemeny!\n");
          break;
        }

        ask_visitor_details(name, email);
        
        
        visitor_t new_visitor = create_visitor(event_id, name, email);
        append_visitor(visitors, &new_visitor);

        unsigned count = count_nodes(visitors);
        printf("%u-es sorszamu latogato hozzaadva!", count);
        break;
	}
    case '5': {
        char name[MAX_VISITOR_NAME_LENGTH];
        ask_modify_visitor_name(name);
        node_t* visitor_node = find_visitor_by_name(visitors, name);

        if (!visitor_node) {
          printf("Nincs ilyen nevu latogato!\n");
          break;
        }

        visitor_t* visitor = (visitor_t *)visitor_node->data;

        unsigned old_event_id = visitor->event_id;
        char *old_email = visitor->email;

        unsigned new_event_id;
        ask_modify_visitor_new_event_id(&new_event_id, old_event_id);

        node_t* event_node = find_event_by_id(events, new_event_id);

        if (!event_node) {
          printf("Nincs ilyen azonositoju esemeny!\n");
          break;
        }

        char new_email[MAX_VISITOR_EMAIL_LENGTH];
        ask_modify_visitor_new_email(new_email, old_email);

        visitor->event_id = new_event_id;
        memcpy(visitor->email, new_email, MAX_VISITOR_EMAIL_LENGTH);
        break;
	}
    case '6': {
        unsigned id;
        ask_modify_event_id(&id);
        printf("Az esemeny torlesevel minden az esemenyt latogato is torlesre kerul. Biztos? i/n ");
        char confirm = get_choice();
        if ('i' != confirm)
	  break;

        delete_event(events, id);
        delete_visitor_all(visitors, id);
        break;
        }
    case 'h': {
        print_main_menu();
        break;
	}
    case 'q': {
        printf("Kilepes...\n");
        break;
	}
  }
}

void ask_event_data(unsigned *id, char *name) {
  printf("Adja meg az esemeny azonositojat: ");
  read_unsigned(id);
  printf("Adja meg az esemeny nevet: ");
  read_bounded_string(name, MAX_EVENT_NAME_LENGTH);
}

void ask_visitor_event_id(unsigned *event_id) {
  printf("Adja meg a latogatott esemeny azonositojat: ");
  read_unsigned(event_id);
}

void ask_visitor_details(char *name, char *email) {
  printf("Adja meg a latogato nevet: ");
  read_bounded_string(name, MAX_VISITOR_NAME_LENGTH);
  printf("Adja meg a latogato email cimet: ");
  read_bounded_string(email, MAX_VISITOR_EMAIL_LENGTH);
}

void ask_modify_visitor_name(char *name) {
  printf("Adja meg a modositando latogato nevet: ");
  read_bounded_string(name, MAX_VISITOR_NAME_LENGTH);
}

void ask_modify_visitor_new_event_id(unsigned *new_event_id, unsigned old_event_id) {
  printf("Adja meg a latogato uj latogatott esemenyenek azonositojat (regi: %u) : ", old_event_id);
  read_unsigned(new_event_id);
}

void ask_modify_visitor_new_email(char *new_email, char *old_email) {
  printf("Adja meg a latogato uj email cimet (regi: %s): ", old_email);
  read_bounded_string(new_email, MAX_VISITOR_EMAIL_LENGTH);
}

void ask_modify_event_id(unsigned* id) {
  printf("Adja meg a torlendo esemeny azonositojat: ");
  read_unsigned(id);
}

void read_bounded_string(char *str, size_t max) {
  fgets(str, max, stdin);
  if ((strlen(str) > 0) && (str[strlen(str) - 1] == '\n'))
    str[strlen(str) - 1] = '\0';
}

void read_unsigned(unsigned *us) {
  char buffer[32];
  fgets(buffer, 32, stdin);
  unsigned long value = strtoul(buffer, NULL, 10);
  *us = value;
}
