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
  char eventId[5];
  char registrationTime[26];
};

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
	    fprintf(f, "%s\r\n", ptr->eventId);
	    fprintf(f, "%s\r\n", ptr->registrationTime);
	    ptr++;
	  };
	  fclose(f);
  }

  printf("Guests saved\n");
}

void saveEvents(int * eventList, int* eventListSize)
{
  remove("events.txt");
  FILE* f=fopen("events.txt","w");
  if (f==NULL){perror("File opening error\n"); exit(1);}

  int* ptr = eventList;
  int* endPtr = eventList + *eventListSize;

  if(ptr == endPtr) 
    fprintf(f, "");
  else while ( ptr < endPtr ){
    fprintf(f, "%d\r\n", *ptr);
    ptr++;
  };


  fclose(f);
  printf("Events saved\n");
}

void saveAll(struct Guests* guestList , int* eventList, int* guestListSize, int* eventListSize)
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

void initLists(struct Guests* guestList , int* eventList, int* guestListSize, int* eventListSize)
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
			  strcpy(guestList[*guestListSize].name, line);
			  fgets(line,sizeof(line),f);
			  len = strlen(line);
				if (len > 0 && line[len-1] == '\n') line[len-2] = '\0';
			  strcpy(guestList[*guestListSize].email, line);
			  fgets(line,sizeof(line),f);
			  len = strlen(line);
				if (len > 0 && line[len-1] == '\n') line[len-2] = '\0';
			  strcpy(guestList[*guestListSize].eventId, line);
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
			  strtok(line, "\n");
			  eventList = realloc(eventList, ++(*eventListSize) * sizeof(int));
			  eventList[*eventListSize-1] = (int) strtol(line, NULL, 10);
		  }
		} 
		fclose(f);
	}

  printf("Lists has been initialized\n");
}

int addGuest(struct Guests* guestList, int* guestListSize)
{
  // if (guestList != NULL){
  //   guestList = malloc(++(*guestListSize) * sizeof(*guestList));
  //   printf("%s\n", "malloc");
  // }
  // else{
  //   struct Guests* tmpList = realloc(guestList, ++(*guestListSize) * sizeof(tmpList));
  //   printf("%s\n", "realloc");
  //   if (tmpList)
  //   {
  //     guestList = tmpList;
  //     printf("%s\n", guestList[0].name);
  //   }
  // }

  // int index = (*guestListSize);

  char name[20];
  getData("Name of the new guest:", name);
  strcpy(guestList[*guestListSize].name, name);

  char email[35];
  getData("Email of the new guest:", email);
  strcpy(guestList[*guestListSize].email, email);
  
  char eventId[4];
  getData("Event Id(max 4 number) wich the guest goes to:", eventId);
  strcpy(guestList[*guestListSize].eventId, eventId);

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

void listGuests(struct Guests* guestList, int* guestListSize)
{
  int index = 0;
  while(index < *guestListSize)
  {
    printf("%s - %s - %s - %s\n", 
    	guestList[index].name, 
			guestList[index].email, 
			guestList[index].eventId, 
			guestList[index].registrationTime);
    ++index;
  }
  printf("Guests are listed\n");
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
  strcpy(guestList[index].eventId, eventId);

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

void addEvent(int* eventList, int* eventListSize)
{
  eventList = realloc(eventList, ++(*eventListSize) * sizeof(int));

  char tmpValue[5];
  getData("Type the id(max 4 number) of the new event:", tmpValue);
  eventList[(*eventListSize)-1] = (int) strtol(tmpValue, NULL, 10);

  printf("Added an event\n");
  saveEvents(eventList, eventListSize);
}

void removeEvent(int* eventList, int* eventListSize)
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
  printf("%d\n", eventList[0]);

  printf("Removed an event\n");
  saveEvents(eventList, eventListSize);
}

int main()
{
  int guestListSize = 0, eventListSize = 0;
  struct Guests* guestList = malloc(100 * sizeof(struct Guests));
  int * eventList = malloc(eventListSize * sizeof(int));

  initLists(guestList, eventList, &guestListSize, &eventListSize);

  char input;
  while(input != 'q')
  {
    const char * menu[7];
      menu[0] = "To add a guest:\t\t\t\tpress 1";
      menu[1] = "To list all guests:\t\t\tpress 2";
      menu[2] = "To modify a guest's data:\t\tpress 3";
      menu[3] = "To remove a guest:\t\t\tpress 4";
      menu[4] = "To add a new event:\t\t\tpress 5";
      menu[5] = "To remove an existing event:\t\tpress 6";
      menu[6] = "To save current data to local storage:\tpress 7";
      menu[7] = "To quit the application:\t\tpress q";
    printf("\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n\n\n\n\n\n\n", 
      menu[0],menu[1],menu[2],menu[3],menu[4],menu[5],menu[6],menu[7]);
    scanf(" %c",&input);
    printf("\n");
    switch(input)
      {
        case '1': printf("\n%d. guest added.\n",(addGuest(guestList, &guestListSize)));break;
        case '2': listGuests(guestList, &guestListSize);break;
        case '3': modifyGuest(guestList, &guestListSize);break;
        case '4': removeGuest(guestList, &guestListSize);break;
        case '5': addEvent(eventList, &eventListSize);break;
        case '6': removeEvent(eventList, &eventListSize);break;
        case '7': saveAll(guestList, eventList, &guestListSize, &eventListSize);break;
        case 'q': break;
        default: printf("%s\n", "Wrong input!");
      }
  }
  return 0;
}