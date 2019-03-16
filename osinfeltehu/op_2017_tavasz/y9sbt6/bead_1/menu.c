#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "menu.h"
#include "persistence.h"

void menu_print(void)
{
    printf(VERT_SPACER);
    printf(" 1 Felvétel\n");
    printf(" 2 Módosítás\n");
    printf(" 3 Törlés\n");
    printf(" 4 Listázás\n");
    printf(" 5 Kérdéssor összeállítása\n");
    printf(" 6 Kérdéssor listázása\n");
    printf(" 7 Mentés\n");
    printf(" q Kilépés\n\n");
    printf(" opció: ");
}

void menu_run(question_list* list)
{
    int quit = 0;
    
    while (!quit)
    {
        menu_print();
        char choice;
        scanf("%c", &choice);
        while (getchar() != '\n');
        printf("\n");
        
        switch (choice)
        {
            case '1':
                menu_add_qstn(list);
                break;
            case '2':
                menu_modify_qstn(list);
                break;
            case '3':
                menu_remove_qstn(list);
                break;
            case '4':
                menu_print_qstns(list);
                break;
            case '5':
                menu_compose_list(list);
                break;
            case '6':
                menu_print_composed_list(list);
                break;
            case '7':
                menu_save(list);
                break;
            case 'q':
                quit = 1;
                break;    
        }
    }
}

void menu_read_qstn_text(question* qstn)
{
    printf("új kérdés: ");
    fgets(qstn->text, sizeof(qstn->text), stdin);
    qstn->text[strlen(qstn->text) - 1] = '\0'; // removes new line character
}

void menu_read_answ_num(question* qstn, const size_t min, const size_t max)
{
    int out_of_range;
    do
    {
        printf("válaszok száma %zd és %zd között : ", min, max);
        scanf("%zd", &(qstn->answer_num));
        getchar();
        out_of_range = (qstn->answer_num < min) || (qstn->answer_num > max);
    } while (out_of_range);
}

void menu_read_answers(question* qstn)
{
    size_t i;
    for (i = 0; i < qstn->answer_num; ++i)
    {
        printf("válasz %zd: ", i + 1);
        fgets(qstn->answers[i], sizeof(qstn->answers[i]), stdin);
        int length = strlen(qstn->answers[i]);
        qstn->answers[i][length - 1] = '\0'; // removes new line character
    }
}

int menu_select_qstns(question_list* list, int modification)
{
    size_t ids[100];
    char buff[310];
    printf("Add meg a módosítandó kérdések sorsszámait szóközzel elválasztva!\nsorszámok: ");
    fgets(buff, sizeof(buff), stdin);

    // convert string ids in buff into size_t nums and load them into ids
    char* begin = buff;
    char* end;
    size_t curr_id = strtoul(begin, &end, 10);
    int i = 0;
    
    while (begin != end)
    {
        ids[i] = curr_id;
        begin = end;
        ++i;
        curr_id = strtoul(begin, &end, 10);
    }

    int id_count = i;
    bubble_sort(ids, id_count);    
    rem_dupl(ids, &id_count);
    i = 0;
    int modified_num = 0;
    question_node* node = list->head->next;

    // modifying questnion.selected
    while ((node != list->last) && (node->qstn.id <= ids[i]))
    {
        if (node->qstn.id == ids[i])
        {
            if (node->qstn.selected != modification)
            {
                node->qstn.selected = modification;
                ++modified_num;
            }            

            if (i + 1 < id_count)
            {
                ++i;
            }
        }

        node = node->next;
    }
    
    return modified_num;
}

void bubble_sort(size_t* a, int len)
{
    int i, j;
    for (i = 0; i < len - 1; ++i)
    {
        for (j = 0; j < len - 1; ++j)
        {
            if (a[j + 1] < a[j])
            {
                size_t tmp = a[j + 1];
                a[j + 1] = a[j];
                a[j] = tmp;
            }
        }
    }
}

void rem_dupl(size_t* a, int* len)
{
    int new_len = 0;
    int i = 0;
    int j = 0;

    while (j < *len)
    {
        a[i] = a[j];

        while ((j < *len - 1) && (a[j] == a[j + 1]))
        {
            j++;
        }

        new_len++;
        ++i;
        ++j;
    }

    *len = new_len;
}

void menu_add_qstn(question_list* list)
{  
    question qstn;
    clear_answers(&qstn);

    menu_read_qstn_text(&qstn);
    menu_read_answ_num(&qstn, MIN_ANSWER_NUM, MAX_ANSWER_NUM);
    menu_read_answers(&qstn);

    qstn.registered = time(NULL);
    qstn.selected = QSTN_NOT_SELECTED;
    insert_qstn(list, &qstn);
}

void menu_modify_qstn(question_list* list)
{
    size_t id;
    printf("Kérdés módosítása\n");
    printf("kérdés azon: ");
    scanf("%zd", &id);
    getchar();
    question* qstn_ptr = find_qstn(list, id);

    if (qstn_ptr != NULL)
    {
        print_qstn(qstn_ptr);
        printf("\nszöveg: 1, válaszok száma: 2, válaszok: 3, minden: 4\n");
        char choice;
        printf("opció: ");
        scanf("%c", &choice);
        getchar();

        switch (choice)
        {
            case '1':
                menu_read_qstn_text(qstn_ptr);
                break;
            case '2':
                menu_read_answ_num(qstn_ptr, MIN_ANSWER_NUM, MAX_ANSWER_NUM);
                break;
            case '3':
                menu_read_answers(qstn_ptr);
                break;
            case '4':
                menu_read_qstn_text(qstn_ptr);
                menu_read_answ_num(qstn_ptr, MIN_ANSWER_NUM, MAX_ANSWER_NUM);
                menu_read_answers(qstn_ptr);
                break;        
        }
    }
    else
    {
        printf("Nincs ilyen azonosítójú kérdés!\n");
    }
}

void menu_remove_qstn(question_list* list)
{
    size_t id;
    printf("Kérdés törlése\n");
    printf("kérdés azon: ");
    scanf("%zd", &id);
    getchar();

    question* qstn_ptr = find_qstn(list, id);
    if (qstn_ptr != NULL)
    {
        print_qstn(qstn_ptr);
        printf("\nBiztosan törölni akarod? (i/n): ");
        char choice;
        scanf("%c", &choice);
        getchar();

        if (choice == 'i')
        {
            if (remove_qstn(list, id))
            {
                printf("sikeres törlés\n");
            }
            else
            {
                printf("sikertelen törlés\n");
            }
        }      
    }
    else
    {
        printf("Nincs ilyen kérdés!\n");
    }
}

void menu_print_qstns(const question_list* list)
{
    printf("Kérdések listája: \n\n");

    if (list->head->next == list->last)
    {
        printf("nincsenek kérdések\n");
        return;
    }
    
    question_node* node = list->head->next;
    while (node != list->last)
    {
        print_qstn(&(node->qstn));
        node = node->next;
    }
}

void menu_compose_list(question_list* list)
{
    printf("Kérdéssor összeállítása\n");
    printf("Hozzáadni vagy eltávolítani szeretnél? (h/e): ");

    int choice;
    do
    {
        choice = getchar();
        getchar();
    } while (choice != 'h' && choice != 'e');

    int sel_num = 0;

    switch (choice)
    {
        case 'h':
            sel_num = menu_select_qstns(list, QSTN_SELECTED);
            break;
        case 'e':
            sel_num =  menu_select_qstns(list, QSTN_NOT_SELECTED);
            break;
    }

    printf("%i kérdés módosult.\n", sel_num);
}

void menu_print_composed_list(const question_list* list)
{
    printf("Kérdéssorban szereplő kérdések:\n\n");
    question_node* node = list->head->next;
    int num = 0;

    while (node != list->last)
    {
        if (node->qstn.selected)
        {
            print_qstn(&(node->qstn));
            ++num;
        }

        node = node->next;
    }

    if (num == 0)
    {
        printf("A kérdéssor üres.\n");
    }
}

void menu_save(const question_list* list)
{
    int choice;
    do
    {
        printf("Biztosan el akarod menteni? (i/n): ");
        choice = getchar();
        getchar();
    } while (choice != 'i' && choice != 'n');

    if (choice == 'i')
    {
        if (save_questions(list))
        {
            printf("Mentés sikeres.\n");
        }
        else
        {
            printf("Mentés sikertelen!\n");
        }
    }
}
