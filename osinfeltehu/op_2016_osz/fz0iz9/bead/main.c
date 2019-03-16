#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "vector.h"
#include "types.h"
#include "menu.h"


int main (int argc, char ** argv) {
    vector events; 
    int errno;
    int i;
    FILE* f;
    char line[160];

    Event* tempevent;
    Guest* tempguest;

    if (argc != 2) {
        errno=EINVAL;
        perror("A program hasznalata: ./kofs [adatallomany]\n");
        exit(1);
    }

    printf("----------------------------------\n"
           "KING OF STANDS -- rendezvenykezelo\n"
           "----------------------------------\n");

    events = vector_init();

    f = fopen(argv[1], "r");
    if (f == NULL) {printf("A file nem erheto el, letrehozasra kerul.\n\n");}
    else {

    char linetype;

    while(fgets(line, sizeof(line), f)) {
            sscanf(line, "%c *[^\n]", &linetype);
            switch (linetype) {
                case 'E':
                    tempevent = event_init();
                    sscanf(line,"%*c %s", tempevent->name);
                    vector_add(&events, tempevent);
                    break;
                case 'G':
                    tempguest = guest_init();
                    sscanf(line,"%*c %s %s %[^\t\n]", tempguest->name, tempguest->mail,
                            tempguest->entry_date);
                    vector_add(&tempevent->guests, tempguest);
                    break;
            }
        }
    fclose(f);
    }

    while(main_menu(&events) == MENU_CONTINUE);
    
    f = fopen(argv[1], "w");
    if (f == NULL) { perror("A file nem irhato.\n");exit(1);}
    int k,j;

    for (k=0; k<events.count; ++k) {
       tempevent = vector_get(&events, k);
       fprintf(f,"E %s\n", tempevent->name);
       for (j=0; j<tempevent->guests.count; ++j) {
           tempguest = vector_get(&tempevent->guests, j);
           fprintf(f, "G %s %s ", tempguest->name, tempguest->mail);
           fputs(tempguest->entry_date,f);
           fputs("\n", f);
       }
       vector_empty(&tempevent->guests);
    }
    vector_empty(&events);
    fclose(f);
}
