/** @author Tamas Laszlo (QJHOXY) */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define EVENT_PLACE_LEN 10
#define EVENT_NAME_LEN 128
#define GUEST_NAME_LEN 64
#define GUEST_MAIL_LEN 128

const char* EVENTS_FILE = "events.bin";
const char* GUESTS_FILE = "guests.bin";

/** Structures to store data ****************************************/
struct event_st {
  int id;
  int took_place;
  char name[EVENT_NAME_LEN + 1];
  char place[EVENT_PLACE_LEN + 1];
};

struct guest_st {
  int event_id;
  int guest_id;
  time_t time;
  char name[GUEST_NAME_LEN + 1];
  char mail[GUEST_MAIL_LEN + 1];
};

struct node {
  void* element;
  struct node* next;
};

/** Create name for temporary file **********************************/
/** @param name :: char* - output parameter *************************/
void create_temp_name(char* name)
{
  time_t x = time(0);

  sprintf(name, "%ld-temp.txt", x);
}

/** Read line from console ******************************************/
/** @param label :: const char* - label before reading **************/
/** @param str :: char* - read line, output parameter ***************/
/** @param len :: int - max size ************************************/
void read_str(const char* label, char* str, int len)
{
  printf("%s: ", label);
  fgets(str, len, stdin);
  str[strlen(str) - 1] = 0;
}

/** Read int from console *******************************************/
/** @param label :: const char* - label before reading **************/
/** @return int - read number ***************************************/
int read_int(const char* label)
{
  int temp, x;

  printf("%s: ", label);
  scanf("%d", &x);
  while ((temp = getchar()) != EOF && temp != '\n');

  return x;
}

/** Create new event with the given name ****************************/
void new_event()
{
  FILE* f;
  struct event_st evt, tmp;
  int max_id = -1;

  f = fopen(EVENTS_FILE, "ab+");
  if (f == NULL)
  {
    perror("File opening error\n"); 
    exit(1);
  }

  read_str("Name of the event", evt.name, EVENT_NAME_LEN);

  read_str("Place of the event", evt.place, EVENT_PLACE_LEN);

  while (fread(&tmp, sizeof(tmp), 1, f))
  {
    if (max_id < tmp.id)
      max_id = tmp.id;
  }

  evt.id = max_id + 1;
  evt.took_place = 0;
  printf("Event created with ID %d\n", evt.id);

  f = freopen(EVENTS_FILE, "ab", f);
  if (f == NULL)
  {
    perror("File opening error\n"); 
    exit(1);
  }
  fwrite(&evt, sizeof(evt), 1, f);
  fclose(f);
}

/** List all the events *********************************************/
void list_events()
{
  FILE* f;
  struct event_st evt;
  int size;

  f = fopen(EVENTS_FILE, "rb");
  if (f == NULL)
  {
    printf("There are no events yet\n");
    return;
  }

  printf(" ID %-30s %-30s\n", "NAME", "PLACE");
  while ((size = fread(&evt, sizeof(evt), 1, f)))
  {
    printf("%3d %-30.30s %-30.30s\n", evt.id, evt.name, evt.place);
  }

  fclose(f);
}

/** Delete participant(s) of an event *******************************/
/** @param event_id :: int ******************************************/
/** @param guest_id :: int - if < 0, delete all participants ********/
void delete_guests(int event_id, int guest_id)
{
  FILE *f, *temp_f;
  struct guest_st guest;
  int found;
  char temp_name[100];

  create_temp_name(temp_name);
  f = fopen(GUESTS_FILE, "rb");
  temp_f = fopen(temp_name, "wb");
  if (f == NULL)
  {
    printf("There are no guests yet\n");
    return;
  }
  if (temp_f == NULL)
  {
    perror("File opening error\n");
    exit(1);
  }

  found = 0;
  while (fread(&guest, sizeof(guest), 1, f))
  {
    if (guest.event_id == event_id && 
       (guest_id < 0 || guest.guest_id == guest_id))
      found++;
    else
      fwrite(&guest, sizeof(guest), 1, temp_f);
  }

  fclose(f);
  fclose(temp_f);

  if (found > 0)
  {
    remove(GUESTS_FILE);
    rename(temp_name, GUESTS_FILE);
    printf("%d guest(s) deleted\n", found);
  }
  else
  {
    remove(temp_name);
    printf("Guest at event #%d not found\n", event_id);
  }
}

/** Delete given event and participants *****************************/
void delete_event()
{
  FILE *f, *temp_f;
  struct event_st evt;
  int id, found;
  char temp_name[100];
  
  create_temp_name(temp_name);
  f = fopen(EVENTS_FILE, "rb");
  temp_f = fopen(temp_name, "wb");
  if (f == NULL)
  {
    printf("There are no evens yet\n");
    return;
  }
  if (temp_f == NULL)
  {
    perror("File opening error\n");
    exit(1);
  }

  id = read_int("ID of the event");

  found = 0;
  while (fread(&evt, sizeof(evt), 1, f))
  {
    if (evt.id == id)
      found = 1;
    else
      fwrite(&evt, sizeof(evt), 1, temp_f);
  }
  
  fclose(f);
  fclose(temp_f);

  if (found > 0)
  {
    remove(EVENTS_FILE);
    rename(temp_name, EVENTS_FILE);
    remove(temp_name);
    printf("Event #%d deleted\n", id);
  }
  else
  {
    remove(temp_name);
    printf("Event with id #%d does not exist\n", id);
  }

  delete_guests(id, -1);
}

/** Check if event does exist ***************************************/
/** @param event_id :: int ******************************************/
/** @return int - 1 if exists, 0 otherwise **************************/
int event_exists(int event_id)
{
  FILE *f;
  struct event_st evt;
  int success;

  f = fopen(EVENTS_FILE, "rb");
  if (f == NULL)
  {
    return 0;
  }

  success = 0;
  while (!success && fread(&evt, sizeof(evt), 1, f))
  {
    if (evt.id == event_id)
      success = 1;
  }

  fclose(f);

  return success;
}

/** Get maximum ID of guests at an event ****************************/
/** @param event_id :: int ******************************************/
/** @return int - guests ID *****************************************/
int max_id_at_event(int event_id)
{
  FILE *f;
  struct guest_st guest;
  int max;

  f = fopen(GUESTS_FILE, "rb");
  if (f == NULL)
  {
    printf("There are no events yet\n");
    return -1;
  }

  max = -1;
  while (fread(&guest, sizeof(guest), 1, f))
  {
    if (guest.event_id == event_id && guest.guest_id > max)
      max = guest.guest_id;
  }

  fclose(f);

  return max;
}

/** Create new guest with the given datas ***************************/
void new_guest()
{
  FILE *f;
  int temp;
  struct guest_st guest;

  f = fopen(GUESTS_FILE, "ab");
  if (f == NULL)
  {
    perror("File opening error\n");
    exit(1);
  }

  printf("ID of the event: ");
  scanf("%d", &guest.event_id);
  while ((temp = getchar()) != EOF && temp != '\n');
  if (!event_exists(guest.event_id))
  {
    printf("No event with ID #%d\n", guest.event_id);
    return;
  }

  read_str("Name of the guest", guest.name, GUEST_NAME_LEN);
  read_str("Email address of the guest", guest.mail, GUEST_MAIL_LEN);

  guest.time = time(0);

  guest.guest_id = max_id_at_event(guest.event_id) + 1;

  fwrite(&guest, sizeof(guest), 1, f);
  printf("New guest successfully added with ID #%d\n", guest.guest_id);

  fclose(f);
}

/** Convert date to formatted string ********************************/
/** @param num :: int ***********************************************/
/** @param str :: char* - output parameter **************************/
void time_to_str(time_t num, char* str)
{
  struct tm time_st = *localtime(&num);

  sprintf(str, "%d.%02d.%02d. %02d:%02d",
    1900 + time_st.tm_year, time_st.tm_mon, time_st.tm_mday,
    time_st.tm_hour, time_st.tm_min);
}

/** List all the guests *********************************************/
void list_guests()
{
  FILE* f;
  struct guest_st guest;
  char time_str[100];

  f = fopen(GUESTS_FILE, "rb");
  if (f == NULL)
  {
    printf("There are no guests yet\n");
    return;
  }

  printf(" EVENT_ID ID %-17s %-20s %-20s\n",
    "TIME", "NAME", "EMAIL");
  while (fread(&guest, sizeof(guest), 1, f))
  {
    time_to_str(guest.time, time_str);
    printf(" %8d %2d %s %-20.20s %-25.25s\n", 
      guest.event_id, guest.guest_id, time_str, guest.name, guest.mail);
  }

  fclose(f);
}

/** Delete given guest **********************************************/
void delete_guest()
{
  int event_id, guest_id;

  event_id = read_int("ID of the event");
  guest_id = read_int("ID of the guest");
  delete_guests(event_id, guest_id);
}

/** Edit given guest ************************************************/
void edit_guest()
{
  int event_id = -1, guest_id = -1, found;
  FILE *f;
  struct guest_st guest;

  event_id = read_int("ID of the event");
  guest_id = read_int("ID of the guest");

  f = fopen(GUESTS_FILE, "rb+");
  if (f == NULL)
  {
    printf("There are no guests yet\n");
    return;
  }

  found = 0;
  while (fread(&guest, sizeof(guest), 1, f))
  {
    if (guest.event_id == event_id && guest.guest_id == guest_id)
    {
      found = 1;
      break;
    }
  }

  if (found != 0)
  {
    printf("Current name: %s\n", guest.name);
    read_str("New name of the guest", guest.name, GUEST_NAME_LEN);

    printf("Current email address: %s\n", guest.mail);
    read_str("New email address of the guest", guest.mail, GUEST_MAIL_LEN);

    guest.time = time(0);

    fseek(f, -sizeof(guest), SEEK_CUR);
    fwrite(&guest, sizeof(guest), 1, f);

    printf("Guest updated\n");
  }
  else
    printf("Guest not found\n");

  fclose(f);
}

void handler(int x) { x = x; }
/** Send organizer **************************************************/
void send_organizer(const char* place, int id)
{
  int pipefd[2];
  pid_t pid;
  char msg[256];
  FILE* f;
  struct guest_st guest;
  
  struct sigaction sigact;
  sigact.sa_handler=handler;
  sigemptyset(&sigact.sa_mask);
  sigact.sa_flags=0;
  sigaction(SIGUSR1,&sigact,NULL);

  if (pipe(pipefd) == -1)
  {
    perror("Error at opening pipe");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if (pid < 0)
  {
    perror("Fork error");
    exit(EXIT_FAILURE);
  }

  /* PARENT */
  if (pid != 0)
  {
    sigset_t sigset;
    sigfillset(&sigset);
    sigdelset(&sigset,SIGUSR1);

    close(pipefd[0]);
    write(pipefd[1], place, sizeof(place));
    sigsuspend(&sigset);

    f = fopen(GUESTS_FILE, "rb");
    if (f != NULL)
    {
      while (fread(&guest, sizeof(guest), 1, f))
      {
        if (guest.event_id == id)
        {
          write(pipefd[1], guest.name, sizeof(guest.name));
          sigsuspend(&sigset);
        }
      }
      fclose(f);
    }
    close(pipefd[1]);
    waitpid(pid, 0, 0);
  }
  /* CHILD */
  else
  {
    struct node *head = 0, *last;
    close(pipefd[1]);
    read(pipefd[0], msg, sizeof(msg));
    kill(getppid(), SIGUSR1);
    printf("Organizer: I have arrived to '%s'\n", msg);
    printf("Organizer: List of guests:\n");
    while (read(pipefd[0], msg, sizeof(msg)) > 0)
    {
      kill(getppid(), SIGUSR1);
      printf("  %s\n", msg);

      if (!head)
      {
        head = (struct node*) malloc(sizeof(struct node));
        last = head;
      }
      else
      {
        (*last).next = (void*) malloc(sizeof(struct node));
        last = (*last).next;
      }
      (*last).next = 0;
      (*last).element = (void*) malloc(strlen(msg) + 1);
      memcpy((*last).element, msg, strlen(msg) + 1);
    }

    printf("These guests did not come:\n");
    last = head;
    srand (time(NULL));
    while (last)
    {
      if (!(rand() % 10))
        printf("  %s\n", (char*) ((*last).element));
      last = (*last).next;
    }

    while (head)
    {
      last = head;
      head = (*last).next;
      free((char*) (*last).element);
      free(last);
    }

    exit(0);
  }
}

/** Start next event ************************************************/
void start_event()
{
  FILE *f;
  struct event_st evt;
  int read = 0;

  f = fopen(EVENTS_FILE, "rb+");
  if (f == NULL)
  {
    printf("There are no events yet\n");
    return;
  }

  while ((read = fread(&evt, sizeof(evt), 1, f)) && evt.took_place != 0);

  if (read && evt.took_place == 0)
  {
    printf("Event #%d '%s' will start soon\n", evt.id, evt.name);

    send_organizer(evt.place, evt.id);

    fseek(f, -sizeof(evt), SEEK_CUR);
    evt.took_place = 1;
    fwrite(&evt, sizeof(evt), 1, f);
    printf("Event #%d '%s' took place\n", evt.id, evt.name);
  }
  else
  {
    printf("There is no upcoming event\n");
  }

  fclose(f);
}

/** Print menu ******************************************************/
void print_menu()
{
  printf("\nSelect menu item by index\n");
  printf("(1) New event\n");
  printf("(2) List events\n");
  printf("(3) Delete event\n");
  printf("(4) New guest\n");
  printf("(5) List guests\n");
  printf("(6) Edit guest\n");
  printf("(7) Delete guest\n");
  printf("(8) Start next event\n");
  printf("(0) Exit\n");
}

/** Select menu item ************************************************/
/** @param code :: int - selected item ******************************/
void select_item(int code)
{
  switch (code)
  {
    case 1: new_event();      break;
    case 2: list_events();    break;
    case 3: delete_event();   break;
    case 4: new_guest();      break;
    case 5: list_guests();    break;
    case 6: edit_guest();     break;
    case 7: delete_guest();   break; 
    case 8: start_event();    break;
  }
}

/** Main program ****************************************************/
int main()
{
  int code, temp;
  
  do
  {
    print_menu();
    scanf("%d", &code);
    while ((temp = getchar()) != EOF && temp != '\n');
    
    select_item(code);
  }
  while (code != 0); 

  return 0;
}
