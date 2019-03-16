#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "vector.h"
#include "types.h"

Guest* guest_init() {
    Guest * guest;
    Guest g;

    guest = (Guest *)malloc(sizeof(Guest));
    get_timestamp(g.entry_date);
    * guest = g;
    return guest;
}

Event* event_init() {
    Event* event;
    Event e;

    event = (Event *)malloc(sizeof(Event));
    e.guests = vector_init();
    * event = e;
    return event;
}

static void get_timestamp(char* ts) {
    time_t currtime;
    struct tm * tm;

    time(&currtime);
    tm = localtime(&currtime);
    strftime(ts, 20,"%Y-%m-%d %H:%M:%S", tm);
}
