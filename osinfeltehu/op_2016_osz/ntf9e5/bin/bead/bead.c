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


	static struct person people[50];

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
    fprintf(fp, "\n%s %s %s", name, mail, id);
    //fprintf(fp, "\n");

    fclose(fp);


}

void modifyPerson(char id[50]){
    // paraméterül kapott embert megkeresi a fileban, törli és módosíthat a felhasználó

    /* letrehozunk egy tombot structbol, filebol beolvasunk es spacek menten tordelunk
        a tort reszeket taroljuk a struct tombben, majd ebben keressuk a megfelelo nevet
    */

    FILE *fp;
    char name[50];
    char email[50];
    char eventID[50];


    char line[160];
    char * result;
    char * token;
    fp = fopen(id, "r");
    int i;
    int ch=0;
    int l = 0;

   /* people[0].name = "ASD";
    char * sd = people[0].name;
    printf(sd);
*/    if (fp == 0){
        printf("File not found\n");
    } else {
        i=0;
        while (!feof(fp)){

        /* ugy fogunk modositani, hogy olvasunk ki filebol es egy temp fileba irjuk bele az adatokat
           ha eljutunk a keresett szemelyhez az o modositott adatait irjuk a temp fileba
           ha vegeztunk a temp file irasaval akkor visszamasoljuk az eredeti nevu fileba
           ha modositani szeretnenk az eventet akkor mashogy kell megkozeliteni,
           elosor megkerdezzuk a filet amiben van, majd utana hogy modositani kell-e a destinationt
           ha igen akkor nem masolunk csak keresunk es ha elertuk a celt akkor irjuk at es toroljuk majd az ujhoz irjuk
           torlesnel ugyanez csak nem irjuk a modositottat a temp fileba */
        fscanf(fp, "%s %s %s\n", name, email, eventID );
        people[i].name = name;
        people[i].email = email;
        people[i].eventID = eventID;
        if (strcmp(people[i].name, "asd") == 0){
            printf("KIIR: %s %s %i\n",people[i].name, people[i].email, i);
        }
        ++i;

        /*

        result = fgets(line, sizeof(line), fp);
        printf("RESULT: %s\n", result);
        token = strtok(result, " ");
        while (token != NULL){
            people[0].name = token;
            printf("TOKEN: %s\n", token);
            token = strtok(NULL, " ");
            people[0].email = token;
            people[0].eventID = token;
        }            /*if (strcmp(line,name)){
                printf(line);
            }*/
            //fgets(people[0].name , sizeof(people[0].name), fp);
       //     printf("%s", people[0].name);
//            result = strtok(line, " ");
  //          printf("DONE\n");
            //++i;
        }
    /*    while (result != 0){
            printf ("%s\n",result);
            result = strtok (0, " ,.-");
        }
    }*//*
    while (ch != EOF){
        ch = fgetc(fp);
        if (ch == '\n'){
            ++l;
        }
    }*/
}/*
for (i = 0; i < 3; ++i){

    printf("KIIR FOR: %s %s %i\n", people[i].name, people[i].email, i);
}
printf("%s %s %s\n", name, email, eventID);
printf("%s %s %i\n",people[0].name, people[0].eventID, i);*/
 //   printf("PEOPLE: %s", people[0].name);
    fclose(fp);
	/*char name[50];
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
    fclose(fp);*/
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
        printf("%s", line);
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
		printf(" 1) Create new Event\n 2) Add person to Event\n 3) Delete Event\n 4) Quit\n 5) List Event guests\n 6) Modify person data\n");
        scanf("%i", &i);
	}

	return 0;
}
