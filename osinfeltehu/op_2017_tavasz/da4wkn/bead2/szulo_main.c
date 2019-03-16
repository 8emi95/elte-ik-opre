#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#include "projhead.h"

struct Kerdes  kerdes1;
struct Kerdes  kerdes2;
struct Kerdes  kerdes3;

void szulo_main(int PtoC[], int CtoP[]);

void Kerdesgyartas()
{

    strcpy(kerdes1->kerdes, "Elso kerdes");
    kerdes1.valaszSzam=2;
    strcpy(kerdes1.valaszok[0], "Elso valasz");
    strcpy(kerdes1.valaszok[1], "Masodik valasz");
    
    strcpy(kerdes2.kerdes, "Masodik kerdes");
   kerdes2.valaszSzam=3;
    strcpy(kerdes2.valaszok[0], "Elso valasz");
    strcpy(kerdes2.valaszok[1], "Masodik valasz");
    strcpy(kerdes2.valaszok[2], "Harmadik valasz");
    
    strcpy(kerdes3.kerdes, "Harmadik kerdes");
    kerdes3.valaszSzam=3;
    strcpy(kerdes3.valaszok[0], "Elso valasz");
    strcpy(kerdes3.valaszok[1], "Masodik valasz");
    strcpy(kerdes3.valaszok[2], "Harmadik valasz");
    strcpy(kerdes3.valaszok[3], "Negyedik valasz");
}

void szulo_main(int PtoC[], int CtoP[])
{
    
    
    close(PtoC[0]);
    close(CtoP[1]);

    Kerdesgyartas();

    write(PtoC[1], &kerdes1, sizeof(kerdes1));
    write(PtoC[1], &kerdes2, sizeof(kerdes2));
    write(PtoC[1], &kerdes3, sizeof(kerdes3)]);
    close(PtoC[1]);

    printf("MLSZ: eredmenyek: \n")


    int j;
    read(CtoP[0], &kerdes1, sizeof(kerdes1));
    printf("\t %s \n",  kerdes1.kerdes);
    for(j=0;j<kerdes1.valaszSzam;++j)
        printf("\t \t %i. %s -- %i \n", j, kerdes1.valaszok[j], kerdes1.(eredmeny[j]);
    read(CtoP[0], &kerdes2, sizeof(kerdes2));
    printf("\t %s \n",  kerdes2.kerdes);
    for(j=0;j<kerdes2.valaszSzam;++j)
        printf("\t \t %i. %s -- %i \n", j, kerdes2.valaszok[j], kerdes2.eredmeny[j]);
    read(CtoP[0], &kerdes3, sizeof(kerdes3));
    printf("\t %s \n", kerdes3.kerdes);
    for(j=0;j<kerdes3.valaszSzam;++j)
        printf("\t \t %i. %s -- %i \n", j, kerdes3.valaszok[j], kerdes3.eredmeny[j]);

    close(CtoP[0]);
}