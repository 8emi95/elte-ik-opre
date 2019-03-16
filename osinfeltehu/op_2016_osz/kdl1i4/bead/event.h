#ifndef _EVENT_H_
#define _EVENT_H_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAXID 32
#define MAXSTRING 128
#define MAXLINE 256

 void newEvent(){
    char id[MAXID];
    FILE *inputf = fopen("events.txt", "a");
	
    if (inputf == NULL)
    {
        printf("Hiba tortent a fajl letrehozasakor/megnyitasakor!\n");
        return;
    }
	
    printf("Kerem adja meg a rendezveny azonositojat!\n");
	
	fgets (id, MAXID, stdin);
    strtok(id, "\n");
	
	while(eventExists(id) == 1){
		printf("A megadott azonositoval mar letezik rendezveny! Kerem adjon meg masik azonositot!\n");
		fgets (id, MAXID, stdin);
		strtok(id, "\n");
	}
	char hely[MAXSTRING];
	printf("\nKerem adja meg a rendezveny helyszinet!\n");
	
	fgets (hely, MAXSTRING, stdin);
    strtok(hely, "\n");
    
    
    fprintf(inputf, "%s\t%s\n",id,hely);
	
    fclose(inputf);
}

void listEvents(){
    char line[MAXLINE];
    int i = 1;
	
    FILE *inputf = fopen("events.txt","r");
    if (inputf == NULL)
    {
        printf("Hiba a fajl olvasasakor!\n");
        return;
    }

    while(fgets(line, sizeof(line), inputf) != NULL){
        printf("%d. %s", i, line);  
        i++;    
    }

    fclose(inputf);
}

void deleteEvent(){
    char line[MAXLINE];
    char name[MAXSTRING];
    char email[MAXSTRING];
    char id[MAXID];
	char eventId[MAXID];
    char date[MAXSTRING];
	char hely[MAXSTRING];
	
    int i = 1;
	int num;
	
	if(eventCount() == 0){
		printf("Jelenleg meg nincs letrehozott esemeny!\n");
		return;
	}
	listEvents();
    printf("Hanyadik esemenyt szeretne torolni?\n");
    scanf(" %d", &num);
	while(num <= 0 || num > eventCount()){
		printf("Adj meg egy ervenyes sorszamot\n");
		scanf(" %d", &num);
		
	}
	FILE *inputapp  = fopen("applicants.txt", "r");
    FILE *inputeve  = fopen("events.txt", "r");
	
    if (inputeve == NULL)
    {
        printf("Jelenleg nincs letrehozott esemeny!\n");
        return;
    }
    
    FILE *outputapp = fopen("newapp.txt", "w");
	FILE *outputeve = fopen("neweve.txt", "w");
    
	while (fgets(line, sizeof(line), inputeve))
    {
        
        if (i == num)
        {
            sscanf(line, "%[^\t]\t%[^\n]", eventId, hely); 
        }else{
            fputs(line, outputeve);   
        }
        i++;    
    }
		
    while (fgets(line, sizeof(line), inputapp))
    {
        
        sscanf(line, "%[^\t]\t%s\t%s\t%[^\n]", name, email, id, date);
		if (strcmp(id,eventId) != 0)
        {
            fputs(line, outputapp); 
		}
    }
	
	fclose(outputeve);
    fclose(outputapp);
	fclose(inputeve);
    fclose(inputapp);
    
    remove("applicants.txt");
	remove("events.txt");
    rename("newapp.txt", "applicants.txt");
	rename("neweve.txt", "events.txt");
	
	
}

int eventCount(){
	FILE *inputf = fopen("events.txt","r");
    int lastChar = 0;    
    int eventCount = 0;
    if (inputf != NULL)
    {
		do
		{
			lastChar = fgetc(inputf);
			if(lastChar == '\n') eventCount++;   
		} while( lastChar != EOF );    
        fclose(inputf);        
    }
	
	return eventCount;
}

int eventExists(char *event){
	char line[MAXLINE];
	char eventId[MAXID];
	char hely[MAXSTRING];
	
	FILE *inputf  = fopen("events.txt", "r");
	if (inputf != NULL)
	{
		while (fgets(line, sizeof(line), inputf))
		{  
			sscanf(line, "%[^\t]\t%[^\n]", eventId, hely); 
			if (strcmp(event, eventId) == 0){
				return 1;
			}
		}
	}
	return 0;
}

int copyApplicantsWithId(char tomb[][100], char* eventId){
	char line[MAXLINE];
	char name[MAXSTRING];
    char email[MAXSTRING];
    char id[MAXID];
    char date[MAXSTRING];
    int i = 0;
  
    FILE *inputf = fopen("applicants.txt","r");
    if (inputf == NULL)
    {
        printf("Hiba a file olvasasakor!\n");
        return;
    }

    while(fgets(line, sizeof(line), inputf) != NULL){
        sscanf(line, "%[^\t]\t%s\t%s\t%[^\n]", name, email, id, date);
        if(strcmp(id, eventId) == 0){
            strcpy(tomb[i], name);
            i++; 
        }
    }

    fclose(inputf);
	
    return i; 
}

#endif