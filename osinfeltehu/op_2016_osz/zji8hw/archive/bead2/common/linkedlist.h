/*
 * Operációs rendszerek - ELTE-IK
 * 2. Beadandó feladat: "King of Stands"
 * common/linkedlist.h - láncolt lista header
 *
 * Készítette: Keszei Ábel
 */

#ifndef BEAD1_LINKEDLIST_H
#define BEAD1_LINKEDLIST_H

#include <time.h>
#include <stdbool.h>

#define STRING_MAX 50

typedef struct
{
    char name[STRING_MAX + 1];
    char email[STRING_MAX + 1];
    int eventID;
    time_t date;
    bool attended;
} visitor;

typedef struct
{
    int ID;
    char name[STRING_MAX + 1];
    char location[STRING_MAX + 1];
} event;

typedef union {
    visitor visitor_d;
    event event_d;
} ndata;

typedef struct _node
{
    ndata *data;
    struct _node *next;
} node;

typedef struct
{
    node *first;
    node *last;
    int count;
} linkedlist;

linkedlist *initlist(void);
void freelist(linkedlist*);
node *listget(linkedlist*, int);
void listadd(linkedlist*, ndata*);
void listremove(linkedlist*, int);

#endif //BEAD1_LINKEDLIST_H
