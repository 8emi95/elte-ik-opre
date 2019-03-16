typedef int bool;
#define true 1
#define false 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define MAX_SIZE 256

struct rendezveny
{
    int razon;
    char* helyszin;
};
bool wait = true;
struct rendezveny* rendezvenyek;
const char* vendegek[5] = {"Hegyi Márk", "Gyenes Boglárka", "Széll Vivien", "Bernáth Bettina", "Tímár Gergely"};
int rendezvenynum;
int vendegnum =  sizeof(vendegek)/sizeof(char*);


void Beolvasas()
{
    FILE* fp;
    fp = fopen("rendezvenyek.txt", "r");
    char rnum[MAX_SIZE];
    fgets(rnum,MAX_SIZE, fp);
    if (strlen(rnum) == 0)
    {
        return;
    }
    rendezvenynum = atoi(rnum);
    rendezvenyek = malloc(rendezvenynum*sizeof(struct rendezveny));
    int r;
    char rid[MAX_SIZE];
    char rhely[MAX_SIZE];
    for (r = 0; r < rendezvenynum; r++)
    {
        fgets(rid,MAX_SIZE,fp);
        fgets(rhely,MAX_SIZE,fp);
        rendezvenyek[r].razon = atoi(rid);
        rhely[strlen(rhely)-1] = '\0';
        rendezvenyek[r].helyszin = malloc(strlen(rhely)*sizeof(char)+1);
        strcpy(rendezvenyek[r].helyszin,rhely);
    }
    fclose(fp);
}

void SignalHandler(int signo)
{
    if(signo == SIGINT)
    {
        printf("Partner: Megérkeztem a helyszínre.\n");
    }

}

int main()
{
    srand(time(NULL));
    Beolvasas();

    pid_t   child;
    int     fd[2],fd2[2], nbytes;
    char    readbuffer[MAX_SIZE];
    signal (SIGINT, SignalHandler);





    int i;
    for (i = 0; i < rendezvenynum; i++)
    {
         if(pipe(fd) == -1)
        {
            perror("pipe\n");
            exit(1);
        }
        if(pipe(fd2) == -1)
        {
            perror("pipe\n");
            exit(1);
        }
        if((child = fork()) == -1)
        {
            perror("fork\n");
            exit(1);
        }

        char* cel;

        if (child > 0)
        {
            printf("KoS: Kovetkezo rendezveny: \n");
            printf("Azonosito: %d\n", rendezvenyek[i].razon);
            printf("Helyszin: %s\n", rendezvenyek[i].helyszin);
            close(fd[0]);
            close(fd2[1]);
            write(fd[1], rendezvenyek[i].helyszin, (strlen(rendezvenyek[i].helyszin)+1));
            sleep(2);
            int k;
            for(k = 0; k < vendegnum; ++k)
            {
                write(fd[1], vendegek[k], (strlen(vendegek[k])+1));
                sleep(2);
            }
            printf("KoS: Partner jelentése:\n");
            nbytes = read(fd2[0], readbuffer, sizeof(readbuffer));
            readbuffer[nbytes+1] =  0;
            printf("A rendezveny sikeressege: %s\n", readbuffer);
            int l;
            for (l = 0; l < vendegnum;++l)
            {
                nbytes = read(fd2[0], readbuffer, sizeof(readbuffer));
                readbuffer[nbytes+1] =  0;
                int x = atoi(readbuffer);
                if(x <= 10)
                {
                    printf("%s nem jelent meg.\n", vendegek[l]);
                }

            }
            sleep(1);
            close(fd[0]);
            close(fd2[1]);
        }


        //gyerek
        if (child == 0)
        {
            close(fd[1]);
            close(fd2[0]);
            nbytes = read(fd[0], readbuffer, (sizeof(readbuffer)));
            cel = malloc(strlen(readbuffer)*sizeof(readbuffer)+1);
            readbuffer[nbytes+1] =  0;
            strcpy(cel, readbuffer);
            kill(getppid(),SIGINT);
            sleep(1);
            char* resztvevok[vendegnum];
            printf("Partner: Resztvevok:\n");
            int j;
            for(j = 0; j < vendegnum; ++j)
            {

                 nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
                 resztvevok[j] = malloc(strlen(readbuffer)*sizeof(readbuffer)+1);
                 readbuffer[nbytes+1] =  0;
                 strcpy(resztvevok[j],readbuffer);
                 printf("%s\n",resztvevok[j]);

            }

            srand(time(NULL));
            int siker = rand() % 100;
            char msg[20];
            snprintf(msg,20,"%d", siker);
            write(fd2[1], msg, sizeof(msg));
            sleep(2);

            for (j = 0; j < vendegnum; ++j)
            {
                srand(time(NULL));
                int megjelent = rand() % 100;
                snprintf(msg,20,"%d", megjelent);
                write(fd2[1], msg, sizeof(msg));
                sleep(2);

            }
            close(fd[1]);
            close(fd2[0]);
            exit(0);
        }

        //Szülő


    }

}
