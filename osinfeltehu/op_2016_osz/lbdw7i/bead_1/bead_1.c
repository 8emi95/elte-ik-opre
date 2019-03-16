#include <stdio.h>
#include <time.h>
#include "string.h"
#include <string.h>
#include <stdlib.h>  /* For exit() function */

int menu();     //function prototypes
int apply();
void listMembers();
void changeMemberData();
void deleteMemberData();
void createNewEvent();
void deleteEvent();
void quit();

int countLines(char* eventIdentifier);
void writeToFile(char* name, char* email, char* eventIdentifier);

int main()
{
    int choice;     // main variables
    char user_filename[100];

    choice = menu();   // get user's first selection

    while(choice != 0)   //execute so long as choice is not equal to QUIT
    {
        switch(choice)
            {
                case 1: //
                    printf("Registered number: %i. \n",apply());
                    break;
                case 2: listMembers();
                    break;
                case 3: changeMemberData();
                    break;
                case 4: deleteMemberData();
                    break;
                case 5: createNewEvent();//
                    break;
                case 6: deleteEvent();//
                    break;
                case 0: 
                    break;
                default:    printf("Invalid choice. ");
                            printf("Please try again.\n");
            }
      choice = menu(); /* get user's subsequent selections */
   }
   return 0;
}

int menu(void)
{
    int option;

    printf("King of Stands\n\n");
    printf("1.\tApply\n");
    printf("2.\tList applied members\n");
    printf("3.\tChange data of member\n");
    printf("4.\tDelete data of member\n");
    printf("5.\tCreate new event\n");
    printf("6.\tDelete event\n");
    printf("0.\tQuit\n\n");
    printf("Select something: ");

    scanf(" %i", &option);

    //while( (scanf(" %d", &option) != 1) /* non-numeric input */
    //     || (option < 0)               /* number too small */
    //      || (option > 6))              /* number too large */
    //{
    //  fflush(stdin);                    /* clear bad data from buffer */
    //  printf("That selection invalid. Please try again.\n");
    //}
    return option;
}

int apply()
{
    char name[100];
    char email[100];
    char eventIdentifier[100];

    printf("Name:\t");
    scanf("%s", &name);

    printf("E-mail:\t");
    scanf("%s", &email);

    printf("Event identifier:\t");
    scanf("%s", &eventIdentifier);

    int regnumber = countLines(eventIdentifier); //return 0 if error
    writeToFile(name, email, eventIdentifier);

    return ((regnumber + 1) / 3);
}

void listMembers(){
    char eventIdentifier[100];
    printf("Add event identifier:\t");
    scanf("%s", &eventIdentifier);

    printf("Members: \n");
    char line[200];
    FILE *fp;

    if ((fp = fopen(eventIdentifier, "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);         
    }

    int i=0;
    while (fgets(line, sizeof(line), fp)) {// stop when new line, or reached the size limit
        ++i;
        if(i % 3 == 1){
            int number = (i/3)+1;
            printf("%i. member: ", number );    
        }
        printf("%s", line); 
    }

    fclose(fp);
}

void changeMemberData(){
    char nameFrom[100];
    char nameTo[100];
    char emailTo[100];
    char eventIdentifier[100];

    printf("Select event identifier you would like to change:\t");
    scanf("%s", &eventIdentifier);
    printf("Select name you would like to change:\t");
    scanf("%s", &nameFrom);
    printf("New name:\t");
    scanf("%s", &nameTo);
    printf("New e-mail:\t");
    scanf("%s", &emailTo);


    FILE *fp1 = fopen(eventIdentifier, "r");
    FILE *fp2 = fopen("changed", "w");
    if(fp1 == NULL)
    {
        printf("Error!");
        exit(1);
    }

    char line[100];
    strcat(nameFrom, "\n");
    while (fgets(line, sizeof(line), fp1)) {
        if(strcmp(line, nameFrom) == 0){
            fprintf(fp2, "%s\n", nameTo);
            fprintf(fp2, "%s\n", emailTo);
            
            time_t rawtime;
            struct tm * timeinfo;
            time ( &rawtime );
            timeinfo = localtime ( &rawtime );
            fprintf(fp2, "%s", asctime (timeinfo));

            //skip next 2 line (old mail, old time)
            fgets(line, sizeof(line), fp1);
            fgets(line, sizeof(line), fp1);
        }else{
            fprintf(fp2, "%s", line);
        }
    }

    fclose(fp1);
    fclose(fp2);
    remove(eventIdentifier);
    rename("changed",eventIdentifier);
}

void deleteMemberData(){
    char nameFrom[100];
    char eventIdentifier[100];

    printf("Select event identifier you would like to change:\t");
    scanf("%s", &eventIdentifier);
    printf("Select name you would like to delete:\t");
    scanf("%s", &nameFrom);

    FILE *fp1 = fopen(eventIdentifier, "r");
    FILE *fp2 = fopen("changed", "w");
    if(fp1 == NULL)
    {
        printf("Error!");
        exit(1);
    }

    char line[100];
    strcat(nameFrom, "\n");
    while (fgets(line, sizeof(line), fp1)) {
        if(strcmp(line, nameFrom) == 0){
            //skip next 2 line too (old mail, old time)
            fgets(line, sizeof(line), fp1);
            fgets(line, sizeof(line), fp1);
        }else{
            fprintf(fp2, "%s", line);
        }
    }

    fclose(fp1);
    fclose(fp2);
    remove(eventIdentifier);
    rename("changed",eventIdentifier);
}

void createNewEvent(){
    char eventIdentifier[100];

    printf("eventIdentifier: ");
    scanf("%s", &eventIdentifier);

    FILE *fp = fopen(eventIdentifier, "a+");
    fclose(fp);
}

void deleteEvent(){
    char eventIdentifier[100];

    printf("eventIdentifier: ");
    scanf("%s", &eventIdentifier);
    
    if( remove(eventIdentifier) == 0 ) {
        printf("File deleted successfully");
    }
    else {
        printf("Error: unable to delete the file");
    }
}

int countLines(char* eventIdentifier){
     /* file handle --- in this case I am parsing this source code */
    FILE *fp = fopen(eventIdentifier, "r");

    /* a holder for each character */
    char c;
    int lines=1;

    /* for as long as we can get characters... */
    while((c=fgetc(fp))) {

        /* break if end of file */
        if(c == EOF) break;

        if( c == '\n' ){
            lines++;
        }
    }
    fclose(fp);

    return lines;
}

void writeToFile(char* name, char* email, char* eventIdentifier){
    FILE *fp = fopen(eventIdentifier, "a");
    if(fp == NULL)
    {
        printf("Error!");
        exit(1);
    }else{
        time_t rawtime;
        struct tm * timeinfo;

        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        printf ( "Current local time and date: %s", asctime (timeinfo) );

        fprintf(fp, "%s\n%s\n%s", name, email, asctime (timeinfo));
    }

    fclose(fp);
}