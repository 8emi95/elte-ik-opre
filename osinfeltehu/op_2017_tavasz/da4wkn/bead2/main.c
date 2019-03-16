#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <string.h>


struct Kerdes
{
    char kerdes[128];
    int valaszSzam;
    char valaszok[4][128];
    int eredmeny[4];
};



void handler(int sgn)
{
    
}



void kerdezobiztos_main(int PtoC[], int CtoP[]) 
{
    struct Kerdes kerdes1;
    struct Kerdes kerdes2;
    struct Kerdes kerdes3;

    kill(getppid(), SIGUSR1);
    close(PtoC[1]);
    close(CtoP[0]);

   
   read(PtoC[0], &kerdes1, sizeof(struct Kerdes));
   read(PtoC[0], &kerdes2, sizeof(struct Kerdes));
   read(PtoC[0], &kerdes3, sizeof(struct Kerdes));

    close(PtoC[0]);
        
    int j;
    for(j=0;j<kerdes1.valaszSzam;++j)
        kerdes1.eredmeny[j] = rand()%11+10;
    write(CtoP[1], &kerdes1, sizeof(struct Kerdes));
    for(j=0;j<kerdes2.valaszSzam;++j)
        kerdes2.eredmeny[j] = rand()%11+10;
    write(CtoP[1], &kerdes2, sizeof(struct Kerdes));
    for(j=0;j<kerdes3.valaszSzam;++j)
        kerdes3.eredmeny[j] = rand()%11+10;
    write(CtoP[1], &kerdes3, sizeof(struct Kerdes));

    close(CtoP[1]);
}

void szulo_main(int PtoC[], int CtoP[])
{
    struct Kerdes  kerdes1;
    struct Kerdes  kerdes2;
    struct Kerdes  kerdes3;

    strcpy(kerdes1.kerdes, "Elso kerdes");
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
    
    close(PtoC[0]);
    close(CtoP[1]);

    write(PtoC[1], &kerdes1, sizeof(kerdes1));
    write(PtoC[1], &kerdes2, sizeof(kerdes2));
    write(PtoC[1], &kerdes3, sizeof(kerdes3));
    close(PtoC[1]);

    printf("MLSZ: eredmenyek: \n");


    int j;
    read(CtoP[0], &kerdes1, sizeof(kerdes1));
    printf("\t %s \n",  kerdes1.kerdes);
    for(j=0;j<kerdes1.valaszSzam;++j)
        printf("\t \t %i. %s -- %i \n", j, kerdes1.valaszok[j], kerdes1.eredmeny[j]);
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


int main(int argc, char *argv[])
{
    signal(SIGUSR1, handler);
    time_t t;
    srand(time(&t));

   int PtoC[2]; // 0: read end, 1: write end
   int CtoP[2]; // 0: read end, 1: write end

    pid_t pid;
    pipe(PtoC);
    pipe(CtoP);
    switch(pid=fork()){
        case -1:
            perror("Forkolas sikertelen!");
            exit(1);
        case 0: // gyerek
            printf("KERDEZOBIZTOS: A kerdezobiztos elindult a helyszinre.\n");
            kerdezobiztos_main(PtoC, CtoP);
            printf("KERDEZOBIZTOS: A kerdezobiztos vegzett a munkajaval.\n");
            exit(0);
        default: // szulo
            printf("MLSZ: Az MLSZ megkezdte munkajat.\n");
            szulo_main(PtoC, CtoP);

            int rv;
            waitpid(pid, &rv, 0);
            printf("MLSZ: A kerdezobiztos folyamat visszateresi erteke: %d.\n", WEXITSTATUS(rv));
           
            printf("MLSZ: Az MLSZ vegzett a munkajaval.\n");
            break;
    }
    return 0;
}