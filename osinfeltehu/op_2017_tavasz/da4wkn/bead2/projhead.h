#ifndef PROJHEAD_H
#define PROJHEAD_H

#include "kerdezobiztos_main.c"
#include "szulo_main.c"

struct Kerdes
{
    char kerdes[128];
    int valaszSzam;
    char valaszok[4][128];
    int eredmeny[4];
};

size_t KerdesSize = sizeof(struct Kerdes);
//typedef struct Kerdes kerdes_type;


// void kerdezobiztos_main(int[], int[]);
// void szulo_main(int[], int[]);

#endif