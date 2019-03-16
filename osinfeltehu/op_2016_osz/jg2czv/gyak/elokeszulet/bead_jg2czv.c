#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

struct Guests
{
  char name[20];
  char email[35];
  int eventId;
  char registrationTime[26];
};

struct Events
{
  int id;
  char location[40];
};

// --------------------------- //
// ------ Save to file ------- //
// --------------------------- //

void saveGuests(struct Guests* guestList, int* guestListSize)
{
  remove("guests.txt");

  struct Guests* ptr = guestList;
  struct Guests* endPtr = guestList + (*guestListSize);

  if(ptr != endPtr) 
  {
	  FILE* f=fopen("guests.txt","w");
	  if (f==NULL){perror("File opening error\n"); exit(1);}
	  while ( ptr < endPtr ){
	    fprintf(f, "%s\r\n", ptr->name);
	    fprintf(f, "%s\r\n", ptr->email);
	    fprintf(f, "%d\r\n", ptr->eventId);
	    fprintf(f, "%s\r\n", ptr->registrationTime);
	    ptr++;
	  };
	  fclose(f);
  }

  printf("Guests saved\n");
}

void saveEvents(struct Events* eventList, int* eventListSize)
{
  remove("events.txt");

  struct Events* ptr = eventList;
  struct Events* endPtr = eventList + *eventListSize;

  if(ptr != endPtr){
	  FILE* f=fopen("events.txt","w");
	  if (f==NULL){perror("File opening error\n"); exit(1);}
  	while ( ptr < endPtr ){
    	fprintf(f, "%d\r\n", ptr->id);
    	fprintf(f, "%s\r\n", ptr->location);
    	ptr++;
  	};
	  fclose(f);
  }

  printf("Events saved\n");
}

void saveAll(struct Guests* guestList , struct Events* eventList, int* guestListSize, int* eventListSize)
{

  saveGuests(guestList, guestListSize);
  saveEvents(eventList, eventListSize);
  printf("All lists has been saved\n");
}

void getData(const char* question, char* input)
{
  printf("%s\n",question);
  scanf("%s",input);
}

// --------------------------- //
// ------- Initialize -------- //
// --------------------------- //

void initLists(struct Guests* guestList , struct Events* eventList, int* guestListSize, int* eventListSize)
{
  FILE* f;

  f=fopen("guests.txt","r");
  char line[160];
  if (f!=NULL){
    while (!feof(f)){
      fgets(line,sizeof(line),f);
      if (!feof(f))
      {
        int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-2] = '\0';
        //guestList = realloc(guestList, (*guestListSize + 1) * sizeof(struct Guests));
        strcpy(guestList[*guestListSize].name, line);
        fgets(line,sizeof(line),f);
        len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-2] = '\0';
        strcpy(guestList[*guestListSize].email, line);
        fgets(line,sizeof(line),f);
        len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-2] = '\0';
        guestList[*guestListSize].eventId = (int) strtol(line, NULL, 10);
        fgets(line,sizeof(line),f);
        len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-2] = '\0';
        strcpy(guestList[*guestListSize].registrationTime, line);
        (*guestListSize)++;
      }
    } 
    fclose(f);
  }

  f=fopen("events.txt","r");
  if (f!=NULL){
    while (!feof(f)){
      fgets(line,sizeof(line),f);
      if (!feof(f))
      {
        int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-2] = '\0';
        //eventList = realloc(eventList, (*eventListSize + 1) * sizeof(struct Events));
        eventList[*eventListSize].id = (int) strtol(line, NULL, 10);
        fgets(line,sizeof(line),f);
        len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-2] = '\0';
        strcpy(eventList[*eventListSize].location, line);
        (*eventListSize)++;
      }
    } 
    fclose(f);
  }

  printf("Lists has been initialized\n");
}

// --------------------------- //
// -------- Guests ----------- //
// --------------------------- //

void listGuests(struct Guests* guestList, int* guestListSize)
{
  int index = 0;
    printf("Lists before initialized: %d\n",*guestListSize);
  while(index < *guestListSize)
  {
    printf("%s - %s - %d - %s\n", 
      guestList[index].name, 
      guestList[index].email, 
      guestList[index].eventId, 
      guestList[index].registrationTime);
    ++index;
  }
  printf("Guests are listed\n");
}

int addGuest(struct Guests* guestList, int* guestListSize)
{
  //guestList = realloc(guestList, (*guestListSize + 1) * sizeof(struct Guests));
  //guestList[*guestListSize + 1] = malloc(sizeof(guestList[*guestListSize+1]));

  char name[20];
  getData("Name of the new guest:", name);
  strcpy(guestList[*guestListSize].name, name);

  char email[35];
  getData("Email of the new guest:", email);
  strcpy(guestList[*guestListSize].email, email);
  
  char eventId[4];
  getData("Event Id(max 4 number) wich the guest goes to:", eventId);
  guestList[*guestListSize].eventId = (int) strtol(eventId, NULL, 10);

  time_t rawtime;
  struct tm* timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  strftime(guestList[*guestListSize].registrationTime, 26, "%Y-%m-%d %H:%M:%S", timeinfo);

  ++(*guestListSize);
  printf("Added a guest\n");

  saveGuests(guestList, guestListSize);
  return (*guestListSize);
}

void modifyGuest(struct Guests* guestList, int* guestListSize)
{
  char tmpIndex[2];
  getData("Type the number of the modifyable guest:", tmpIndex);
  int index =((int) strtol(tmpIndex, NULL, 10))-1;

  char name[20];
  getData("New name of the selected guest:", name);
  strcpy(guestList[index].name, name);

  char email[35];
  getData("New email of the selected guest:", email);
  strcpy(guestList[index].email, email);
  
  char eventId[4];
  getData("Event Id(max 4 number) wich the selected guest goes to:", eventId);
  guestList[index].eventId = (int) strtol(eventId, NULL, 10);

  printf("Modified a guest\n");
  saveGuests(guestList, guestListSize);
}

void removeGuest(struct Guests* guestList, int* guestListSize)
{
  char tmpIndex[2];
  getData("Type the number of the removable guest:", tmpIndex);
  int index =((int) strtol(tmpIndex, NULL, 10))-1;
  
  while(index < *guestListSize)
  {
    guestList[index]=guestList[index+1];
    index++;
  }

  (*guestListSize)--;

  printf("Removed a guest\n");
  saveGuests(guestList, guestListSize);
}

// --------------------------- //
// -------- Events ----------- //
// --------------------------- //

void listEvents(struct Events* eventList, int* eventListSize)
{
  int index = 0;
  while(index < *eventListSize)
  {
    printf("%d - %s\n", 
      eventList[index].id, 
      eventList[index].location); 
    ++index;
  }
  printf("Events are listed\n");
}

void addEvent(struct Events* eventList, int* eventListSize)
{
  //eventList = realloc(eventList, (*eventListSize + 1) * sizeof(struct Events));

  char id[5];
  getData("Type the id(max 4 number) of the new event:", id);
  eventList[*eventListSize].id = (int) strtol(id, NULL, 10);
  char location[40];
  getData("Type the location of the new event:", location);
  strcpy(eventList[(*eventListSize)++].location, location);

  printf("Added an event %d\n", *eventListSize);
  saveEvents(eventList, eventListSize);
}

void removeEvent(struct Events* eventList, int* eventListSize)
{
  char tmpIndex[2];
  getData("Type the number of the removable event:", tmpIndex);
  int index =((int) strtol(tmpIndex, NULL, 10))-1;
  
  while(index < *eventListSize)
  {
    eventList[index]=eventList[index+1];
    index++;
  }

  (*eventListSize)--;
  printf("%d\n", eventList[0].id);

  printf("Removed an event\n");
  saveEvents(eventList, eventListSize);
}

// --------------------------- //
// ---------- Fork ----------- //
// --------------------------- //

void handler(int signumber)
{
  // printf("\nSignal has arrived: %d\n", signumber);
}

void make_pipe(char* name)
{
  printf("Esemenykezeles elinditasa (cso inditas)!\n");
  int fid=mkfifo(name, S_IRUSR|S_IWUSR );
  if (fid==-1)
  {
    printf("Error number: %i",errno);
    exit(EXIT_FAILURE);
  }
}

void pipe_put(char* pipe, char* message)
{
  int fd=open(pipe,O_WRONLY);
  write(fd,message,strlen(message)+1);
  close(fd);    
}

int pipe_read(char* pipe, char* reader, int size)
{
  int fd=open(pipe,O_RDONLY);
  int tmp = read(fd,reader,size);
  close(fd);
  return tmp;
}

void manageEvent(char* s, int size)
{
  printf("\nGyerek vagyok, utazok a megkapott helyszinre: %s\n", s);
  char checklist[100][size];
  int i = 0,finished = 0;
  
  printf("Gyerek vagyok, kuldom a jelzest, hogy megerkeztem es varom a resztvevoket.\n");
  kill(getppid(),SIGUSR1);

  while(finished == 0)
  {
    pipe_read("pipe",s,size);
    if(strlen(s) != 0)
    {
      strcpy(checklist[i++], s);
      kill(getppid(),SIGUSR1);
    }
    else 
    {
      finished = 1;
    }
  }

  if (i>0)
  {
    printf("\nGyerek vagyok, megkaptam a resztvevoket:\n");
    int j;
    for (j = 0; j < i; ++j)
    {
      printf("%s\n", checklist[j]);
    }

    time_t t;
    srand((unsigned) time(&t));
    char result[4];
    sprintf(result,"%d", rand() % 100);

    sigset_t sigset;
    sigfillset(&sigset);
    sigdelset(&sigset,SIGUSR1);

    printf("\nGyerek vagyok, %s szazalekos sikere volt a rendezvénynek.\n", result);
    
    pipe_put("pipe",result);
    // sleep(1);
    for (j = 0; j < i; ++j)
    {
      if (rand() % 100 <= 10)
      {
        printf("Gyerek vagyok, %s - %s\n", checklist[j], "Nem jelent meg");
        pipe_put("pipe",checklist[j]);
      }
      else
      {
        printf("Gyerek vagyok, %s - %s\n", checklist[j], "Megjelent");
      }
    }
    pipe_put("pipe","");
    kill(getppid(),SIGUSR1);
  }
  else
  {
    printf("%s\n%s\n","Gyerek vagyok, a lista szerint erre a rendezvényre nem jelentkeztek.",
                      "Gyerek vagyok, miert jottem ide feleslegesen?");
  }
}

void holdEvents(struct Guests* guestList , struct Events* eventList, int* guestListSize, int* eventListSize)
{
  make_pipe("pipe");
  printf("A kezelo keszen all az esemeny lebonyolitasara, gyerek inditasa!\n");

  struct sigaction sigact;
  sigact.sa_handler=handler;
  sigemptyset(&sigact.sa_mask);
  sigact.sa_flags=0;
  sigaction(SIGUSR1,&sigact,NULL);

  signal(SIGUSR1,handler);
  pid_t pid = fork();

  if(pid>0)   //parent 
  {
    int e;
    for (e = 0; e < *eventListSize; ++e)
    {
      sigset_t sigset;
      sigfillset(&sigset);
      sigdelset(&sigset,SIGUSR1);
      sigsuspend(&sigset);
      // pause();
      printf("\n%s\n","Szulo: Kuldom a kovetkezo helyszint es varok a gyerek jelzesere!");
      pipe_put("pipe",eventList[e].location);

      sigsuspend(&sigset);
      // pause();
      printf("\nSzulo: Megkaptam a jelzest, kuldom a resztvevoket!\n");
      int i;
      for (i = 0; i < *guestListSize; ++i)
      {
        if (guestList[i].eventId == eventList[e].id)
        {
          pipe_put("pipe",guestList[i].name);
          sigsuspend(&sigset);
          // pause();
        }
      }
      pipe_put("pipe","");
      
      printf("Szulo: Varom az eredmenyt az esemenyrol\n");

      char r[16];
      while(pipe_read("pipe",r, sizeof(r)) == 0){;}
      printf("\nSzulo: Megkaptam az eredmenyt az esemenyrol: %s szazalek\n", r);

      int endofresult = 0;
      while(endofresult == 0){
        pipe_read("pipe",r, sizeof(r));
        if (strlen(r) != 0)
        {
          printf("\nSzulo: A jelentes szerint %s nem jelent meg\n", r);
        }
        else
        {
          endofresult = 1;
        }
      }
    }

    printf("\nSzulo: Nincs uj helyszin, ertesitem a gyereket errol!\n");
    pipe_put("pipe","");
    unlink("pipe");
    int status;
    wait(&status);
  }
  else // child
  {
    int hasnewevent = 1;
    while (hasnewevent == 1)
    {
      char s[100]="Semmi"; 

      printf("\nGyerek vagyok, varok a helyszinre!\n");
      kill(getppid(),SIGUSR1);
      
      pipe_read("pipe",s,sizeof(s));

      if (strlen(s) != 0)
      { 
        manageEvent(s,sizeof(s));
      }
      else
      {
        printf("\nGyerek vagyok, ugy ertesultem, hogy nincs uj helyszin!\n");
        hasnewevent = 0;
      }
    }
    exit(EXIT_SUCCESS);
  } 
}

// --------------------------- //
// ---------- Main ----------- //
// --------------------------- //

int main()
{
  int guestListSize = 0, eventListSize = 0;
  struct Guests* guestList = malloc(100 * sizeof(struct Guests));
  struct Events* eventList = malloc(100 * sizeof(struct Events));

  initLists(guestList, eventList, &guestListSize, &eventListSize);
  char input;
  while(input != 'q')
  {
    const char * menu[10];
      menu[0] = "To add a guest:\t\t\t\tpress 1";
      menu[1] = "To list all guests:\t\t\tpress 2";
      menu[2] = "To modify a guest's data:\t\tpress 3";
      menu[3] = "To remove a guest:\t\t\tpress 4";
      menu[4] = "To add a new event:\t\t\tpress 5";
      menu[5] = "To list all event:\t\t\tpress 6";
      menu[6] = "To remove an existing event:\t\tpress 7";
      menu[7] = "To save current data to local storage:\tpress 8";
      menu[8] = "To hold the existing events:\t\tpress 9";
      menu[9] = "To quit the application:\t\tpress q";
    printf("\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n\n\n\n\n\n\n", 
      menu[0], menu[1], menu[2], menu[3], menu[4], menu[5], menu[6], menu[7], menu[8], menu[9]);
    scanf(" %c",&input);
    printf("\n");
    switch(input)
    {
      case '1': printf("\n%d. guest added.\n",(addGuest(guestList, &guestListSize)));break;
      case '2': listGuests(guestList, &guestListSize);break;
      case '3': modifyGuest(guestList, &guestListSize);break;
      case '4': removeGuest(guestList, &guestListSize);break;
      case '5': addEvent(eventList, &eventListSize);break;
      case '6': listEvents(eventList, &eventListSize);break;
      case '7': removeEvent(eventList, &eventListSize);break;
      case '8': saveAll(guestList, eventList, &guestListSize, &eventListSize);break;
      case '9': holdEvents(guestList, eventList, &guestListSize, &eventListSize);break;
      case 'q': break;
      default: printf("Wrong input!\n");
    }
  }
  return 0;
}