
#include "visitor.h"
#include "visitor_list.h"
#include "event.h"
#include "event_list.h"

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
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

void ready_signal_handler(int signum);

void szulo_tevekenyseg(node_t** events,
    node_t** visitors,
    int write_to_child,
    int read_from_child);
void gyerek_tevekenyseg(int write_to_parent, int read_from_parent);
void run_events(node_t** events, node_t** visitors);

typedef struct visitor_group {
  int num;
  char names[16][255];
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

void ready_signal_handler(int signum) {}

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

  if (pid > 0) {
    // parent process
    close(child_to_parent[1]);
    close(parent_to_child[0]);

    szulo_tevekenyseg(events, visitors, parent_to_child[1], child_to_parent[0]);

    close(child_to_parent[0]); // finally we close the used read end
    close(parent_to_child[1]); // finally we close the used write end

    int status;
    waitpid(pid, &status, 0);

    printf("Szulo: gyerek megerkezett, vege a lebonyolitasnak!");	
    fflush(stdout);
  } else {
    // child process
    close(child_to_parent[0]);
    close(parent_to_child[1]);

    gyerek_tevekenyseg(child_to_parent[1], parent_to_child[0]);

    close(child_to_parent[1]); // finally we close the used write end
    close(parent_to_child[0]); // finally we close the used read end
    exit(EXIT_SUCCESS);
  }
}

void handler(int signum) {}

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

  sigset_t sigset;
  sigfillset(&sigset);
  sigdelset(&sigset,SIGUSR1);
  sigprocmask(SIG_BLOCK,&sigset,NULL);

  signal(SIGUSR1, ready_signal_handler);

  printf("Szulo: kezdodik a lebonyolitas!\n");
  fflush(stdout);
  int num_events = count_nodes(events);
  printf("Szulo: %d esemeny van a nyilvantartasban!\n", num_events);
  fflush(stdout);

  printf("Szulo: esemenyek darabszamanak elkuldese a gyereknek...\n");
  fflush(stdout);
  write(write_to_child, &num_events, sizeof(int));
  read(read_from_child, &num_events, sizeof(int));
  printf("Szulo: gyerek tudomasul vett %d darab esemenyt!\n", num_events);
  fflush(stdout);

  int event_index = 0;
  node_t **event_node_it = events;
  while(*event_node_it) {
    int event_id = ((event_t*)((*event_node_it)->data))->id;
    visitor_group visitor_candidates =  get_visitor_group_for_event(visitors, event_id);

    printf("Szulo: a regisztracio szerint %d latogato fog megjelenni, a kovetkezok:\n", visitor_candidates.num);
    fflush(stdout);
    int visitor_index;
    for (visitor_index = 0; visitor_index < visitor_candidates.num; ++visitor_index) {
      printf("%s\n", visitor_candidates.names[visitor_index]);
      fflush(stdout);
    }

    printf("Szulo: %d. esemeny (id = %d) elküldese...\n", event_index + 1, event_id);
    fflush(stdout);
    write(write_to_child, &visitor_candidates, sizeof(visitor_candidates));

    printf("Szulo: varom hogy a gyerek odaerjen, aludni megyek...\n");
    fflush(stdout);
    pause();
    printf("Szulo: szolt a gyerek, hogy ott van!\n");
    fflush(stdout);

    printf("Szulo: megnezem mi lett az eredmeny!\n");
    fflush(stdout);

    visitor_group visitors_arrived;
    read(read_from_child, &visitors_arrived, sizeof(visitor_group));

    printf("Szulo: a rendezvenyen %d latogato jelent meg a %d bol!\n", visitors_arrived.num, visitor_candidates.num);
    fflush(stdout);

    ++event_index;
    event_node_it = &((*event_node_it)->next);
  }

}

void gyerek_tevekenyseg(int write_to_parent, int read_from_parent) {
  srand(getpid());

  int num_events;
  printf("Gyerek: varakozik az esemenyek szamara...\n");
  fflush(stdout);
  read(read_from_parent, &num_events, sizeof(int));
  printf("Gyerek: szulo szerint %d esemeny van!\n", num_events);
  fflush(stdout);
  printf("Gyerek: %d esemeny fogadasat igazoljuk vissza!\n", num_events);
  fflush(stdout);
  write(write_to_parent, &num_events, sizeof(int));

  int event_index;
  for (event_index = 0; event_index < num_events; ++event_index) {
    visitor_group visitor_candidates;
    read(read_from_parent, &visitor_candidates, sizeof(visitor_group));

    printf("Gyerek: megkaptam a regisztraciot, utazom a helyszinre...\n");
    fflush(stdout);
    sleep(3);
    printf("Gyerek: a helyszinre ertem!\n");
    fflush(stdout);

    kill(getppid(), SIGUSR1);
    
    visitor_group visitors_arrived;

    visitors_arrived.num = 0;
    int visitor_index;
    for (visitor_index = 0; visitor_index < visitor_candidates.num; ++visitor_index) {
      if (rand() % 2) {
        printf("Gyerek: %s nevu latogato megerkezett!\n", visitor_candidates.names[visitor_index]);
        fflush(stdout);
        strcpy(visitors_arrived.names[visitors_arrived.num], visitor_candidates.names[visitor_index]);
        ++visitors_arrived.num;
      } else {
        printf("Gyerek: %s nevu latogato nem erkezett meg!\n", visitor_candidates.names[visitor_index]);
        fflush(stdout);
      }
    }

    printf("Gyerek: mindenki feldolgozva!\n");
    fflush(stdout);

    write(write_to_parent, &visitors_arrived, sizeof(visitor_group));
  }
}

