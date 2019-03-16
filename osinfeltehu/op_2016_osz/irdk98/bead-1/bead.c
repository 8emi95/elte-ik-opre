#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h> //open,create
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define LINELENGTH 100


char** explode(char* buffer) {
	char ** result = NULL;
	char * pch;
    int numberOfParts = 0;

  	// printf ("Splitting string \"%s\" into tokens:\n", buffer);

  	pch = strtok(buffer, " ");

  	while (pch != NULL) {
        result = (char **) realloc(result, sizeof (char*) * ++numberOfParts); // Az Isten megáldja a compilert hogy ennyire érzékeny a pointer castolásokra!
    	if (NULL == result) {
            printf("Nem sikerült elég memóriát allokálni. A progam bezár. \n");
            exit(-1);
        }

        result[numberOfParts-1] = pch;

        pch = strtok(NULL, " ");
  	}
	
	return result;
} 

void addPerson(const char* fname, char* name, char* email, char* eventName) {
	// name email eventName systime
   
    char buffer[LINELENGTH];

    FILE* f = fopen(fname, "r+");
    char** explodedLine;
    int nthNum = 0;

    while (true) {
       if (fgets(buffer, LINELENGTH, f) == NULL) break;
       // puts(buffer);
       explodedLine = explode(buffer);
       ++nthNum;

       if(0 == strcmp(explodedLine[0], name) && 0 == strcmp(explodedLine[1], email)) {
            // Akkor már egyszer benne van. Szólunk, majd terminálás.
            printf("%s %s\n", explodedLine[0], explodedLine[1]);
            printf("Már egyszer hozzáadta ezt a felhasználót! \n");
            exit(-1);
       }
    }

    // Ha ide eljutunk, mehet a hozzáadás.
    fprintf(f, "%s %s %s %d\n", name, email, eventName, (unsigned long)time(NULL)); 
    /* Ezzel a könyvtár / file struktúrával igaziból el sem kellene tárolnom az eventName-t. */

    printf("Sikeres jelentkezés az eventre %i azonosítóval.\n", nthNum);     

    fclose(f);
}

void editPerson(char* fname, char* name, char* email, char* newName, char* newMail) {
    // name email eventName systime   
    char buffer[LINELENGTH];

    FILE* f = fopen(fname, "r+");
    FILE* tempFile = fopen("events/tempFile", "w");

    char** explodedLine;
    bool personExists = false;
    int nthNum = 0;

    while (true) {
       if (fgets(buffer, LINELENGTH, f) == NULL) break;
       // puts(buffer);
       buffer[strcspn(buffer, "\r\n")] = 0; // works for LF, CR, CRLF, LFCR, ...
       explodedLine = explode(buffer);
       ++nthNum;

       if(0 == strcmp(explodedLine[0], name) && 0 == strcmp(explodedLine[1], email)) {
            personExists = true;
            fprintf(tempFile, "%s %s %s %s\n", newName, newMail, explodedLine[2], explodedLine[3]);
       } else {
         // Amúgy meg írjuk kifelé a sort
         fprintf(tempFile, "%s %s %s %s\n", explodedLine[0], explodedLine[1], explodedLine[2], explodedLine[3]); 
       }
    }

    if(!personExists) {
        printf("Nem találtam meg a kért személyt. :( \n");
    } else {
        printf("A felhasználó adatai módosítva. \n");
        remove(fname);
        rename("events/tempFile", fname);
    }

    fclose(tempFile);
    fclose(f);
}

void removePerson(const char* fname, char* name, char* email) {
    // name email eventName systime   
    char buffer[LINELENGTH];

    FILE* f = fopen(fname, "r+");
    FILE* tempFile = fopen("events/tempFile", "w");

    char** explodedLine;
    bool personExists = false;
    int nthNum = 0;

    while (true) {
       if (fgets(buffer, LINELENGTH, f) == NULL) break;
       // puts(buffer);
       buffer[strcspn(buffer, "\r\n")] = 0; // works for LF, CR, CRLF, LFCR, ...
       explodedLine = explode(buffer);
       ++nthNum;

       if(0 == strcmp(explodedLine[0], name) && 0 == strcmp(explodedLine[1], email)) {
            personExists = true;
       } else {
         // Amúgy meg írjuk kifelé a sort
         fprintf(tempFile, "%s %s %s %s\n", explodedLine[0], explodedLine[1], explodedLine[2], explodedLine[3]); 
       }
    }

    if(!personExists) {
        printf("Nem találtam meg a kért személyt. :( \n");
    } else {
        printf("A felhasználó törölve. \n");
        remove(fname);
        rename("events/tempFile", fname);
    }

    fclose(tempFile);
    fclose(f);
}

int main(int argc,char** argv) {
	int actionCode = 0;
	char fullEventName[100];
	char folder[9] = "events/";
	char eventName[11]; // Including terminating 0
	bool fExsists = false;

	printf("Üdv! Muveletek: 1. rendezvenykezeles 2. felhasznalókezelés \n");
    scanf("%d", &actionCode);

	if(actionCode == 1) {
		printf("------------- \n");
		printf("Rendezvenykezeles \n");
		printf("1 - hozzaadas, 2- jelentkezettek listázása, 3 - torles \n");

		// Here we can modify our action code again. I don't feel like introducing new variables for this.
		scanf("%d", &actionCode);

		if(actionCode == 1) {
			printf("------------- \n");
			printf("Rendezvény hozzáadása \n");
			printf("Adja meg a rendezveny azonosítóját! (A nevét célszerű, max 10 char) \n");

			do { 
				memset(fullEventName,0,sizeof(fullEventName));

				scanf("%s", eventName);	
				
				strcat(fullEventName, folder);
				strcat(fullEventName, eventName);

				fExsists = access( fullEventName, F_OK ) != -1;	

				if(fExsists) {
					printf("A file már létezik, kérem adjon meg másik nevet! \n");
				}
			} while(fExsists);
			
			printf("A követező file létreozása: %s .... \n", fullEventName);

			FILE *fp;

			fp = fopen(fullEventName, "w+");
			// fprintf(fp, "This is testing for fprintf...\n");
			// fputs("This is testing for fputs...\n", fp);
			fclose(fp);

			printf("Esemeny letrehozva! Hivatkozhat az esemeny nevere (%s). \n", eventName);		
		} else if(actionCode == 2) {
			printf("------------- \n");
			printf("Jelentkezettek listázása: adja meg a rendeveny kódját! \n");

            do { 
                memset(fullEventName, 0, sizeof(fullEventName));
                scanf("%s", eventName);                
                strcat(fullEventName, folder);
                strcat(fullEventName, eventName);
                fExsists = access( fullEventName, F_OK ) != -1;
                if(!fExsists) {
                    printf("Ilyen nevű rendezvény nem létezik. \n");
                }
            } while(!fExsists);

            // Most pedig ki kell listáznunk a jelentkezetteket. Let's do this s#&t.

            char buffer[LINELENGTH];
            FILE* f = fopen(fullEventName, "r+");
            char** explodedLine;

            while (true) {
               if (fgets(buffer, LINELENGTH, f) == NULL) break;
               explodedLine = explode(buffer);
               printf("%s %s\n", explodedLine[0], explodedLine[1]); 
            }            

            fclose(f);

		} else {
			printf("------------- \n");
			printf("Rendezvény törlése \n");
			printf("Adja meg a rendezvény nevét! \n");

			do { 
				memset(fullEventName,0,sizeof(fullEventName));

				scanf("%s", eventName);	
				
				strcat(fullEventName, folder);
				strcat(fullEventName, eventName);

				fExsists = access( fullEventName, F_OK ) != -1;	

				if(!fExsists) {
					printf("Ilyen nevű rendezvény nem létezik. \n");
				}
			} while(!fExsists);

			if(0 == (int) remove(fullEventName)) {
				printf("Az event sikeresen el lett távolítva! \n");
			} else {
				printf("Hiba történt a törlés során. \n");
			}
		}
	} else {
		printf("------------- \n");
		printf("Felhasználókezelés \n");
		printf("1. hozzáadás 2. törlés 3. módosítás \n");
		
		scanf("%d", &actionCode);

		if(actionCode != 1 && actionCode != 2 && actionCode != 3) {
			printf("Nincs ilyen művelet. \n");
			exit(1);
		}

		printf("Először adja meg az esemény nevét, melyhez a felhasználó tarozik (vagy tartozni fog)! \n");

		do { 
			memset(fullEventName,0,sizeof(fullEventName));

			scanf("%s", eventName);	
				
			strcat(fullEventName, folder);
			strcat(fullEventName, eventName);

			fExsists = access( fullEventName, F_OK ) != -1;	

			if(!fExsists) {
				printf("Az esemény még nem létezik, kérem adjon meg másk eseményt! \n");
			}
		} while(!fExsists);

        char name[LINELENGTH / 2];
        char mail[LINELENGTH / 2];

        printf("Írja be a felhaszáló adatait ilyen formában: nev email (minden esetben szükséges) \n");
        scanf("%s %s", name, mail);

		if(actionCode == 1) {
            printf("Felhasználó hozzáadása.... \n");
            addPerson(fullEventName, name, mail, eventName);
        } else if(actionCode == 2) {
            printf("Felhasználó törlése.... \n");
            removePerson(fullEventName, name, mail);
        } else {
            // Akkor módosít
            char newName[LINELENGTH / 2];
            char newMail[LINELENGTH / 2];

            printf("Írja be az új adatokat (nev mail) \n");
            scanf("%s %s", newName, newMail);

            if (strlen(newName) < 1 || strlen(newMail) < 1) {
                printf("Nullától nagyobb hosszúságú stringeket kérek \n.");
                exit(-1);
            }

            editPerson(fullEventName, name, mail, newName, newMail);
        }
	}	
}