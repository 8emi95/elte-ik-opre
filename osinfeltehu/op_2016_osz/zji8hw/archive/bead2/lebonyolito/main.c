/*
 * Operációs rendszerek - ELTE-IK
 * 2. Beadandó feladat: "King of Stands" - Lebonyolító
 * lebonyolito/main.c
 *
 * Készítette: Keszei Ábel
 */

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>

#include "../common/linkedlist.h"
#include "../common/defines.h"

linkedlist *event_list;

void load_events()
{
    FILE *f = fopen(EVENTS_FILE, "r");
    if (f == NULL)
        return;

    printf("Rendezveny adatok betoltese...  ");

    event e;
    size_t len = 72;
    char *line = malloc(len);

    while (fgets(line, len, f) != NULL)
    {
        e.ID = strtol(strtok(line, ";"), NULL, 10);

        memset(e.name, '\0', sizeof(e.name));
        strcpy(e.name, strtok(NULL, ";"));

        memset(e.location, '\0', sizeof(e.location));
        strcpy(e.location, strtok(NULL, ";"));

        ndata *d = malloc(sizeof(ndata));
        d->event_d = e;
        listadd(event_list, d);
    }

    if (line)
        free(line);

    fclose(f);
    printf("OK\n\n");
}

void load_evt_visitors(linkedlist *list, int evt_id)
{
    FILE *f = fopen(VISITORS_FILE, "r");
    if (f == NULL)
        return;

    visitor v;
    size_t len = 130;
    char *line = malloc(len);

    while (fgets(line, len, f) != NULL)
    {
        memset(v.name, '\0', sizeof(v.name));
        strcpy(v.name, strtok(line, ";"));

        memset(v.email, '\0', sizeof(v.email));
        strcpy(v.email, strtok(NULL, ";"));

        v.eventID = strtol(strtok(NULL, ";"), NULL, 10);
        v.date = strtol(strtok(NULL, ";"), NULL, 10);

        if (v.eventID == evt_id)
        {
            ndata *d = malloc(sizeof(ndata));
            d->visitor_d = v;
            listadd(list, d);
        }
    }

    if (line)
        free(line);

    fclose(f);
}

void nothing(int n) {}

void wait_and_handle(int signum, void (*f)(int))
{
    struct sigaction sigact;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_handler = f;
    sigact.sa_flags = 0;
    sigaction(signum, &sigact, NULL);

    sigset_t sigmask;
    sigfillset(&sigmask);
    sigdelset(&sigmask, signum);

    sigsuspend(&sigmask);
}

void wait_for(int signum)
{
    wait_and_handle(signum, nothing);
}

void start_timeout_handler()
{
    // véletlenszerű az is, hogy melyik esemény következik
    int rand_idx = rand() % event_list->count;
    node *next_event = listget(event_list, rand_idx);

    event evt = next_event->data->event_d;

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        perror("opening pipe");
        exit(1);
    }

    pid_t child_pid = fork();
    if (child_pid < 0)
    {
        perror("creating child");
        exit(1);
    }
    else if (child_pid == 0)
    {
        // (2) helyszín fogadása
        char s[100];
        memset(s, '\0', sizeof(s));
        read(pipe_fd[0], s, sizeof(s));
        printf("(2)(C) kapott helyszin: %s\n", s);

        // (3) leutazás
        int timeout = rand() % 3 + 1;                           // véletlen hosszú ideig tart a leutazás
        printf("(3)(C) utazas (%d napig tart)...\n", timeout);

        alarm(timeout);
        wait_for(SIGALRM);

        // (4) visszajelzés a megérkezésről
        printf("(4)(C) megerkezett a helyszinre\n");
        kill(getppid(), SIGUSR1);

        // (5) résztvevők adatainak fogadása
        printf("(5)(C) resztvevok adatainak fogadasa...\n");

        int count = 0;
        FILE *f = fdopen(pipe_fd[0], "r");
        size_t len = 50;
        char *line = malloc(len);
        linkedlist *names = initlist();

        while (fgets(line, len, f) != NULL)
        {
            if (strcmp(line, "__end__\n") == 0)
                break;

            printf("\t%d. resztvevo: %s\n", ++count, line);
            ndata *d = malloc(sizeof(ndata));
            strcpy(d->visitor_d.name, line);
            listadd(names, d);
        }

        if (line)
            free(line);

        // (6) várakozás a résztvevőkre
        printf("(6)(C) rendezveny indul, varakozas a resztvevokre...\n");

        node *n = names->first;
        while (n != NULL)
        {
            visitor *v = &n->data->visitor_d;
            v->attended = rand() % 100 < 90;    // 90% eséllyel jelent meg a résztvevő

            if (v->attended)
            {
                alarm(rand() % 5 + 2);          // véletlenszerű időközönként érkeznek a résztvevők
                wait_for(SIGALRM);
                printf("\tmegerkezett a rendezvenyre: %s\n", v->name);
            }

            n = n->next;
        }

        // (7) részvételi adatok visszaküldése
        kill(getppid(), SIGUSR1);

        printf("(7)(C) reszveteli adatok kuldese...\n");
        char success[100];
        sprintf(success, "%d", rand() % 100 + 1);           // random % sikeresség
        write(pipe_fd[1], success, strlen(success) + 1);
        write(pipe_fd[1], "\n", 1);

        n = names->first;
        while (n != NULL)
        {
            visitor v = n->data->visitor_d;
            if (!v.attended)
            {
                write(pipe_fd[1], v.name, strlen(v.name) + 1);
                write(pipe_fd[1], "\n", 1);
            }
            n = n->next;
        }
        write(pipe_fd[1], "__end__\n", 8);

        freelist(names);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        exit(0);
    }
    else
    {
        // (1) helyszín küldése
        printf("(1)(P) helyszin kuldese...\n");
        write(pipe_fd[1], evt.location, sizeof(evt.location));

        wait_for(SIGUSR1);

        // (5) résztvevők adatainak leküldése
        printf("(5)(P) resztvevok adatainak kuldese...\n");
        linkedlist *visitors = initlist();
        load_evt_visitors(visitors, evt.ID);

        node *act = visitors->first;
        while (act != NULL)
        {
            char *vis_name = act->data->visitor_d.name;
            write(pipe_fd[1], vis_name, strlen(vis_name) + 1);
            write(pipe_fd[1], "\n", 1);

            act = act->next;
        }
        write(pipe_fd[1], "__end__\n", 8);
        freelist(visitors);

        wait_for(SIGUSR1);

        // (7) részvételi adatok fogadása
        printf("(7)(P) reszveteli adatok fogadasa...\n");
        FILE *f = fdopen(pipe_fd[0], "r");
        size_t len = 50;
        char *line = malloc(len);
        bool first = true;
        int count = 0;

        while (fgets(line, len, f) != NULL)
        {
            if (strcmp(line, "__end__\n") == 0)
                break;

            if (first)
            {
                printf("\ta rendezveny %s%%-ban volt sikeres\n", line);
                first = false;
            }
            else
            {
                count++;
                printf("\t%s nem jelent meg a rendezvenyen\n", line);
            }
        }

        if (count == 0)
            printf("\tmindenki megjelent a rendezvenyen\n");

        if (line)
            free(line);

        close(pipe_fd[0]);
        close(pipe_fd[1]);
    }

    listremove(event_list, rand_idx);
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    event_list = initlist();
    load_events();

    printf(" ..:: Rendezveny lebonyolito rendszer ~ King of Stands ::..\n");
    printf("-----------------------------------------------------------");

    if (event_list->count == 0)
    {
        printf("\n(!) Nincsenek rendezvenyek!\n");
        return 0;
    }

    while (event_list->count != 0)
    {
        // véletlen idő múlva indul a következő esemény
        int timeout = rand() % 5 + 2;
        printf("\nA kovetkezo rendezveny indul: %d nap (masodperc)...\n", timeout);

        alarm(timeout);
        wait_and_handle(SIGALRM, start_timeout_handler);
    }

    // lezajlottak az rendezvények, tiszta lappal indulunk
    remove(EVENTS_FILE);
    remove(VISITORS_FILE);

    freelist(event_list);
    return 0;
}