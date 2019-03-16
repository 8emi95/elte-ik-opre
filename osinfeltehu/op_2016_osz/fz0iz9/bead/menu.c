#include <string.h>
#include "menu.h"


int main_menu(vector *events) {
    int i;
    int status;
    int eventindex;
    char menuchar;
    Event * e;

    status = MENU_INVALID;

    printf("\n---------------------------------\n"
           "Meghirdetett rendezvenyek:\n");
    if (events->count>0) {
        for (i = 0; i<events->count; i++) {
            e = (Event*)vector_get(events, i);
            printf("%d. %s (%d vendeg)\n",i+1,e->name,e->guests.count);
        } 
    } else {
        printf("(nincs meg meghirdetett rendezveny)\n");
    }
    printf("---------------------------------\n"
           "1. Uj rendezveny\t2. Rendezveny torlese\n"
           "3. Vendegek hozzadasa/listazasa/adatmodositas/torles\n"
           "4. Kilepes es mentes\n");

    while (status == MENU_INVALID) {

        printf("> ");
        status = MENU_VALID;
        if(scanf(" %c", &menuchar))
            flush_stdin();
        switch (menuchar) {
            case '1':
                add_event(events);
                break;
            case '2':
                delete_event(events);
                break;
            case '3':
                eventindex = get_event_index(events);
                if (eventindex == -1)
                    break;
                while (guestlist_menu(vector_get(events,eventindex)) == MENU_CONTINUE);
                break;
            case '4':
                return MENU_EXIT;
                break;
            default:
                status = MENU_INVALID;
                break;
        }
    }
    return MENU_CONTINUE;

}

int guestlist_menu(Event* event) {
    int i;
    int status;
    char menuchar;
    Guest* g;

    status = MENU_INVALID;

    printf("\n\n---------------------------------\n"
            "%s - vendeglista: %d vendeg\n\n",event->name, event->guests.count);
    if (event->guests.count) {
        for (i=0; i<event->guests.count; i++) {
            g = vector_get(&event->guests, i);
            printf("%d. %s, %s, %s\n",
                    i+1,
                    g->name,
                    g->mail,
                    g->entry_date);
        }
     } else printf("(meg nincsenek vendegek)\n");

    printf("\n---------------------------------\n"
           "1. Vendeg hozzaadasa\n"
           "2. Vendeg szerkesztese\n"
           "3. Vendeg torlese\n"
           "4. Vissza\n");

    while (status == MENU_INVALID) {

        printf("\n> ");
        status = MENU_VALID;
        if(scanf(" %c", &menuchar))
            flush_stdin();

        switch (menuchar) {
            case '1':
                add_guest(event);
                break;
            case '2':
                edit_guest(event);
                break;
            case '3':
                delete_guest(event);
                break;
            case '4':
                return MENU_EXIT;
                break;
            default:
                status = MENU_INVALID;
                break;
        }
    }
    return MENU_CONTINUE;
}

static int get_event_index(vector* events) {
    int index, status;
    status = MENU_INVALID;
    if (!events->count) {
        printf("\nNincsenek rendezvenyek!\n");
        return -1;
    }
    while (status == MENU_INVALID) {
        printf("Adja meg az esemény sorszámát (1-%d): ", events->count);
        if (scanf("%d", &index) ==1 && index > 0 && index <= events->count) {
            status = MENU_VALID;
            return index-1;
        } else {
            flush_stdin();
            printf("Helytelen sorszam!\n");
        }
    }
};

static int get_guest_index(Event* event) {
    int index, status;
    vector * guests = &event->guests;
    status = MENU_INVALID;
    if (!guests->count) {
        printf("\nNincsenek vendegek!\n");
        return -1;
    }
    while (status == MENU_INVALID) {
        printf("Adja meg a vendeg sorszámát (1-%d): ", guests->count);
        if (scanf("%d", &index) ==1 && index > 0 && index <= guests->count) {
            status = MENU_VALID;
            return index-1;
        } else {
            flush_stdin();
            printf("Helytelen sorszam!\n");
        }
    }
};

void flush_stdin() {
    while (getchar() != '\n'); 
}

void delete_event(vector* events) {
    int index;
    Event* e;
    index = get_event_index(events);
    if (index != -1) {
        e = vector_get(events, index);
        vector_empty(&e->guests);
        vector_delete(events, index);
        printf("Az esemény torolve\n");
    }
}

void read_string(int min_length, char* question, char* target, int allowempty) {
    int status;
    static char buffer[40]; 
    int readstatus;

    status = MENU_INVALID;
    while (status == MENU_INVALID) {
        printf("%s", question);
        readstatus = scanf("%s", buffer); 
        if (readstatus && strlen(buffer)>min_length) {
            status = MENU_VALID;
            strcpy(target, buffer);
        } else if (strcmp(buffer,"-") == 0 && allowempty) {
            status = MENU_VALID;
        } else {
        printf("Helytelen adat (legalabb %d karakter hosszunak kell lennie)\n", min_length);
        }
    }
}

void add_event(vector * events) {
    Event * e;
    e = event_init();
    read_string(3, "Esemeny neve: ", e->name,0);
    vector_add(events, e);
}

void add_guest(Event* event) {
    Guest* g;
    g = guest_init();
    read_string(3, "Vendeg neve: ", g->name,0);
    read_string(8, "Vendeg emailcime: ", g->mail,0);
    vector_add(&event->guests, g);
    printf("\nVendeg hozzaadva\n");
}

void delete_guest(Event* event) {
    int index;
    index = get_guest_index(event);
    if (index == -1) return;
    vector_delete(&event->guests, index);
    printf("Vendeg torolve");
}

void edit_guest(Event* event) {
    int index;
    Guest* g;
    index = get_guest_index(event);
    if (index == -1) return;
    g = vector_get(&event->guests, index);
    printf("\nValtozatlanul hagyashoz irjon be egy kotojelet: - !\n");
    read_string(3, "Vendeg uj neve: ", g->name, 1);
    read_string(8, "Vendeg uj emailcime: ", g->mail, 1);
    printf("\nAdatok modositva\n");
}
