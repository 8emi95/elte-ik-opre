#include <stdio.h>
#include <stdlib.h>
#include <poll.h> // poll
#include <errno.h>
#include <fcntl.h>  //O_RDONLY,
#include <unistd.h> //close
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h> //rand
#include <signal.h>
#include <string.h>
#include <time.h> //ctime


typedef int bool;
#define true 1
#define false 0

struct Personstr
{
    char name[500];
    char email[500];
    time_t time;
};
typedef struct Personstr Person;

struct Eventstr
{
    char name[500];
    int id;
    int guestCount;
    Person guests[1000];
};
typedef struct Eventstr Event;

struct Databasestr
{
    int eventCounter;
    Event events[1000];  
};
typedef struct Databasestr Database;

Database datab;

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
void Inic()
{
    datab.eventCounter = 10;
    int i;
    for ( i=0; i < datab.eventCounter; ++i )
    {
        char iStr[10];
        sprintf(iStr, "%d", i);
        strcpy(datab.events[i].name, concat("Konferencia ", iStr) ); // + i ); // "Konferencia";
        datab.events[i].id = i;
        datab.events[i].guestCount = (i+1) * 10;
        int j;
        for( j=0; j < datab.events[i].guestCount; ++j )
        {
            char jStr[10];
            sprintf(jStr, "%d", j);
            char jiStr[20];
            strcpy(jiStr, concat(iStr, jStr) ); 
            strcpy(datab.events[i].guests[j].name, concat("Vendeg", jiStr )   ); // + i + j ); // = 'V'; // "Vendeg";
            strcpy(datab.events[i].guests[j].email, "valami@freemail.hu"); // + i + j); // + "@freemail.hu");// = 'S'; //" Valami@gmail.com";
            datab.events[i].guests[j].time = time(NULL); 
        }
    }
}

void Load()
{
    FILE* f;
    f = fopen("jelentkezesek.txt", "r");

    if (f == NULL){
        printf("Hiba a betoltesnel");
        return;
    }

    fread( &datab.eventCounter, sizeof(int), 1, f);
    int i;
    for (i = 0; i < datab.eventCounter; i++) {
        fread( &datab.events[i].name,  sizeof(char) , strlen(datab.events[i].name) , f); 
        fread( &datab.events[i].id, sizeof(int), 1, f);      
        fread( &datab.events[i].guestCount, sizeof(int), 1, f);
        int j;
        for( j = 0; j < datab.events[i].guestCount; ++j )
        {
            fread( &datab.events[i].guests[j].name, sizeof( char) ,strlen(datab.events[i].guests[j].name) , f);
            fread( &datab.events[i].guests[j].email, sizeof( char ) ,strlen(datab.events[i].guests[j].email) , f);
            fread(&datab.events[i].guests[j].time , sizeof(time_t), 1, f);
        }
    }

}


void handler()
{
    printf("Valaki szignált küldött \n");
}

void KingsOfStands( int esemenyId)
{
    //Aktuálisan vizsgált esemény
    Event aktEv = datab.events[esemenyId];

    //Névcső létrehozása a helyszínküldéshez
    int fd;

    int nevcso = mkfifo( "pipe.pp", S_IRUSR|S_IWUSR );
    
    //Szignál
    signal(SIGTERM,handler);

    //pipe a vendégnevek küldéséhez
    int pipefd[2];

    //pipe a hiányzó vendégnevek küldéséhez
    int pipefdtwo[2];


        if (pipe(pipefd) == -1) 
	    {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
        }

	
        if (pipe(pipefdtwo) == -1) 
	    {
               perror("Hiba a pipetwo nyitaskor!");
               exit(EXIT_FAILURE);
        }



        pid_t  partner = fork();
        
        if ( partner == -1 )
        {
            perror("Fork hiba");
            exit(EXIT_FAILURE);
        }
        //Szulo
        if (partner > 0)
        { 
            printf("Csonyitas eredmenye: %d!\n",nevcso);
                fd=open("pipe.pp",O_WRONLY);
                write(fd,"Budapest, ELTE Gömbaula ", 25);
                close(fd);
            pause(); 
            printf("Partner megerkezett \n",SIGTERM);

            //Rendzvény résztvevőinek átküldése
            char aktResztvevok[1000][500];
            int m;
            for( m = 0; m < aktEv.guestCount; ++m)
            {
                strcpy( aktResztvevok[m] , aktEv.guests[m].name);
            }

  /*          for( m = 0; m < aktEv.guestCount; ++m)
            {
                printf("Resztv: %s \n" , aktResztvevok[m] );
            }
*/
            close(pipefd[0]); //Usually we close unused read end
            write(pipefd[1], &aktResztvevok, sizeof(aktResztvevok));
            close(pipefd[1]); // Closing write descriptor 
            printf("Szulo beirta az adatokat a csobe!\n");
            fflush(NULL); 	// flushes all write buffers (not necessary)
            //wait();		// waiting for child process (not necessary)

            pause(); 

            printf("[Szulo] esemeny vege!\n");
            int siker;
            //char sikerStr[10];
            fd = open("pipe.pp",O_RDONLY);
            //printf("Csonyitas eredmenye: %d!\n",sikercso);
	        read(fd, &siker,  sizeof(siker));
	        printf("[Szulo] Az esemeny sikeressege: %d \n",siker);
	        close(fd);


            //Hianyzo vendegek szama
            int nemJottElDb;
            sleep(3);
            //char sikerStr[10];
            fd = open("pipe.pp",O_RDONLY);
            //printf("Csonyitas eredmenye: %d!\n",sikercso);
	        read(fd, &nemJottElDb,  sizeof(nemJottElDb));
	        printf("[Szulo] Nem jott el: %d \n",nemJottElDb);
	        close(fd);
            pause();



            char hianyzoVendegek[1000][500];            

            close(pipefdtwo[1]);
            printf("[Szulo] nevek jonnek!\n");
            read(pipefdtwo[0],&hianyzoVendegek,sizeof(hianyzoVendegek));
            close(pipefdtwo[0]);

            printf("Szervezo megkapta a hianyzok listajat, ami a kovetkezo:  \n");
            int k;
            for(k=0; k < nemJottElDb; k++ )
            {
                printf("Nev (%i):  %s \n", k, hianyzoVendegek[k]);
            }
               
            if ( nemJottElDb < 0)
            {
                printf("Mindenki eljott");
            }
            else
            {
                for ( m = 0; m <= nemJottElDb; m++)
                {
                    printf("Nev:  %s \n ", hianyzoVendegek[m]);
                }
            }

	        printf("\n");
            int status;
            wait(&status);
            printf("Rendezveny befelyezodott\n");
        }
        else if ( partner == 0)
        {
            char s[1024];		
	        printf("Csonyitas eredmenye partnerben: %d!\n",nevcso);
	        fd = open("pipe.pp",O_RDONLY);
	        read(fd,s,sizeof(s));
	        printf("Ide kell utaznom: %s \n",s);
	        close(fd);
            printf("Travel 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
            sleep(3);
            kill(getppid(),SIGTERM); 

            char aktVendegek[1000][500];

            char hianyzoVendegek[1000][500];            
           // sleep(3);	// sleeping a few seconds, not necessary
               close(pipefd[1]);  //Usually we close the unused write end
	    //   printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd[0],&aktVendegek,sizeof(aktVendegek));
               printf("Partner megkapta a vendeglistat, ami a kovetkezo:  \n");
               
                int nemJottElDb;
                nemJottElDb = -1;
               int m;
               for ( m = 0; m < aktEv.guestCount; m++)
               {
                   printf("  Nev:  %s \n", aktVendegek[m]);
                   int ittVan = rand() % 100;
                   if( ittVan < 100 )
                   {
                       nemJottElDb ++ ;
                       printf("(     Nem jott el :( ) \n");
                       strcpy( hianyzoVendegek[nemJottElDb], aktVendegek[m]);
                   }
               }
	       printf("\n");
               close(pipefd[0]);

            printf("Travel 3 seconds, then send a SIGTERM %i signal\n",SIGTERM);
            sleep(3);
            kill(getppid(),SIGTERM); 

            //Sikeresseg visszakuldese
            int siker = rand() % 101;
                fd=open("pipe.pp",O_WRONLY);
                write(fd, &siker, sizeof(siker));
            close(fd);

            sleep(1);
            //Hianyzottak szamanak visszakuldese
                fd=open("pipe.pp",O_WRONLY);
                write(fd, &nemJottElDb, sizeof(nemJottElDb));
            close(fd);

            sleep(3);
            int k;
               //for(k=0; k < nemJottElDb; k++ )
              // {
              //      printf("Nev:  %s \n ", hianyzoVendegek[k]);
              // }
               
            printf("Menj\n");
            close(pipefdtwo[0]); 
            if (nemJottElDb > 0)
                write(pipefdtwo[1], &hianyzoVendegek, nemJottElDb*sizeof(hianyzoVendegek[0]));
            printf(" Fenebe\n");
            close(pipefdtwo[1]);
            printf(" Idiota\n");
            
            kill(getppid(),SIGTERM); 

            sleep(2);
            printf("Partner process ended\n");  
        }
        unlink("pipe.pp");
        exit(EXIT_SUCCESS);
    }    
//}


int main()
{
    //Load();
    Inic();
    KingsOfStands( 0 );

    return(0);
}