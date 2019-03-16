#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <time.h>
#include <string.h>

/*struct Person{
	char* name;
	char* email;
	char* eventID;
};*/

void addPerson(char id[50]){
    char name[50];
    char mail[50];
    int ch = 0;
    int l=0;

    FILE *fp;
    fp = fopen(id, "r");
    if (fp == 0){
        printf("File not found\n");
    } else {
       // printf("File found\n");
        printf("Please input a name: ");
       scanf("%s", &name);
       printf("Please input an email: ");
       scanf("%s", &mail);


    while (ch != EOF){
        ch = fgetc(fp);
        if (ch == '\n'){
            ++l;
        }
    }
    }

    printf("EZ: %i\n", l+1);

    fp = fopen(id, "a");
    fprintf(fp, "%s %s %s\n", name, mail, id);

    fclose(fp);


}

void modifyPerson(char id[50]){
    // paraméterül kapott embert megkeresi a fileban, törli és módosíthat a felhasználó
	char name[50];
	char line[160];
	printf("Add name: ");
	scanf("%s", name);
	FILE *fp;
    fp = fopen(id, "r");

    if (fp == 0){
        printf("File not found\n");
    } else {
        while (!feof(fp)){
        fgets(line, sizeof(line), fp);
            if (strcmp(line,name)){
                printf(line);
            }
        }
    }
    fclose(fp);
}


void deletePerson(/* filenév ahonnan törlünk */){
    // paraméterül kapott embert megkeresi a fileban és törlni
}

void listGuests(char id[50]){
	// paraméterül kapott fileból kiírja a neveket
	char name[50];
	char line[160];
	FILE *fp;
    fp = fopen(id, "r");
    if (fp == 0){
        printf("File not found\n");
    } else {
        while (!feof(fp)){
        fgets(line, sizeof(line), fp);
        printf("%s\n", line);
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

void createFileName(){

}
void proba(){
    char* name;
    char* mail;
    int id;

   printf("Please input a name: ");
   scanf("%s", &name);
   printf("Please input an email: ");
   scanf("%s", &mail);
   printf("Please input an id: ");
   scanf("%i", &id);
   printf("You entered: %s %s %i\n", &name, &mail, id);
}

int main(){
    char str[10];

    int i = 0;
	while (1){

        if (i == 1){
            printf("Enter new event code: ");
            scanf("%s", str);
            startNewEvent(str);
		} else if (i == 2) {
//		itt kérnénk be paramétereket, és úgy módosítaná a program?
            printf("Enter event code to add a person: ");
            scanf("%s", str);
            addPerson(str);
		} else if (i == 3) {
		    printf("Enter event code to delete: ");
            scanf("%s", str);
            deleteEventData(str);
		} else if (i == 4) {
            exit(0);
		} else if (i == 5) {
		    printf("List guests of event: ");
            scanf("%s", str);
            listGuests(str);
		} else if (i == 6) {
		    printf("Modify in event: ");
            scanf("%s", str);
            modifyPerson(str);
		}


		printf("Choose\n");
        scanf("%i", &i);
	}

	return 0;
}
