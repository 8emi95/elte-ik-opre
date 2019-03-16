#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "visitor.h"
#include "event.h"
#include "vector.h"
#include <time.h>

#define NEW_EVENT 1
#define NEW_VISITOR 4
#define LIST_VISITOR 5
#define DELETE_VISITOR 6
#define LIST_EVENTS 2
#define DELETE_EVENTS 3
#define SAVE_CHANGES 7
#define MODIFY_VISITOR 8
#define EXIT 9

void action(int* const p);
void add_event(vector_* events);
void add_visitor(vector_* events);
void list_events(vector_* events);
void list_visitors(Event* ev);
void delete_visitor(Event* ev);
void save_changes(vector_* events);
void init_Events(vector_* events);
void init_Visitors(vector_* events);
void modify_visitor(Event* ev);
void* select_event(vector_* events); // returns Event struct or NULL
void* select_visitor(Event* ev); // returns index of the visitor or NULL
int select_eventIndex(vector_* events); // return index* of Event or NULL
void delete_event(vector_* events);

int main() {
    vector_ events;
    vector_init(&events);
    init_Events(&events);
    init_Visitors(&events);
    int* const todop = (int *) malloc(sizeof(int));
    do {
        action(todop);
        switch(*todop) {
            case NEW_EVENT:
                add_event(&events);
                break;
            case NEW_VISITOR:
                add_visitor(&events);
                break;
            case LIST_VISITOR:
                list_visitors(select_event(&events));
                break;
            case DELETE_VISITOR:
                delete_visitor(select_event(&events));
                break;
            case LIST_EVENTS:
                list_events(&events);
                break;
            case DELETE_EVENTS:
                delete_event(&events);
                break;
            case SAVE_CHANGES:
                save_changes(&events);
                break;
            case MODIFY_VISITOR:
                modify_visitor(select_event(&events));
                break;
            case EXIT:
                break;
            default:
                printf("No such option\n");
        }
    } while(*todop != EXIT);


    event_free(&events);
    return 0;
}

void action(int* const p) {
    printf("What action would you like to perform?\n");
    printf("%i. Start new event\n", NEW_EVENT);
    printf("%i. List events\n", LIST_EVENTS);
    printf("%i. Delete Event\n", DELETE_EVENTS);
    printf("%i. Add new visitor to an event\n", NEW_VISITOR);
    printf("%i. List visitors from an event\n", LIST_VISITOR);
    printf("%i. Delete visitor from an event\n", DELETE_VISITOR);
    printf("%i. Save changes\n", SAVE_CHANGES); // Debug function
    printf("%i. Modify Visitor\n", MODIFY_VISITOR);
    printf("%i. Exit\n", EXIT);
    int hiba;
    do {
        hiba = scanf("%i", p);
    } while(hiba <= 0);
}

void list_events(vector_* events) {
    int i = 0;
    while(i < vector_count(events)) {
        printf("%i", i + 1);
        printf(". ");
        printEvent((Event*) vector_get(events, i));
        i++;
    }
}

void add_event(vector_* events) {
    Event* ev;
    ev = (Event *) malloc(sizeof(Event));
    init_Event(ev);
    printf("What is the event' ID?\n");
    scanf("%255s", ev->_id);
    printf("What is the event's name?\n");
    scanf("%255s", ev->_name);
    vector_add(events, ev);
}

void add_visitor(vector_* events) {
    printf("What is the event's ID?\n");
    Visitor* vis;
    vis = (Visitor*) malloc(sizeof(Visitor));
    scanf("%255s", vis->_id);
    int found = 0;
    int i = 0;
    while(i < vector_count(events) && found != 1) {
        if(strcmp(vis->_id, event_id((Event*) vector_get(events, i))) == 0) {
            printf("Event found!\n");
            printf("What is your name?\n");
            scanf("%255s", vis->_name);
            printf("What is your E-mail address?\n");
            scanf("%255s", vis->_email);
            printf("You are the ");
            time ( &vis->_time );
            printf("%i.", add_visitorToEvent(vector_get(events, i), vis));
            printf(" visitor. Enjoy yourself\n");
            found = 1;
        }
        i++;
    }
    if(found == 0) {
        printf("Event not found\n");
    }
}

void* select_event(vector_* events) {
    if(vector_count(events) != 0) {
        printf("Select the event\n");
        list_events(events);
        int* which = (int *) malloc(sizeof(int));
        scanf("%i", which);
        *which = *which - 1;
        if(*which >= vector_count(events) || *which < 0){
            return NULL;
        } else {
            return (Event*) get_event(events, *which);
        }
    } else {
        return NULL;
    }
}

int select_eventIndex(vector_* events) {
    if(vector_count(events) != 0) {
        printf("Select the event\n");
        list_events(events);
        int* which = (int *) malloc(sizeof(int));
        scanf("%i", which);
        *which = *which - 1;
        if(*which >= vector_count(events) || *which < 0){
            return -1;
        } else {
            return *which;
        }
    } else {
        return -1;
    }
}

void delete_event(vector_* events) {
    vector_delete(events, select_eventIndex(events));
}

void* select_visitor(Event* ev) {
    if(get_visitorSize(ev) != 0) {
        int* which = (int *) malloc(sizeof(int));
        int i = 0;

        printf("Select the visitor\n");
        while(get_visitorSize(ev) > i) {
            printf("%i %s", i + 1, " ");
            printVisitor(get_visitor(ev, i));
            i++;
        }
        scanf("%i", which);
        *which = *which - 1;
        if(*which >= get_visitorSize(ev) || *which < 0) {
            return NULL;
        } else {
            return which;
        }
    } else {
        return NULL;
    }
}

void list_visitors(Event* ev) {
    if(ev != NULL) {
         if(get_visitorSize(ev) != 0) {
            int i = 0;

            while(get_visitorSize(ev) > i) {
                printVisitor(get_visitor(ev, i));
                i++;
            }
        } else {
            printf("There are no visitors on this event!\n");
        }
    } else {
        printf("There is no such event!\n");
    }
}

void delete_visitor(Event* ev) {
    if(ev == NULL){
        printf("Event not found!\n");
    } else {
        if(get_visitorSize(ev) != 0) {
            delete_visitorFromVector(ev, select_visitor(ev));
        } else {
            printf("There are no visitors on this event!\n");
        }
    }
}

void init_Events(vector_* events) {
    FILE* eventsFileRead = fopen("events.txt", "r");
    char evID[EVENT_ID_SIZE];
    char evName[EVENT_NAME_SIZE];
    while(fscanf(eventsFileRead, "%s\t%s" , evID, evName) == 2) {
        Event* ev;
        ev = (Event *) malloc(sizeof(Event));
        init_Event(ev);
        set_eventID(ev, evID);
        set_eventName(ev, evName);
        vector_add(events, ev);
    }
    if(feof(eventsFileRead)) {
        printf("Data successfully loaded!\n");
    } else {
        printf("An error has occured during reading data!\n");
    }
    fclose(eventsFileRead);
}

void modify_visitor(Event* ev) {
    if(ev == NULL) {
        printf("No such event!\n");
    } else {
        void* index = select_visitor(ev);
        if(index == NULL) {
            printf("No such visitor!\n");
        } else {
            Visitor* vis = get_visitor(ev, *(int*) index);
            int* action = (int*) malloc(sizeof(int));
            while(*action != 3) {
                printf("What would you like to modify?\n");
                printf("1. Name\n");
                printf("2. E-mail\n");
                printf("3. Done\n");
                scanf("%1i", action);
                if(*action == 1) {
                    char visName[255];
                    scanf("%255s", visName);
                    set_visitorName(vis, visName);
                } else if(*action == 2) {
                    char visEmail[255];
                    scanf("%255s", visEmail);
                    set_visitorEmail(vis, visEmail);
                }
            }
        }
    }
}

void init_Visitors(vector_* events) {
    FILE* visitorsFileRead = fopen("visitors.txt", "r");
    char rawtime[256];
    char visName[255];
    char visEmail[255];
    char visID[255];
    int found = -1;
    while(fgets(rawtime, 256, visitorsFileRead)) {
        if(fscanf(visitorsFileRead, "%s %s %s", visID,visName,visEmail) == 3) {

            int i = 0;
            while(vector_count(events) > i && found == -1) {
                 char* tempId = event_id((Event*) vector_get(events, i));
                 if(strcmp(tempId, visID) == 0) {
                    found = i;
                 }
                 i++;
            }
            i = 0;
            while(i < strlen(visName)) {
                if((char)visName[i] == '_') {
                    visName[i] = ' ';
                }
                i++;
            }

            Visitor* vis = (Visitor *) malloc(sizeof(Visitor));
            set_visitorID(vis, visID);
            set_visitorEmail(vis, visEmail);
            set_visitorName(vis, visName);

            struct tm tm;
            strptime(rawtime, "%F %T", &tm);
            time_t t = mktime(&tm);
            vis->_time = t;
            if(found >= 0) {
                add_visitorToEvent((Event*) vector_get(events, found), vis);
            } else {
                free(vis);
            }
        }
    }
    if(!feof(visitorsFileRead)) {
        printf("Error happened while reading visitors!\n");
    }
    fclose(visitorsFileRead);
}

void save_changes(vector_* events) {
    FILE* save_events = fopen("events.txt", "w");
    FILE* save_visitors = fopen("visitors.txt", "w");
    int i = 0;
    while(i < vector_count(events)) {
        Event* ev = get_event(events, i);
        fputs(ev->_id, save_events);
        fputs(" ", save_events);
        fputs(ev->_name, save_events);
        fputs("\n", save_events);
        int m = 0;
        while(m < get_visitorSize(ev)) {
            struct tm *ptm = gmtime(&get_visitor(ev, m)->_time);
            char buf[256];
            strftime(buf, sizeof buf, "%F %T", ptm);
            fputs(buf, save_visitors);
            fputs("\n", save_visitors);
            fputs(get_visitor(ev, m)->_id, save_visitors);
            fputs(" ", save_visitors);
            fputs(get_visitor(ev, m)->_name, save_visitors);
            fputs(" ", save_visitors);
            fputs(get_visitor(ev, m)->_email, save_visitors);
            fputs("\n", save_visitors);
            m++;
        }
        i++;
    }
}
