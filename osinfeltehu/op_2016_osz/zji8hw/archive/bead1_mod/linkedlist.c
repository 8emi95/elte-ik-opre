/*
 * Operációs rendszerek - ELTE-IK
 * 1. Beadandó feladat: "King of Stands"
 * linkedlist.c - láncolt lista megvalósítás
 *
 * Készítette: Keszei Ábel
 */

#include <stdlib.h>
#include "linkedlist.h"

linkedlist *initlist()
{
    linkedlist *list = malloc(sizeof(linkedlist));
    list->first = NULL;
    list->last = NULL;
    list->count = 0;

    return list;
}

void freelist(linkedlist *list)
{
    node *act = list->first;
    while (act != NULL)
    {
        node *n = act->next;
        free(act->data);
        free(act);
        act = n;
    }
    free(list);
}

node *listget(linkedlist *list, int at)
{
    node *act = list->first;
    int cnt = 0;
    while (act != NULL)
    {
        if (cnt == at)
            return act;

        act = act->next;
        cnt++;
    }

    return NULL;
}

void listadd(linkedlist *list, ndata *data)
{
    node *n = malloc(sizeof(node));
    n->data = data;
    n->next = NULL;

    if (list->count == 0)
    {
        list->first = n;
        list->last = n;
    }
    else
    {
        list->last->next = n;
        list->last = n;
    }

    list->count++;
}

void listremove(linkedlist *list, int at)
{
    if (list->count <= at || at < 0)
        return;

    if (at == 0)
    {
        node *rem = list->first;
        list->first = rem->next;

        if (list->count == 0)
            list->last = NULL;

        free(rem->data);
        free(rem);
    }
    else
    {
        int cnt = 0;
        node *act = list->first;
        while (cnt < at - 1)
        {
            act = act->next;
            cnt++;
        }

        node *rem = act->next;
        act->next = rem->next;

        if (at == list->count - 1)
            list->last = act;

        free(rem->data);
        free(rem);
    }

    list->count--;
}