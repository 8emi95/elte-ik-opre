#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Event.h"

char* find_event(char* word ,char *file_name);

void new_event(char* name , int id)
{
	char * result = NULL;
    asprintf(&result, "%d,%s", id, name);
	
	if(find_word_bool(result, "events.txt")) {
		printf("%s alread exists with %d event \n", name, id);
		return;
	}
	
	FILE *events;
	events = fopen("events.txt","a");
	
	fputs(result, events);
	fputs(",0 \n", events);
	
	fclose(events);
}
void print_all_events()
{
	FILE *file;
	file = fopen("events.txt", "r");
	char* line;
	line = (char *)malloc(1024);
	while (fgets(line , 1000 , file )!= NULL)
   {
        printf("%s",line);	
   }
   fclose(file);
}
void delete_event(char* name)
{
	
	char* delete_line = find_event(name, "events.txt");
	if (delete_line == NULL) {
		printf("the event doesn't exists \n");
		return;
	}
	char line [1024];
	
	FILE *outFile = fopen("tmp.txt", "w+");
	FILE *inFile = fopen("events.txt", "r");
	while ( fgets(line, sizeof(line), inFile))
	{			
		if (strlen(line) >= 2)
		{
			if (strstr(line, delete_line) == NULL) {
				fprintf(outFile, "%s", line);
			}
		}
	}
	fclose(inFile); 
	fclose(outFile);
	
	rename("tmp.txt", "events.txt");
}
char* find_event(char* word ,char *file_name)
{
	FILE *file;
	file = fopen(file_name, "r");
	
   char* line ;
   line = (char *)malloc(1024);
   while (fgets(line , 100 , file )!= NULL)
   {
      if (strstr(line , word )!= NULL)
      {
        //printf("%s \n",line);
		return line;
      }
   }
   fclose(file);
   return 0;
}