#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <time.h>
#include <string.h>

struct person{
	char* name;
	char* email;
	char* eventID;
};


void addPerson(char id[50]){
    char name[50];
    char mail[50];
    int ch = 0;
    int line_counter=0;

    FILE *fp;
    fp = fopen(id, "r");
    if (fp == 0){
        printf("File not found\n");
    } else {
       printf("Add a name for a person: ");
       scanf("%s", &name);
       printf("Add an e-mail for a person: ");
       scanf("%s", &mail);


    while (ch != EOF){
        ch = fgetc(fp);
        if (ch == '\n'){
            ++line_counter;
        }
    }
    }

    printf("Person added as: %i\n", line_counter+1);

    fp = fopen(id, "a");
    fprintf(fp, "%s %s %s\n", name, mail, id);


    fclose(fp);


}

void modifyPerson(char id[50]){
    // paraméterül kapott embert megkeresi a fileban, törli és módosíthat a felhasználó
    struct person people[50];

    FILE *fp;
    FILE *ftemp;

    char name[50];
    char email[50];
    char eventID[50];

    char mod_name[50];

    fp = fopen(id, "r");
    ftemp = fopen("./temp.txt", "w");

    int i;
    int ch=0;

    printf("Input a name to modify: ");
    scanf("%s", &mod_name);

    if (fp == 0){
        printf("File not found\n");
    } else {
        i=0;
        while (!feof(fp)){

        fscanf(fp, "%s %s %s\n", name, email, eventID);
        people[i].name = name;
        people[i].email = email;
        people[i].eventID = eventID;
        if (strcmp(people[i].name, mod_name) == 0){

            printf("Change name to: ");
            scanf("%s", &name);
            printf("Change e-mail to: ");
            scanf("%s", &email);

            fprintf(ftemp, "%s %s %s\n", name, email, people[i].eventID);
        } else{
            fprintf(ftemp, "%s %s %s\n", people[i].name, people[i].email, people[i].eventID);
        }
        ++i;

        }
    fclose(fp);
    fclose(ftemp);
    }

    ftemp = fopen("./temp.txt", "r");
    fp = fopen(id, "w");
    if (ftemp == 0){
        printf("File not found\n");
    } else {
        while (!feof(ftemp)){
        fscanf(ftemp, "%s %s %s\n", name, email, eventID);
        fprintf(fp, "%s %s %s\n", name, email, eventID);
        }
    }

    fclose(fp);
    fclose(ftemp);
}


void deletePerson(char id[50]){
    // paraméterül kapott embert megkeresi a fileban és törlni

     struct person people[50];

    FILE *fp;
    FILE *ftemp;

    char name[50];
    char email[50];
    char eventID[50];

    char mod_name[50];

    fp = fopen(id, "r");
    ftemp = fopen("./temp.txt", "w");

    int i;
    int ch=0;

    printf("Delete person with the name: ");
    scanf("%s", &mod_name);

    if (fp == 0){
        printf("File not found\n");
    } else {
        i=0;
        while (!feof(fp)){

        fscanf(fp, "%s %s %s\n", name, email, eventID);
        people[i].name = name;
        people[i].email = email;
        people[i].eventID = eventID;
        if (strcmp(people[i].name, mod_name) != 0){
            fprintf(ftemp, "%s %s %s\n", people[i].name, people[i].email, people[i].eventID);
        }
        ++i;

        }
    fclose(fp);
    fclose(ftemp);
    }
	
	remove(fp);
	
    ftemp = fopen("./temp.txt", "r");
    fp = fopen(id, "w");
    if (ftemp == 0){
        printf("File not found\n");
    } else {
        while (!feof(ftemp)){
        fscanf(ftemp, "%s %s %s\n", name, email, eventID);
        fprintf(fp, "%s %s %s\n", name, email, eventID);
        }
    }

    fclose(fp);
    fclose(ftemp);
}

void listGuests(char id[50]){
	// paraméterül kapott fileból kiírja a neveket
	char name[50];
	char email[50];
	char eventID[50];

	FILE *fp;
    fp = fopen(id, "r");
    if (fp == 0){
        printf("File not found\n");
    } else {
        while (!feof(fp)){
        fscanf(fp, "%s %s %s\n", name, email, eventID);
        printf("%s %s %s\n", name, email, eventID);
        }
    }
    fclose(fp);
}

void startNewEvent(char id[50]){
    FILE *fp;
    fp = fopen(id, "w");
    if (fp == 0){
        printf("File not created\n");
    } else {
        printf("File created!\n");
    }
    fclose(fp);

}

void deleteEventData(char id[50]){
    int ret;
    ret = remove(id);

   if(ret == 0)
   {
      printf("File deleted successfully\n");
   }
   else
   {
      printf("Error: unable to delete the file\n");
   }
}

int main(){
    char str[50];

    int i = 0;
	while (1){

        if (i == 1){
            printf("Create new event with code: ");
            scanf("%s", str);
            startNewEvent(str);
		printf("\n");
		} else if (i == 2) {
            printf("Enter event code to add a person: ");
            scanf("%s", str);
            addPerson(str);
		printf("\n");
		} else if (i == 3) {
		    printf("Enter event code to delete: ");
            scanf("%s", str);
            deleteEventData(str);
		printf("\n");
		} else if (i == 4) {
            exit(0);
		} else if (i == 5) {
		    printf("List guests of event: ");
            scanf("%s", str);
            listGuests(str);
		printf("\n");
		} else if (i == 6) {
		    printf("Modify person in event: ");
            scanf("%s", str);
            modifyPerson(str);
		printf("\n");
		} else if (i == 7){
            printf("Remove person from event: ");
            scanf("%s", str);
            deletePerson(str);
		printf("\n");
		}


		printf(" 1) Create new Event\n 2) Add person to Event\n 3) Delete Event\n 4) Quit\n 5) List Event guests\n 6) Modify person data\n 7) Remove guest\n");
        scanf("%i", &i);
	}

	return 0;
}
