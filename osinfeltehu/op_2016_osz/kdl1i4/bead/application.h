#ifndef _APPLICATION_H_
#define _APPLICATION_H_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAXID 32
#define MAXSTRING 128
#define MAXLINE 256

void listApplicants(){
	char line[MAXLINE];
    int i = 1;
	
	if(applicantCount() == 0){
		printf("Jelenleg meg nincs jelentkezett vendeg!\n");
		return;
		
	}
	
    FILE *inputf = fopen("applicants.txt","r");
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

void newApplication(){
    char name[MAXSTRING];
    char email[MAXSTRING];
    char id[MAXID];
	
	if(eventCount() == 0){
		printf("Jelenleg nincs hozzaadott rendezveny, elotte adjon hozza rendezvenyt!\n");
		return;
	}
	
    FILE *inputf = fopen("applicants.txt", "a");
    if (inputf == NULL)
    {
        printf("Hiba a fajl megnyitasakor/letrehozasakor!\n");
        return;
    }
    
    printf("Adja meg a nevet!\n");
    fgets (name, MAXSTRING, stdin);
    strtok(name, "\n");
    
    printf("Adja meg az e-mail cimet!\n");
    fgets (email, MAXSTRING, stdin);
    strtok(email, "\n");
    
    printf("Adja meg a rendezveny azonositojat amelyre fel kivan jelentkezni!\n");
	fgets (id, MAXID, stdin);
	strtok(id, "\n");
	
	while(eventExists(id) != 1){
		printf("A megadott azonositohoz nem tartozik rendezveny! Kerem adja meg ujra!\n");
		fgets (id, MAXID, stdin);
		strtok(id, "\n");
	}
	
    time_t current = time(0);
    
    fprintf(inputf, "%s\t%s\t%s\t%s", name, email, id, ctime(&current));
	printf("On a %d. jelentkezo!", applicantCount() + 1);
	
	fclose(inputf);
}


void editApplication(){
    char line[MAXLINE];
    char name[MAXSTRING];
    char email[MAXSTRING];
    char id[MAXID];
    char date[MAXSTRING];
    int i = 1;
	if(applicantCount() == 0){
		printf("Jelenleg nincs feljelentkezett vendeg!\n");
		return;
	}
	int num = 1;
    listApplicants();
    printf("Hanyadik sorszamu vendeget szeretne modositani?\n");
    scanf(" %d", &num);
	
	while(num <= 0 || num > applicantCount()){
		printf("A megadott sorszamhoz nem tartozik vendeg!\n");
		scanf(" %d", &num);
	}
	
    FILE *inputf  = fopen("applicants.txt", "r");
    if (inputf == NULL)
    {
        printf("Jelenleg nincsen feljelentkezett vendeg\n");
        return;
    }
    
    FILE *outputf = fopen("newapp.txt", "w");
    
    while (fgets(line, sizeof(line), inputf))
    {
        
        if (i == num)
        {
            sscanf(line, "%[^\t]\t%s\t%s\t%[^\n]", name, email, id, date);
            int menu = 0;
            while(menu != '4')
            {
                printf("Adja meg a modositani kivant adat sorszamat!");
                printf("1. Nev");
                printf("2. Email");
                printf("3. Rendezveny azonosito");
                printf("4. Kesz\n");
                scanf(" %c%*c", &menu);
                switch(menu)
                {
                case '1':
                    printf("Adja meg az uj nev:");
                    fgets (name, MAXSTRING, stdin);
                    strtok(name, "\n");
                    break;
                case '2':
                    printf("Adja meg az uj e-mail cimet:");
                    fgets (email, MAXSTRING, stdin);
                    strtok(email, "\n");
                    break;
                case '3':               
					while(eventExists(id) != 1){
						printf("Adjon meg egy letezo rendezveny azonositot!\n");
						fgets (id, MAXID, stdin);
						strtok(id, "\n");
					}
                    break;
                case '4':
                  
                    break;
                default:
                    printf("A kivaslasztott opcio nem letezik, kerem valassza ki ujra!\n");
                }
            }
            
            fprintf(outputf, "%s\t%s\t%s\t%s\n", name, email, id, date);
            
        }
        else{
            fputs(line, outputf);   
        }
        i++;    
    }

    fclose(outputf);
    fclose(inputf);
    
    remove("applicants.txt");
    rename("newapp.txt", "applicants.txt");
    
    
}
void deleteApplication(){
    char line[MAXLINE];
    int i =1;
	
	if(applicantCount() == 0){
		printf("Jelenleg nincs feljelentkezett vendeg!\n");
		return;
	}
    int num = 1;
    listApplicants();
	
    printf("Hanyadik vendeget szeretne torolni?\n");
    scanf(" %d", &num);
	while(num <= 0 || num > applicantCount()){
		printf("A megadott sorszamhoz nem tartozik vendeg! Adja meg ujra!\n");
		scanf(" %d", &num);
		
	}
	
    FILE *inputf  = fopen("applicants.txt", "r");
    if (inputf == NULL)
    {
        printf("No applications\n");
        return;
    }
    
    FILE *outputf = fopen("newapp.txt", "w");
    
    while (fgets(line, sizeof(line), inputf))
    {
        
        if (i != num)
        {
			fputs(line, outputf);  
        }
        i++;    
    }

    fclose(outputf);
    fclose(inputf);
    
    remove("applicants.txt");
    rename("newapp.txt", "applicants.txt");
    
}



int applicantCount(){
	FILE *inputf = fopen("applicants.txt","r");
    int lastChar = 0;    
    int applicantCount = 0;
    if (inputf != NULL)
    {
		do
		{
			lastChar = fgetc(inputf);
			if(lastChar == '\n') applicantCount++;   
		} while( lastChar != EOF );    
        fclose(inputf);        
    }
	
	return applicantCount;
	
}


#endif