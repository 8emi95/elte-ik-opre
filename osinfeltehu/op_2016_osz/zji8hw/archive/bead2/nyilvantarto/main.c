/*
 * Operációs rendszerek - ELTE-IK
 * 2. Beadandó feladat: "King of Stands" - Nyilvántartó
 * (az 1. Beadandó feladat módosított változata)
 * nyilvantarto/main.c
 *
 * Készítette: Keszei Ábel
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "../common/linkedlist.h"
#include "../common/defines.h"

linkedlist *visitor_list;
linkedlist *event_list;

void clearStdin()
{
    int ch;
    while ((ch = getchar()) != EOF && ch != '\n');
}

bool readChar(char *r, char *prompt)
{
    printf(prompt);

    int c = getchar();
    if (c == '\n')
    {
        fprintf(stderr, "(!) Hiba: ures a bemenet!\n");
        return false;
    }

    int c2 = getchar();
    if (c2 != '\n')
    {
        fprintf(stderr, "(!) Hiba: tul hosszu a bemenet!\n");
        clearStdin();
        return false;
    }

    *r = (char)c;
    return true;
}

bool readString(char *r, char *prompt, int len)
{
    printf(prompt);

    char str[len + 1];

    int c = getchar();
    if (c == '\n')
    {
        fprintf(stderr, "(!) Hiba: ures a bemenet!\n");
        return false;
    };

    int count = 0;
    do {
        if (count == len)
        {
            fprintf(stderr, "(!) Hiba: tul hosszu a bemenet!\n");
            clearStdin();
            return false;
        }

        str[count++] = (char)c;
    } while ((count < len + 1) && (c = getchar()) != '\n');

    str[count] = '\0';
    memcpy(r, str, (size_t)len + 1);
    return true;
}

bool readInt(int *r, char *prompt)
{
    char str[9];
    if (!readString(str, prompt, 9))
        return false;

    int x = strtol(str, NULL, 10);
    if (x == 0 && str[0] != '0')
    {
        fprintf(stderr, "(!) Hiba: szam megadasa szukseges!\n");
        return false;
    }

    *r = x;
    return true;
}

void load_visitors()
{
    FILE *f = fopen(VISITORS_FILE, "r");
    if (f == NULL)
        return;

    printf("Latogatoi adatok betoltese...  ");

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

        ndata *d = malloc(sizeof(ndata));
        d->visitor_d = v;
        listadd(visitor_list, d);
    }

    if (line)
        free(line);

    fclose(f);
    printf("OK\n");
}

void save_visitors()
{
    FILE *f = fopen(VISITORS_FILE, "w");

    node *act = visitor_list->first;
    while (act != NULL)
    {
        visitor *v = &act->data->visitor_d;
        fprintf(f, "%s;%s;%i;%ld\n", v->name, v->email, v->eventID, (long)v->date);
        act = act->next;
    }

    fclose(f);
}

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
    printf("OK\n");
}

void save_events()
{
    FILE *f = fopen(EVENTS_FILE, "w");

    node *act = event_list->first;
    while (act != NULL)
    {
        event *e = &act->data->event_d;
        fprintf(f, "%i;%s;%s;\n", e->ID, e->name, e->location);
        act = act->next;
    }

    fclose(f);
}

bool event_name(char *r, int id)
{
    node *act = event_list->first;
    while (act != NULL)
    {
        if (act->data->event_d.ID == id)
        {
            char *n = act->data->event_d.name;
            memcpy(r, n, sizeof(n));
            return true;
        }

        act = act->next;
    }

    return false;
}

void print_visitors(bool with_id)
{
    node *act = visitor_list->first;
    int cnt = 0;
    while (act != NULL)
    {
        visitor vis = act->data->visitor_d;

        char tbuf[20];
        strftime(tbuf, 20, "%Y-%m-%d %H:%M", localtime(&vis.date));

        char nbuf[STRING_MAX + 1];
        memset(nbuf, '\0', sizeof(nbuf));
        event_name(nbuf, vis.eventID);

        if (with_id)
            printf("#%i: Nev: %s, E-mail: %s, Rendezveny: %s (%i), Datum: %s\n", cnt, vis.name, vis.email, nbuf, vis.eventID, tbuf);
        else
            printf("Nev: %s, E-mail: %s, Rendezveny: %s (%i), Datum: %s\n", vis.name, vis.email, nbuf, vis.eventID, tbuf);

        act = act->next;
        cnt++;
    }
}

bool event_exists(int id)
{
    node *act = event_list->first;
    while (act != NULL)
    {
        if (act->data->event_d.ID == id)
            return true;

        act = act->next;
    }
    return false;
}

bool menu()
{
    char c;
    bool _continue = true;

    printf(" ..:: Latogatoi nyilvantartas ~ King of Stands ::..\n");
    printf("---------------------------------------------------\n");
    printf("1 - Latogatoi jelentkezes\n");
    printf("2 - Latogato adatainak modositasa\n");
    printf("3 - Latogato adatainak torlese\n");
    printf("4 - Jelentkezettek listaja\n");
    printf("5 - Rendezveny inditasa\n");
    printf("6 - Rendezveny torlese\n");
    printf("7 - Kilepes\n\n");

    if (!readChar(&c, "Valasztas: "))
        return _continue;

    switch (c)
    {
        case '1':
            printf("\n -= Latogatoi jelentkezes =-\n");

            visitor v;
            while (!readString(v.name, "Nev: ", STRING_MAX));
            while (!readString(v.email, "E-mail cim: ", STRING_MAX));
            while (!readInt(&v.eventID, "Rendezveny azonosito: "));
            v.date = time(0);

            if (!event_exists(v.eventID))
            {
                fprintf(stderr, "(!) Hiba: nincs ilyen azonositoju rendezveny!\n");
                break;
            }

            ndata *d = malloc(sizeof(ndata));
            d->visitor_d = v;
            listadd(visitor_list, d);

            save_visitors();
            printf("\nLatogato mentve. Ez a(z) %i. latogato.\n", visitor_list->count);
            break;
        case '2':
            printf("\n -= Latogato adatainak modositasa =-\n");
            if (visitor_list->count == 0)
            {
                printf("Nincsenek jelentkezettek!\n");
                break;
            }

            print_visitors(true);
            printf("\n");

            int ch;
            while (!readInt(&ch, "Melyiket modositjuk? ") || ch < 0 || ch >= visitor_list->count)
            {
                if (ch < 0 || ch >= visitor_list->count)
                    fprintf(stderr, "(!) Hiba: nincs ilyen jelentkezo!\n");
            }

            printf("\n");

            visitor v2;
            while (!readString(v2.name, "Nev: ", STRING_MAX));
            while (!readString(v2.email, "E-mail cim: ", STRING_MAX));
            while (!readInt(&v2.eventID, "Rendezveny azonosito: "));

            if (!event_exists(v2.eventID))
            {
                fprintf(stderr, "(!) Hiba: nincs ilyen azonositoju rendezveny!\n");
                break;
            }

            node *vis_node = listget(visitor_list, ch);
            v2.date = vis_node->data->visitor_d.date;
            vis_node->data->visitor_d = v2;

            save_visitors();
            printf("\nLatogato adatai modositva.\n");
            break;
        case '3':
            printf("\n -= Latogato adatainak torlese =-\n");
            if (visitor_list->count == 0)
            {
                printf("Nincsenek jelentkezettek!\n");
                break;
            }

            print_visitors(true);
            printf("\n");

            int ch2;
            while (!readInt(&ch2, "Melyiket toroljuk? ") || ch2 < 0 || ch2 >= visitor_list->count)
            {
                if (ch2 < 0 || ch2 >= visitor_list->count)
                    fprintf(stderr, "(!) Hiba: nincs ilyen jelentkezo!\n");
            }

            listremove(visitor_list, ch2);

            save_visitors();
            printf("\nLatogato adatai torolve.\n");
            break;
        case '4':
            printf("\n -= Jelentkezettek listaja =-\n");

            if (visitor_list->count == 0)
            {
                printf("Nincsenek jelentkezettek!\n");
                break;
            }

            print_visitors(false);
            break;
        case '5':
            printf("\n -= Rendezveny inditasa =-\n");

            event e;
            while (!readInt(&e.ID, "Rendezveny azonosito: "));

            if (event_exists(e.ID))
            {
                fprintf(stderr, "(!) Hiba: mar van ilyen azonositoju rendezveny!\n");
                break;
            }

            while (!readString(e.name, "Nev: ", STRING_MAX));
            while (!readString(e.location, "Helyszin: ", STRING_MAX));

            ndata *d2 = malloc(sizeof(ndata));
            d2->event_d = e;
            listadd(event_list, d2);

            save_events();
            printf("\nRendezveny mentve.\n");
            break;
        case '6':
            printf("\n -= Rendezveny torlese =-\n");
            if (event_list->count == 0)
            {
                printf("Nincs egy rendezveny sem!\n");
                break;
            }

            node *act = event_list->first;
            int cnt = 0;
            while (act != NULL)
            {
                event ev = act->data->event_d;
                printf("#%i: Azonosito: %i, Nev: %s\n", cnt, ev.ID, ev.name);

                act = act->next;
                cnt++;
            }

            int ch3;
            while (!readInt(&ch3, "Melyiket toroljuk? ") || ch3 < 0 || ch3 >= event_list->count)
            {
                if (ch3 < 0 || ch3 >= event_list->count)
                    fprintf(stderr, "(!) Hiba: nincs ilyen rendezveny!\n");
            }

            node *rem = listget(event_list, ch3);
            int tmp_id = rem->data->event_d.ID;
            listremove(event_list, ch3);

            act = visitor_list->first;
            int count = 0;
            while (act != NULL)
            {
                if (act->data->visitor_d.eventID == tmp_id)
                {
                    act = act->next;
                    listremove(visitor_list, count);
                }
                else
                {
                    count++;
                    act = act->next;
                }
            }

            save_events();
            save_visitors();
            printf("\nRendezveny osszes adata torolve.\n");
            break;
        case '7':
            printf("Viszlat!\n");
            _continue = false;
            break;
        default:
            fprintf(stderr, "Nincs ilyen menupont!\n");
            break;
    }

    printf("\n");
    return _continue;
}

int main(int argc, char **argv)
{
    visitor_list = initlist();
    event_list = initlist();

    load_visitors();
    load_events();

    printf("\n");
    while (menu());

    freelist(visitor_list);
    freelist(event_list);
    return 0;
}