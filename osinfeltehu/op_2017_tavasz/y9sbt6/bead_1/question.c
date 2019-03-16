#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "question.h"

void print_qstn(const question* qstn)
{
    char* time_str = ctime(&(qstn->registered));
    time_str[strlen(time_str) - 1] = '\0';
    printf("  %zd | %s | %s\n", qstn->id, qstn->text, time_str);
    
    size_t i;
    for (i = 0; i < qstn->answer_num; ++i)
    {
        printf("    %zd. %s\n", i + 1, qstn->answers[i]);
    }
}

void clear_answers(question* qstn)
{
    size_t i;
    for (i = 0; i < MAX_ANSWER_NUM; ++i)
    {
        qstn->answers[i][0] = '\0';
    }
}

void insert_qstn(question_list* into, question* qstn)
{
    question_node* node = malloc(sizeof(question_node));

    node->qstn = *qstn;
    node->prev = into->last->prev;
    node->next = into->last;
    node->prev->next = node;
    node->next->prev = node;

    ++(into->number);
    node->qstn.id = into->number;    
}

void modify_qstn(question* from, const question* to)
{
    from->selected = to->selected;
    from->answer_num = to->answer_num;
    strcpy(from->text, to->text);
    
    size_t i;
    for (i = 0; i < MAX_ANSWER_NUM; ++i)
    {
        strcpy(from->answers[i], to->answers[i]);
    }    
}

int remove_qstn(question_list* from, size_t id)
{
    question_node* node = from->head->next;
    while ((node != from->last) && (node->qstn.id != id))
    {
        node = node->next;
    }

    if (node != from->last)
    {
        question_node* corrctn = node->next;
        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->prev = node->next = NULL;
        free(node);
        --(from->number);

        // lower question ids by 1, after the deleted question
        while (corrctn != from->last)
        {
            --(corrctn->qstn.id);
            corrctn = corrctn->next;
        }

        return 1;
    }
    else
    {
        return 0;
    }
}

void init_qstn_list(question_list* list)
{
    list->head->prev = list->last->next = NULL;
    list->head->next = list->last;
    list->last->prev = list->head;
    list->number = 0;
}

question* find_qstn(question_list* list, const size_t id)
{
    question_node* node = list->head->next;

    while ((node != list->last) && (node->qstn.id != id))
    {
        node = node->next;
    }

    if (node != list->last)
    {
        return &(node->qstn);
    }
    else
    {
        return NULL;
    }
}
