#include <stdio.h>
#include <stdlib.h>
#include "question.h"
#include "menu.h"
#include "persistence.h"

int main(int argc, char** argv)
{
    question_list qstns;
    qstns.head = malloc(sizeof(question_node));
    qstns.last = malloc(sizeof(question_node));
    
    init_qstn_list(&qstns);
    load_questions(&qstns);

    printf("MLSZ kérdéssor összeállító\n");
    menu_run(&qstns);

    return 0;
}
