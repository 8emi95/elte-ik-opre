#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>

#include "projhead.h"


struct Kerdes kerdes1;
struct Kerdes kerdes2;
struct Kerdes kerdes3;

void kerdezobiztos_main(int PtoC[], int CtoP[]);

void kerdezobiztos_main(int PtoC[], int CtoP[]) 
{
    kill(getppid(), SIGUSR1);
    close(PtoC[1]);
    close(CtoP[0]);

   
   read(PtoC[0], &kerdes1, KerdesSize);
   read(PtoC[0], &kerdes2, KerdesSize);
   read(PtoC[0], &kerdes3, KerdesSize);

    close(PtoC[0]);
        
    int j;
    for(j=0;j<kerdes1->valaszSzam;++j)
        kerdes1->eredmeny[j] = rand()%11+10;
    write(CtoP[1], &kerdes1, KerdesSize);
    for(j=0;j<kerdes2->valaszSzam;++j)
        kerdes2->eredmeny[j] = rand()%11+10;
    write(CtoP[1], &kerdes2, KerdesSize);
    for(j=0;j<kerdes3->valaszSzam;++j)
        kerdes3->eredmeny[j] = rand()%11+10;
    write(CtoP[1], &kerdes3, KerdesSize);

    close(CtoP[1]);
}