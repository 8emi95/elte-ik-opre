
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
void ask_event_data(int*, char*);
void ask_visitor_event_id(int*);
void ask_visitor_details(char*, char*);
void ask_modify_visitor_name(char*);
void ask_modify_visitor_new_event_id(int*, int);
void ask_modify_visitor_new_email(char*, char*);
void ask_modify_event_id(int*);
void read_bounded_string(char *, size_t);
void read_int(int *);

void signal_handler(int signum);

void szulo_tevekenyseg(node_t** events,
                       node_t** visitors,
                       int write_to_child,
                       int read_from_child);
void gyerek_tevekenyseg(int write_to_parent, int read_from_parent);
void run_events(node_t** events, node_t** visitors);

typedef struct visitor_group {
  int num;
  char names[10][255];
} visitor_group;

visitor_group get_visitor_group_for_event(node_t **visitors, int event_id);

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
[7] Rendezvenyek levezenylese\n\
[h] Ez a menu\n\
[q] Kilepes\n";
  printf(main_options);
}

void print_main_menu_legend() {
  const char main_legend[] = "\n\
[LE = List Events, LV = List Visitors, AE = Add Event, AV = Add Visitor, \n\
 MV = Modify Visitor, DE = Delete Event, RE = Run Events]\n";
  printf(main_legend);
}

void print_main_menu_simple() {
  const char simple_options[] = "\n\
[ 1 LE, 2 LV, 3 AE, 4 AV, 5 MV, 6 DE, 7 RE, h HELP, q QUIT ]\n";
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
      int id;
      char name[MAX_EVENT_NAME_LENGTH];
      ask_event_data(&id, name);
      event_t new_event = create_event(id, name);
      append_event(events, &new_event);
      break;
    }
    case '4': {
      int event_id;
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

      int count = count_nodes(visitors);
      printf("%d-es sorszamu latogato hozzaadva!", count);
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

      int old_event_id = visitor->event_id;
      char *old_email = visitor->email;

      int new_event_id;
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
      int id;
      ask_modify_event_id(&id);
      printf("Az esemeny torlesevel minden az esemenyt latogato is torlesre kerul. Biztos? i/n ");
      char confirm = get_choice();
      if ('i' != confirm)
        break;

      delete_event(events, id);
      delete_visitor_all(visitors, id);
      break;
    }
    case '7': {
      run_events(events, visitors);
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

void ask_event_data(int *id, char *name) {
  printf("Adja meg az esemeny azonositojat: ");
  read_int(id);
  printf("Adja meg az esemeny nevet: ");
  read_bounded_string(name, MAX_EVENT_NAME_LENGTH);
}

void ask_visitor_event_id(int *event_id) {
  printf("Adja meg a latogatott esemeny azonositojat: ");
  read_int(event_id);
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

void ask_modify_visitor_new_event_id(int *new_event_id, int old_event_id) {
  printf("Adja meg a latogato uj latogatott esemenyenek azonositojat (regi: %d) : ", old_event_id);
  read_int(new_event_id);
}

void ask_modify_visitor_new_email(char *new_email, char *old_email) {
  printf("Adja meg a latogato uj email cimet (regi: %s): ", old_email);
  read_bounded_string(new_email, MAX_VISITOR_EMAIL_LENGTH);
}

void ask_modify_event_id(int* id) {
  printf("Adja meg a torlendo esemeny azonositojat: ");
  read_int(id);
}

void read_bounded_string(char *str, size_t max) {
  fgets(str, max, stdin);
  if ((strlen(str) > 0) && (str[strlen(str) - 1] == '\n'))
    str[strlen(str) - 1] = '\0';
}

void read_int(int *i) {
  char buffer[32];
  fgets(buffer, 32, stdin);
  int long value = atoi(buffer);
  *i = value;
}

void parent_handler(int signum) {}

void run_events(node_t** events, node_t** visitors) {
  int parent_to_child[2];
  int child_to_parent[2];


  if (pipe(parent_to_child) == -1) 
  {
    perror("Hiba a pipe nyitaskor!");
    exit(EXIT_FAILURE);
  }

  if (pipe(child_to_parent) == -1) 
  {
    perror("Hiba a pipe nyitaskor!");
    exit(EXIT_FAILURE);
  }

  pid_t pid;
  pid = fork();
  
  if (pid == -1) 
  {
    perror("Fork hiba");
    exit(EXIT_FAILURE);
  }
  
  printf("pid: %d\n", pid);

  if (pid > 0) {
    // parent process
    close(child_to_parent[1]);
    close(parent_to_child[0]);
    
    szulo_tevekenyseg(events, visitors, parent_to_child[1], child_to_parent[0]);

    close(child_to_parent[0]); // finally we close the used read end
    close(parent_to_child[1]); // finally we close the used write end
  } else {
    // child process
    close(child_to_parent[0]);
    close(parent_to_child[1]);

    gyerek_tevekenyseg(child_to_parent[1], parent_to_child[0]);

    close(child_to_parent[1]); // finally we close the used write end
    close(parent_to_child[0]); // finally we close the used read end
  }
}

visitor_group get_visitor_group_for_event(node_t **visitors, int event_id) {
  visitor_group current_event_visitors;
  
  int count = 0;
  node_t * visitor_node_it = find_visitor_by_event_id(visitors, event_id);
  while (visitor_node_it) {
    strcpy(current_event_visitors.names[count], ((visitor_t*)(visitor_node_it->data))->name);
    count++;
    visitor_node_it = find_visitor_by_event_id(&(visitor_node_it->next), event_id);
  }

  current_event_visitors.num = count;
  return current_event_visitors;
}

void szulo_tevekenyseg(node_t** events,
                       node_t** visitors,
                       int write_to_child,
                       int read_from_child) {


    printf("Szulo: kezdodik a tevekenyseg\n");
    int num_events = count_nodes(events);
    printf("Szulo: there are %d events\n", num_events);

    printf("Szulo: sending number of events to child...\n");
    write(write_to_child, &current_event_visitors, sizeof(current_event_visitors));
    read(read_from_child, &num_events, sizeof(int));
    printf("Szulo: child acknowledged %d events!\n", num_events);

    int status;
    wait(&status);

    return;

    node_t **event_node_it = events;

    while(*event_node_it) {
      int event_id = ((event_t*)((*event_node_it)->data))->id;
      visitor_group current_event_visitors =  get_visitor_group_for_event(visitors, event_id);
      //int i;
      //for (i = 0; i < count; ++i) {
      //  printf("The %dth visitor for event %d is %s\n", i+1, event_id, current_event_visitors.names[i]);
      //}

      write(write_to_child, &current_event_visitors, sizeof(current_event_visitors));

      printf("Szulo: aludni megyek");
      pause();
      printf("Szulo: felebresztettek!");

      event_node_it = &((*event_node_it)->next);
    }
    

    printf("Szulo: befejeztem!\n");	
}

void gyerek_tevekenyseg(int write_to_parent, int read_from_parent) {
    int num_events;
    printf("Gyerek: varakozik az esemenyek szamara...\n");
    read(read_from_parent, &num_parents, sizeof(int));
    printf("Gyerek: szulo szerint %d esemeny van!", num_events);
    printf("Gyerek: %d esemeny fogadasat igazoljuk vissza!", num_events);
    write(write_to_parent, &num_events, sizeof(int));
    printf("Gyerek: utazom a helyszinre...");
    sleep(1);
    exit(EXIT_SUCCESS);
}

