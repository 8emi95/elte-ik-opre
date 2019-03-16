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
    char *result = malloc(strlen(s1)+strlen(s2)+1);
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
        strcpy(datab.events[i].name, concat("Konferencia ", iStr) ); 
        datab.events[i].id = i;
        datab.events[i].guestCount = (i+1) * 10;
        int j;
        for( j=0; j < datab.events[i].guestCount; ++j )
        {
            char jStr[10];
            sprintf(jStr, "%d", j);
            char jiStr[20];
            strcpy(jiStr, concat(iStr, jStr) ); 
            strcpy(datab.events[i].guests[j].name, concat("Vendeg", jiStr )   ); 
            strcpy(datab.events[i].guests[j].email, "valami@freemail.hu"); 
            datab.events[i].guests[j].time = time(NULL); 
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

    printf("\n AKTUALIS RENDEZVENY: %s \n \n", aktEv.name);
    sleep(1);
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
          //  printf("Csonyitas eredmenye: %d!\n",nevcso);
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

            close(pipefd[0]); 
            write(pipefd[1], &aktResztvevok, sizeof(aktResztvevok));
            close(pipefd[1]);  
           // printf("Szulo beirta az adatokat a csobe!\n");
            fflush(NULL); 	

            pause(); 

            //Sikeresség
            int siker;
            fd = open("pipe.pp",O_RDONLY);
	        read(fd, &siker,  sizeof(siker));
	        printf("Az esemeny sikeressege: %d \n",siker);
	        close(fd);

            //Hianyzo vendegek szama
            int nemJottElDb;
            sleep(3);
            fd = open("pipe.pp",O_RDONLY);
	        read(fd, &nemJottElDb,  sizeof(nemJottElDb));
	        printf("Nem jott el: %d \n",nemJottElDb+1);
	        close(fd);
            pause();

            //Nem megjelent vendégek
            char hianyzoVendegek[1000][500];            

            close(pipefdtwo[1]);
            read(pipefdtwo[0],&hianyzoVendegek,sizeof(hianyzoVendegek));
            close(pipefdtwo[0]);

            printf("Szervezo megkapta a hianyzok listajat, ami a kovetkezo:  \n");
               
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
            //Helyszínre utazás
            char s[1024];		
	      //  printf("Csonyitas eredmenye partnerben: %d!\n",nevcso);
	        fd = open("pipe.pp",O_RDONLY);
	        read(fd,s,sizeof(s));
	        printf("Ide kell utaznom: %s \n",s);
	        close(fd);
            printf("Travel 3 seconds, then send a signal\n",SIGTERM);
            sleep(3);
            kill(getppid(),SIGTERM); 

            //Vendéglista
            char aktVendegek[1000][500];
               close(pipefd[1]);  
               read(pipefd[0],&aktVendegek,sizeof(aktVendegek));
               printf("Partner megkapta a vendeglistat, ami a kovetkezo:  \n");

            //Hiányzó vendégek       
                char hianyzoVendegek[1000][500]; 
                int nemJottElDb;
                nemJottElDb = -1;
               int m;
               for ( m = 0; m < aktEv.guestCount; m++)
               {
                   printf("  Nev:  %s \n", aktVendegek[m]);
                   int ittVan = rand() % 100;
                   if( ittVan < 10 )
                   {
                       nemJottElDb ++ ;
                       printf("            (Nem jott el :( ) \n");
                       strcpy( hianyzoVendegek[nemJottElDb], aktVendegek[m]);
                   }
               }
	        printf("\n");
            close(pipefd[0]);

            printf("Sleep 3 sec. \n",SIGTERM);
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

            //Hiányzók nevének visszaküldése
            close(pipefdtwo[0]); 
            if (nemJottElDb > 0)
            write(pipefdtwo[1], &hianyzoVendegek, (1 + nemJottElDb)*sizeof(hianyzoVendegek[0]));
            close(pipefdtwo[1]);
            kill(getppid(),SIGTERM); 

            sleep(2);
            printf("Partner process ended\n");  
        }
        unlink("pipe.pp");
}    


int main()
{
    Inic();
    int i;
    for ( i=0; i < datab.eventCounter; ++i)
    {
        KingsOfStands( i );
    }
    printf ("Minden esemenyt lebonyolitottunk \n");
    return(0);
}