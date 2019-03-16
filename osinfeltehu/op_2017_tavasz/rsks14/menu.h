#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdbool.h>



struct menu
{
   int chose;
   int tombszamolo;
};

struct data
{

    char question[145];
    char ans1[144];
    char ans2[144];
    char ans3[144];
    char ans4[144];
    char ido[21];


};

FILE *be_file;
FILE *ki_file;



void run(struct menu, struct data[]);
int dataloading(struct data[], int);
bool kerdes_e(char []);
int kerdes_1(char [], struct data);
void buffervivion(struct data[], int);
void new_kerdes(struct data[], int);
int valasz_e(char []);
void kerdes_modosit( struct data[], int);
int torol(struct data[], int);
void veglegesites(struct data dat[],int teto);


#endif // MENU_H
