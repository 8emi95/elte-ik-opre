#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "kerdesek.h"
#define MAXLEN 4096

int dontes = 0;
time_t raw_date;

void initArray(Array *a, size_t size){
    //int loop;
    a->question_array = (Question *)malloc(sizeof(Question)*size);
    a->size = size;
    a->used_size = 0;
    /*for(loop = 0; loop < size; loop++){
        a->question_array[loop].answer_array = (Answer *)malloc(sizeof(Answer));
        a->question_array[loop].answer_size = 1;
        a->question_array[loop].answer_used_size = 0;
    }*/
    a->question_array[0].answer_array = (Answer *)malloc(sizeof(Answer));
    a->question_array[0].answer_size = 1;
    a->question_array[0].answer_used_size = 0;
    
}

void addElement(Array *a, char *element){
    int number_of_answers = 0;
    raw_date = time(NULL);
    struct tm tm = *localtime(&raw_date);
    if(a->used_size == a->size){
        a->size++;
        a->question_array = realloc(a->question_array, sizeof(Question)*a->size);
    }
    if(strlen(element) != 0){
        a->question_array[a->used_size].question = (char *)malloc(sizeof(char)*strlen(element));                
        a->question_array[a->used_size].question = element;
        sprintf(a->question_array[a->used_size].date, "%d.%d.%d. - %d:%d:%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        printf("Adjnon meg minimum kettő, maximum négy kérdést. Kilépéshez adja meg az '1'-et.\n");
        ANSWERLOOP:while(number_of_answers != 4 || number_of_answers < 2){
            printf("Adja meg a(z) %i. választ: \n", number_of_answers+1);
            char *ans = realloc(NULL, sizeof(char)*MAXLEN);
            fgets(ans, MAXLEN, stdin);
            if(strlen(ans) != 0){
                if((*ans - '0') == 1){
                    if(number_of_answers >= 2){
                        break;
                    }else{
                        printf("Nem adott meg még elég választ.\n");
                        goto ANSWERLOOP;
                    }
                }else{         
                    if(a->question_array[a->used_size].answer_used_size == a->question_array[a->used_size].answer_size){
                        a->question_array[a->used_size].answer_size++;
                        a->question_array[a->used_size].answer_array = (Answer *)realloc(a->question_array[a->used_size].answer_array, a->question_array[a->used_size].answer_size * sizeof(Answer));
                    }
                    a->question_array[a->used_size].answer_array[a->question_array[a->used_size].answer_used_size].answer = (char *)malloc(sizeof(ans));
                    a->question_array[a->used_size].answer_array[a->question_array[a->used_size].answer_used_size].answer = ans;
                    a->question_array[a->used_size].answer_used_size++;
                    number_of_answers++;
                }
            }else{
                printf("Hibásan megadott válasz.\n");
                goto ANSWERLOOP;
            }
        }
    }
    a->used_size++;
}

void editElement(Array *a, int index){
    if(index <= a->used_size || index < 1){
        printf("Adja meg az új kérdést: \n");
        int number_of_answers = 0;
        char *element = realloc(NULL, sizeof(char)*MAXLEN);
        fgets(element, MAXLEN, stdin);
        a->question_array[index-1].question = (char *)realloc(NULL, sizeof(char)*strlen(element));
        a->question_array[index-1].question = element;
        a->question_array[index-1].answer_array = realloc(NULL, sizeof(Answer));
        a->question_array[index-1].answer_size = 1;
        a->question_array[index-1].answer_used_size = 0;
        ANSWERLOOP:while(number_of_answers != 4 || number_of_answers < 2){
            printf("Adja meg a(z) %i. választ: \n", number_of_answers+1);
            char *ans = realloc(NULL, sizeof(char)*MAXLEN);
            fgets(ans, MAXLEN, stdin);
            if((*ans - '0') == 1){
                if(number_of_answers >= 2){
                    break;
                }else{
                    printf("Nem adott meg még elég választ.\n");
                    goto ANSWERLOOP;
                }
            }
            if(a->question_array[index-1].answer_size == a->question_array[index-1].answer_used_size){
                a->question_array[index-1].answer_size++;
                a->question_array[index-1].answer_array = (Answer *)realloc(a->question_array[index-1].answer_array, a->question_array[index-1].answer_size * sizeof(Answer));
            }
            a->question_array[index-1].answer_array[number_of_answers].answer = (char *) realloc(NULL, sizeof(char)*strlen(ans));
            a->question_array[index-1].answer_array[number_of_answers].answer = ans;
            a->question_array[index-1].answer_used_size++;
            number_of_answers++;            
        }
    }else{
        printf("Ilyen indexű kérdés nincs.\n");
    }
}

void deleteElement(Array *a, int index){
    int loop;
    for(loop = index-1; loop < a->used_size; loop++){
        if(a->question_array[loop+1].question != NULL){
            a->question_array[loop] = a->question_array[loop+1];
        }   
    }    
    a->used_size--;
    a->size--;
    a->question_array = realloc(a->question_array, sizeof(Question)*a->size);
}

void listElements(Array *a){
    int loop1,loop2;
    for(loop1 = 0; loop1 < a->used_size; loop1++){
        printf("A(z) %i. kérdés: %s\n", loop1+1, a->question_array[loop1].question);
        printf("Date: %s\n", a->question_array[loop1].date);
        for(loop2 = 0; loop2 < a->question_array[loop1].answer_used_size; loop2++){
            printf("\tA(z) %i. válasz: %s\n", loop2+1, a->question_array[loop1].answer_array[loop2].answer);
        }
    }
}

int dont(){
    char *dontesChar = malloc(sizeof(char));
    dontesChar = fgets(dontesChar, sizeof(dontesChar), stdin);
    dontes = (int) *dontesChar - '0';
    return dontes;
}

Array kerdesek_veglegesitese(Array *a){
    listElements(a);
    printf("Vesszővel elválasztva adja meg melyik kérdéseket akarja megtartani. Csak a valid számok kerülnek kiválasztásra.\n");
    char *dontesek = malloc(MAXLEN);
    Array finalized_array;
    int loop,actual_number,choosen_numbers = 0;
    dontesek = fgets(dontesek, MAXLEN, stdin);
    int numbers[a->used_size];
    for(loop = 0; loop < strlen(dontesek); loop++){
        if(dontesek[loop] != ','){
            actual_number = dontesek[loop] - '0';
            if(actual_number > 0 && actual_number <= a->used_size){
                numbers[choosen_numbers] = actual_number;
                choosen_numbers++;
            }
        }
    }
    printf("Choosen numbers: ");
    for(loop = 0; loop < choosen_numbers; loop++){
        printf("%i\t", numbers[loop]);
    }
    printf("\n");
    initArray(&finalized_array, 1);
    for(loop = 0; loop < choosen_numbers; loop++){
        if(finalized_array.used_size == finalized_array.size){
            finalized_array.size++;
            finalized_array.question_array = realloc(finalized_array.question_array, sizeof(Question)*finalized_array.size);
        }
        finalized_array.question_array[loop] = a->question_array[numbers[loop]-1];
        finalized_array.used_size++;
    }
    return finalized_array;
}

void menu(int dont){
    switch(dont){
        case(1):
            printf("1. Uj kerdessor keszitese. \n");
            printf("2. Kerdessor szerkesztese. \n");
            printf("3. Kerdessor megtekintese. \n");
            printf("4. Kerdessor veglegesitese. \n");
            printf("5. Kilepes. \n");
            break;
        case(2):
            printf("1. Uj kerdes megadasa.\n");
            printf("2. Vissza.\n");
            break;
        case(3):
            printf("1. Kerdes szerkesztese.\n");
            printf("2. Kerdes torlese.\n");
            printf("3. Vissza.\n");
            break;
        case(4):
            printf("1. Vissza.\n");
            break;
        default:
            printf("Nincs ilyen menu.\n");
            break;
    }
}

Array createQuestions(Array *array)
{
    Array a;
    int dontes = 0;
    MAINMENU:menu(1);
    switch(dont()){
        case(1):
            HOZZAD:menu(2);
            switch(dont()){
                case(1):
                    printf("Adja meg a kérdést: \n");
                    char *element = realloc(NULL, sizeof(char)*MAXLEN);
                    fgets(element, MAXLEN, stdin);
                    addElement(array, element);
                    goto HOZZAD;
                    break;
                case(2):
                    goto MAINMENU;
                    break;
                default:
                    printf("Ilyen menupont nincs.");
                    goto HOZZAD;
                    break;
            }
            break;
        case(2):
               listElements(array);
               SZERKESZT: menu(3);
               switch(dont()){
                case(1):
                    printf("Adja meg melyik kerdest szerkesztene.\n");
                    dontes = dont();
                    editElement(array, dontes);
                    goto SZERKESZT;
                    break;
                case(2):{
                    printf("Adja meg melyik kerdest torolne.\n");
                    dontes = dont();
                    printf("Torles balbla.\n");
                    deleteElement(array, dontes);
                    goto SZERKESZT;
                    break;
                }
                case(3):
                    goto MAINMENU;
                    break;
                default:
                    printf("Ilyen menupont nincs.\n");
                    goto SZERKESZT;
                    break;
               }
            break;
        case(3):
            listElements(array);
            LISTA: menu(4);
            switch(dont()){
                case(1):
                    goto MAINMENU;
                    break;
                default:
                    printf("Ilyen menupont nincs.\n");
                    goto LISTA;
                    break;
            }
            break;
        case(4):
            a = kerdesek_veglegesitese(array);
            goto MAINMENU;
            break;
        case(5):
            return a;
            exit(0);
        default:
            printf("Hibas menu.");
            goto MAINMENU;
    }
    /*FILE *tisztitas;
    FILE *tisztitas2;
    tisztitas = fopen("asd.txt", "w");
    tisztitas2 = fopen("temp.txt", "w");
    fclose(tisztitas);
    fclose(tisztitas2);*/
    //return vegeleges_kerdesek_szama;
}