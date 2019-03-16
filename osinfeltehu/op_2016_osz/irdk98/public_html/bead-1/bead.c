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

int main(int argc,char** argv) {
	int actionCode = 0;

	printf("Udv! Muveletek: 1. rendezvenykezeles 2. felhasznalókezelés \n");
    scanf("%d", &actionCode);

	if(actionCode == 1) {
		printf("------------- \n");
		printf("Rendezvenykezeles \n");
		printf("1 - hozzaadas, 2- jelentkezettek listázása, 3 - torles \n");

		// Here we can modify our action code again. I don't feel like introducing new variables for this.
		scanf("%d", &actionCode);

		if(actionCode == 1) {
			int eventNameSize = 11;
			char eventName[11]; // Including terminating 0
			// char fullEventName[20];
			char * fullEventName;

			printf("------------- \n");
			printf("Rendezvény hozzáadása \n");
			printf("Adja meg a rendezveny azonosítóját! (A nevét célszerű, max 10 char) \n");

			// Check if file extists:
			bool fExsists = false;
			do {
				scanf("%s", eventName);	

				char folder[9] = "/events/";
				fullEventName = strcat(folder, eventName);

				printf("Full event name: %s", fullEventName);
				exit(0);

				fExsists = access( fullEventName, F_OK ) != -1;

				if(fExsists) {
					printf("A file már létezik, kérem adjon meg másik nevet!");
				}					
			} while(fExsists);
			
			printf("Creating file with name: ... %s", fullEventName);

			/*
		    FILE *fp;

			fp = fopen(strcat("/events/", eventName), "w+");
			fprintf(fp, "This is testing for fprintf...\n");
			fputs("This is testing for fputs...\n", fp);
			fclose(fp);
			*/

		} else if(actionCode == 2) {
			printf("------------- \n");
			printf("Jelentkezettek listázása: adja meg a rendeveny kódját! \n");
		} else {
			printf("------------- \n");
			printf("Rendezvény törlése \n");
		}
	} else {
		printf("------------- \n");
		printf("Felhasználókezelés \n");
		printf("1. Hozzadas 2. torles\n");
	}	
}