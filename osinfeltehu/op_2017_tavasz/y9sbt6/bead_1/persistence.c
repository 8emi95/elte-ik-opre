#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // open
#include <unistd.h> // close
#include "persistence.h"

int save_questions(const question_list* list)
{
    int outfile = open(QUESTIONS_FILE, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    
    question_node* node = list->head->next;
    write(outfile, &(list->number), sizeof(list->number));

    while (node != list->last)
    {
        if (write(outfile, &(node->qstn), sizeof(node->qstn)) == -1)
        {
            close(outfile);
            return 0;
        }

        node = node->next;
    }
    close(outfile);

    return 1;
}

int load_questions(question_list* list)
{
    if (access(QUESTIONS_FILE, F_OK) != -1)
    {
        int infile = open(QUESTIONS_FILE, O_RDONLY);
        size_t qstn_num;
        read(infile, &qstn_num, sizeof(qstn_num));

        size_t i;
        for (i = 0; i < qstn_num; ++i)
        {
            question qstn;
            read(infile, &qstn, sizeof(qstn));
            insert_qstn(list, &qstn);
        }
        close(infile);
        return 1;
    }
    else
    {
        list->number = 0;
        return 0;
    }  
}
