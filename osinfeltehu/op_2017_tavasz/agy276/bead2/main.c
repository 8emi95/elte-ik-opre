#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "kerdesek.h"
#define MAXLEN 4096

void createRandomQuestions(Array*, Array*);
void createRandomQuestionsFromFile(Array*);
void askThePeople(Array*);

int main(){
    Array finalized, choosen;
    initArray(&choosen, 3);
    size_t loop, loop2;
    srand(time(NULL));
    char *dontesChar = malloc(sizeof(char));
    pid_t child;
    int parent_pipe[2], child_pipe[2]/*, p_pipe = pipe(parent_pipe), c_pipe = pipe(child_pipe)*/, status;
    //char buf[MAXLEN];
    
    pipe(parent_pipe);
    pipe(child_pipe);
    child = fork();
    if(child < 0){
        perror("Fork error...\n");
        exit(-1);
    }else if(child > 0){    //Parent
        printf("Starting parent.\n");
        close(child_pipe[1]);
        close(parent_pipe[0]);
        printf("Új kérdéssor(1), vagy fájlból?\n");
        dontesChar = fgets(dontesChar, sizeof(dontesChar), stdin);
        int dontes = (int) *dontesChar - '0';
        if(dontes == 1){
            initArray(&finalized, 1);
            finalized = createQuestions(&finalized);        
        }else{
            initArray(&finalized, 3);
            createRandomQuestionsFromFile(&finalized);
            finalized.question_array[0].answer_size--; //fixing reading error
        }
        char numbers[15];
        printf("Write to pipe... \n");
        for(loop = 0; loop < 3; loop++){
            write(parent_pipe[1], finalized.question_array[loop].question, MAXLEN);
            sprintf(numbers, "%zu", finalized.question_array[loop].answer_size);
            write(parent_pipe[1], numbers, MAXLEN);
            for(loop2 = 0; loop2 < finalized.question_array[loop].answer_size; loop2++){
                write(parent_pipe[1], finalized.question_array[loop].answer_array[loop2].answer, MAXLEN);
            }
        }
      
        close(parent_pipe[1]);
        printf("Parent close pipe...\n");
        wait(&status);
        //char element[MAXLEN];
        printf("Parent reading..\n");
        int szavazatok, reading_number_q = 0, reading_number_a = 0;
        while((loop = read(child_pipe[0], &szavazatok, sizeof(int)*2)) > 0){
            finalized.question_array[reading_number_q].answer_array[reading_number_a].number_of_votes = szavazatok;
            reading_number_a++;
            if(reading_number_a == finalized.question_array[reading_number_q].answer_used_size){
                reading_number_q++;
                reading_number_a = 0;
            }
        }
        wait(&status);
        printf("Parent finished reading...\n");
        close(child_pipe[0]);
        for(loop = 0; loop < 3; loop++){
            printf("A(z) %zu. végleges kérdés: %s\n", loop+1, finalized.question_array[loop].question);
            for(loop2 = 0; loop2 < finalized.question_array[loop].answer_used_size; loop2++){
                printf("\tA(z) %zu. válasz: %s \t A szavazatok száma: %i\n", loop2+1, finalized.question_array[loop].answer_array[loop2].answer, finalized.question_array[loop].answer_array[loop2].number_of_votes);
            }
        }
        printf("Parent finished.\n");
    }else{  //Child
        printf("Child started...\n");
        close(parent_pipe[1]);
        close(child_pipe[0]);
        Array childArray;
        initArray(&childArray, 3);
        printf("Child reading...\n");
        int reading_loop = 0, reading_answer = 0, ans_nums = 0;
        char element[MAXLEN];

        while((loop = read(parent_pipe[0], element, MAXLEN)) > 0){            
            if(strlen(element) == 1){
                childArray.question_array[reading_loop].answer_size = *element - '0';
                childArray.question_array[reading_loop].answer_used_size = childArray.question_array[reading_loop].answer_used_size - 1;
                childArray.question_array[reading_loop].answer_array = malloc(sizeof(Answer)*(*element - '0'));
                reading_answer = 1;
            }else if(reading_answer == 1 && ans_nums < childArray.question_array[reading_loop].answer_size){
                childArray.question_array[reading_loop].answer_array[ans_nums].answer = (char *)realloc(NULL, sizeof(char)*MAXLEN);
                strcpy(childArray.question_array[reading_loop].answer_array[ans_nums].answer, element);
                ans_nums++;
            }
            else{
                childArray.question_array[reading_loop].question = (char *)malloc(sizeof(char)*MAXLEN);
                strcpy(childArray.question_array[reading_loop].question, element);
            }
            if(ans_nums == childArray.question_array[reading_loop].answer_size && reading_answer == 1){
                reading_loop++;
                reading_answer = 0;
                ans_nums = 0;
            }
        }
        close(parent_pipe[0]);
        askThePeople(&childArray);
        
        int szavazatok;
        printf("Children writing back...\n");
        for(loop = 0; loop < 3; loop++){
            for(loop2 = 0; loop2 < childArray.question_array[loop].answer_size; loop2++){
                szavazatok = childArray.question_array[loop].answer_array[loop2].number_of_votes;
                write(child_pipe[1], &szavazatok, sizeof(int)*2);
            }
        }
        
        close(child_pipe[1]);
        wait(&status);
        //printf("child closed pipe....\n");
        
        printf("Children finished.\n");
    }

    
    return 0;
}

void createRandomQuestions(Array *from, Array *to){
    int loop, loop2, random[3];
    for(loop = 0; loop < 3; loop++){
        random[loop] = (rand() % from->used_size) + 1;
        CHECK:for(loop2 = 0; loop2 < loop; loop2++){
            if(random[loop] == random[loop2]){
                random[loop] = (rand() % from->used_size) + 1;
                goto CHECK;
            }
        }
    }
    for(loop = 0; loop < 3; loop++){        
        to->question_array[loop].answer_array = realloc(NULL, sizeof(Answer)*from->question_array[random[loop]].answer_size);
        to->question_array[loop] = from->question_array[random[loop]];
        to->used_size++;
    }
}

void createRandomQuestionsFromFile(Array *a){
    FILE *file;
    file = fopen("vegleges_temp.txt", "r");
    char *beolvas, sor[MAXLEN];
    int random[3], loop, loop2, talalat = 0, valasz=0, locale_id = 0;
    for(loop = 0; loop < 3; loop++){
        random[loop] = (rand() % 8) + 1;
        CHECK:for(loop2 = 0; loop2 < loop; loop2++){
            if(random[loop] == random[loop2]){
                random[loop] = (rand() % 8) + 1;
                goto CHECK;
            }
        }
    }
    for(loop = 0; loop < 3; loop++){
        while(fgets(sor, MAXLEN, file) != NULL){
            if(sor[0] == 'Q'){
                
                talalat = 0;
                valasz = 0;
                locale_id = sor[2] - '0';
                for(loop2 = 0; loop2 < 3; loop2++){
                    if(random[loop2] == locale_id){
                        talalat = 1;
                        break;
                    }
                }
                    if(talalat == 1){
                        if(loop > 0){
                            a->used_size++;
                        }
                        talalat = 1;
                        beolvas = realloc(NULL, sizeof(char)*MAXLEN);
                        strncpy(beolvas, sor+5, strlen(sor)-5);
                        if(beolvas != NULL){                            
                            a->question_array[a->used_size].question = realloc(NULL, sizeof(char)*strlen(beolvas));
                            strcpy(a->question_array[a->used_size].question, beolvas);
                        }
                        loop++;
                    }
            }
            if(talalat == 1){
                if(sor[0] == 'D'){                    
                    beolvas = realloc(NULL, sizeof(char)*strlen(sor)-6);
                    strncpy(beolvas, sor+6, strlen(sor));
                    strcpy(a->question_array[a->used_size].date, beolvas);
                }
                else if(sor[1] == 'A'){
                    beolvas = realloc(NULL, sizeof(char)*strlen(sor)-5);
                    strncpy(beolvas, sor+6, strlen(sor)-5);
                    a->question_array[a->used_size].answer_array = (Answer *)realloc(a->question_array[a->used_size].answer_array, sizeof(Answer)*valasz+1);
                    a->question_array[a->used_size].answer_array[valasz].answer = realloc(NULL, strlen(beolvas));
                    strcpy(a->question_array[a->used_size].answer_array[valasz].answer, beolvas);
                    a->question_array[a->used_size].answer_array[valasz].number_of_votes = 0;
                    valasz++;
                    a->question_array[a->used_size].answer_size++;
                    a->question_array[a->used_size].answer_used_size++;
                }
            }            
        }
       
    }
}

void askThePeople(Array *a){
    int number_of_people = ((rand() % 10) + 1) + 10, loop, loop3, choice;
    for(loop = 0; loop < 3; loop++){
            for(loop3 = 0; loop3 < number_of_people; loop3++){
                choice = (rand() % a->question_array[loop].answer_size);
                a->question_array[loop].answer_array[choice].number_of_votes++;
            }
    }
}