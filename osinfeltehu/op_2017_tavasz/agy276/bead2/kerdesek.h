#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifndef KERDESEK_H_
#define KERDESEK_H_

typedef struct Answer{
    char *answer;
    int id;
    int number_of_votes;
}Answer;

typedef struct Question{
    char *question;
    Answer *answer_array;
    int id;
    char date[25];
    size_t answer_size;
    size_t answer_used_size;
}Question;

typedef struct Array{
    Question *question_array;
    size_t size;
    size_t used_size;
}Array;

void initArray(Array*, size_t);
void addElement(Array*, char*);
void editElement(Array*, int);
void deleteElement(Array*,int);
void listElements(Array*);
void menu(int);
int dont();
Array createQuestions(Array*);
Array kerdesek_veglegesitese(Array*);

#endif //KERDESEK_H_