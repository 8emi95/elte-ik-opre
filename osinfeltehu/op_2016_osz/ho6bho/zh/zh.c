#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <wait.h> 

#define MAXSZAM 100

struct uzenet { 
     long tipus;
     int sorszam;
     int maxszam;
     int pote;
};

struct valaszuzenet
{
    long tipus;
    int sorszam;
    char szoveg[1024];
    int siker;
};

int kuld( int uzenetsor, int sor, int max, int pot) 
{ 
     const struct uzenet uz = { 5, sor, max, pot }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(long) + sizeof(sor) + sizeof(max) + sizeof(int), 0 ); 
     if ( status < 0 ) 
          perror("Uzenet nem lett elkuldve"); 
     return 0; 
} 

int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 

     status = msgrcv(uzenetsor, &uz, sizeof(long) + sizeof(int) + sizeof(int), 5, 0 ); 
     
     if ( status < 0 ) 
          perror("Uzenet nem lett elkuldve(fogad)"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %d, MAX: %d\n", uz.tipus, uz.sorszam, uz.maxszam ); 
     return 0; 
} 

//Főprogram
int main(int argc, char* argv[])
{
    key_t kulcs = ftok(argv[0],1);
    srand(time(NULL));
    int jaratok = atoi(argv[2]);


    //Üzenetsor létrehozása
    int uzenetsor;
    uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
    if(uzenetsor < 0)
    {
        perror("Nem sikerult kapcsolatot letesiteni!");
        exit(1);
    }
    //Gyerekfolyamat
    pid_t gyerek = fork();
    //Szülőfolyamat
    int status;
    if(gyerek > 0)
    {
        //Szerelvényszám elküldése
        //srand(time(NULL));
        int maxszam = atoi(argv[1]);
        int szamlalo = 0;
        int potlasok = 0;

        while(szamlalo < jaratok)
        {
            //int r = rand() % maxszam +1;
            kuld(uzenetsor, szamlalo+1, maxszam, 0);
            //wait(NULL);

            //Válasz fogadása
            struct valaszuzenet v; 

            status = msgrcv(uzenetsor, &v, 1024, 5, 0 ); 
        
            if ( status < 0 ) 
            perror("yay."); 

            else
            {
                if(v.siker == 0)
                {
                    ++potlasok;
                    printf( "A kapott valasz kodja: %ld, Szerelveny szama:  %d, Uzenet: %s\n", v.tipus, v.sorszam, v.szoveg ); 
                    kuld(uzenetsor, szamlalo+1, maxszam, 1);
                    //wait(NULL);
                    struct valaszuzenet vv; 

                    status = msgrcv(uzenetsor, &vv, 1024, 5, 0 ); 
        
                    if ( status < 0 ) 
                    perror("Valasz nem erkezett meg."); 
                    else
                    {
                        printf( "A kapott valasz kodja: %ld, Szerelveny szama:  %d, Uzenet: %s\n", v.tipus, v.sorszam, v.szoveg ); 
                    }
                }
                else{
                    printf( "A kapott valasz kodja: %ld, Szerelveny szama:  %d, Uzenet: %s\n", v.tipus, v.sorszam, v.szoveg ); 
                }
            }
            

            ++szamlalo;
        }
          status = msgctl( uzenetsor, IPC_RMID, NULL ); 
            if ( status < 0 ) 
                perror("msgctl"); 
                printf("POTLASOK: %d\n", potlasok);
          kill(gyerek, SIGKILL);
          return 0; 
    }
    //Gyerekfolyamat
    else if(gyerek == 0 )
    {
        int b = 0;
        while (b < jaratok)
        {
            //Üzenet fogadása
            struct uzenet uz; 
            int status; 

            status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); 
        
            if ( status < 0 ) 
            perror("Uzenet nem erkezett meg."); 
            //else
           //printf( "A kapott uzenet kodja: %ld, Szerelveny szama:  %d, MAX: %d\n", uz.tipus, uz.sorszam, uz.maxszam ); 
            
            //Feldolgozás
            int p = rand() % 10;
            
            if(p == 0)
            {
               
                
                if(uz.pote == 0)
                {
                     printf("Itt a(z) %d szamu szerelveny. Utanam meg %d szerelveny kovetkezik, en visziszont nem fogok megerkezni uzemzavar miatt.\n", uz.sorszam, uz.maxszam - uz.sorszam);
                    const struct valaszuzenet v = { 5, uz.sorszam, "Bevontattak uzemzavar miatt!\n",0 };
                    status = msgsnd( uzenetsor, &v, sizeof(long) + sizeof(int) + strlen(v.szoveg) + 1 + sizeof(int), 0 ); 
                    if ( status < 0 ) 
                        perror("Valasz nem lett elkuldve"); 
                }
                else{
                     printf("A(z) %d szamu szerelvenyt helyettesitem. Utanam meg %d szerelveny kovetkezik, de en is elekadtam.\n", uz.sorszam, uz.maxszam - uz.sorszam);
                    const struct valaszuzenet v = { 5, uz.sorszam, "Bevontattak uzemzavar miatt!\n",0 };
                    status = msgsnd( uzenetsor, &v, sizeof(long) + sizeof(int) + strlen(v.szoveg) + 1 + sizeof(int), 0 ); 
                    if ( status < 0 ) 
                        perror("Valasz nem lett elkuldve"); 
                }
            }
            else
            {
                if(uz.pote == 0)
                {
                    printf("Itt a(z) %d szamu szerelveny. Utanam meg %d szerelveny kovetkezik, en pedig epsegben megerkeztem.\n", uz.sorszam, uz.maxszam - uz.sorszam);
                    const struct valaszuzenet v = { 5, uz.sorszam, "Epsegben megerkeztem!\n", 1 };
                    status = msgsnd( uzenetsor, &v, sizeof(long) + sizeof(int) + strlen(v.szoveg) + 1 + sizeof(int), 0 ); 
                    if ( status < 0 ) 
                        perror("Valasz nem lett elkuldve"); 
                }
                else{
                    printf("A(Z) %d szamu szerelvenyt potlom. Utanam meg %d szerelveny kovetkezik, en pedig epsegben megerkeztem.\n", uz.sorszam, uz.maxszam - uz.sorszam);
                    const struct valaszuzenet v = { 5, uz.sorszam, "Epsegben megerkeztem!\n", 1 };
                    status = msgsnd( uzenetsor, &v, sizeof(long) + sizeof(int) + strlen(v.szoveg) + 1 + sizeof(int), 0 ); 
                    if ( status < 0 ) 
                        perror("Valasz nem lett elkuldve"); 
                }
                
                ++b;
            }
        }
        return 0; 

    }

    return 0;
}