#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "types.h"

void guest_add(Event* event, char * name, char * mail) {
    Guest* guest = guest_init(name, mail);
    vector_add(&event->guests, guest);

}


int main() {
    vector events;
    events = vector_init();
    
    vector_add(&events, event_init("Tacsko vilagtalalkozo"));
    vector_add(&events, event_init("idsfksfjzo"));
    Guest* g1 = guest_init("Morzsika", "morzsi@dog.de");
    Guest* g2 = guest_init("Jancsika", "jancsi@dog.de");

    Event *ev = vector_get(&events, 0);
    vector_add(&ev->guests, g1);
    vector_add(&ev->guests, g2);
    guest_add(ev, "Csabika", "dddd");    
    guest_add(ev, "Csabika", "dddd");    
    guest_add(ev, "Csabika", "dddd");    
    guest_add(ev, "Csabika", "dddd");    
    guest_add(ev, "Csabika", "dddd");    
    guest_add(ev, "Csabika", "dddd");    
    vector_delete(&ev->guests, 1);
    int i;
    for (i = 0; i < ev->guests.count; i++) {
        Guest* g = vector_get(&ev->guests, i);
        printf("%s, %s, %s\n", g->name, g->mail, g->entry_date);
    }

    printf("\n\n\n");

    for (i = 0; i<events.count; i++) {
        ev = vector_get(&events, i);
        puts(ev->name);
    }

    return 0;

}

