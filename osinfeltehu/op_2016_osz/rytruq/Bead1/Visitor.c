#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Visitor.h"
#include "Event.h"

bool find_word_bool(char* word, char *file_name);
char* find_word(char* word ,char *file_name);

void print(char* name)
{
	char* result = find_word(name, "visitors.txt");
	printf(result);
}

void print_all()
{
	FILE *file;
	file = fopen("visitors.txt", "r");
	char* line;
	line = (char *)malloc(1024);
	while (fgets(line , 1000 , file )!= NULL)
   {
        printf("%s",line);	
   }
   fclose(file);
}
void print_event_members(int event_id)
{
	char word[100] ;
	snprintf(word, 100, "%d", event_id);
	char comma = ',';
	size_t len = strlen(word);
	char * str = malloc(len + 1 + 1 + 1);
	strncpy(str, word, 1);
	str[0] = comma;
	str[len] = comma;
	str[len+1] = '\0';
	
	char line [1024];
	FILE *inFile = fopen("visitors.txt", "r");
	
	while ( fgets(line, sizeof(line), inFile))
	{
		if (strstr(line , word) != NULL) {
			printf("%s", line);
		}
	}
	fclose(inFile); 
}

void add_visitor(char* name, char* e_mail, int event_id)
{
	
	char * result = NULL;
    asprintf(&result, "%s,%s,%d", name, e_mail, event_id);
	
	if(find_word_bool(result, "visitors.txt")) {
		printf("%s alread exists with %d event \n", name, event_id);
		return;
	}
	
	FILE *visitors;
	visitors = fopen("visitors.txt","a");
	fputs(result, visitors);
	fputs(",", visitors);
	
	time_t sign_date;
	sign_date = time(NULL);
	fputs(ctime(&sign_date), visitors);
	fputs("\n", visitors);
	
	fclose(visitors);
}

void delete_visitor(char* name)
{
	char* delete_line = find_word(name, "visitors.txt");
	if (delete_line == NULL) {
		printf("the visitor doesn't exists \n");
		return;
	}
	char line [1024];
	
	FILE *outFile = fopen("tmp.txt", "w+");
	FILE *inFile = fopen("visitors.txt", "r");
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
	
	rename("tmp.txt", "visitors.txt");
	
}

void delete_event_members(int id_event)
{
	char word[100] ;
	snprintf(word, 100, "%d", id_event);
	char comma = ',';
	size_t len = strlen(word);
	char * str = malloc(len + 1 + 1 + 1);
	strncpy(str, word, 1);
	str[0] = comma;
	str[len] = comma;
	str[len+1] = '\0';
	
	char line [1024];
	FILE *outFile = fopen("tmp.txt", "w+");
	FILE *inFile = fopen("visitors.txt", "r");
	
	while ( fgets(line, sizeof(line), inFile))
	{
		if (line != "") {
			printf("meleg");
			fprintf(outFile, "%s", line);
		}
	}
	fclose(inFile); 
	fclose(outFile);
	
	if(!rename("visitors.txt", "tmp.txt")) {
		printf("Rename Error");
		return;
	}
}

void modify_name(char* name, char* name_modify);
void modify_e_mail(char* name, char* modify_e_mail);
void modify_event_id(char* name , char* event_id_modify);

bool find_word_bool(char* word ,char *file_name)
{
	FILE *file;
	file = fopen(file_name, "r");
	
   char line[1024] ;
   while (fgets(line , sizeof(line) , file )!= NULL)
   {
      if (strstr(line , word )!= NULL)
      {
		  fclose(file);
		  return 1;
         //printf("%s \n",line);
      }
   }
   fclose(file);
   return 0;
}

char* find_word(char* word ,char *file_name)
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
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}