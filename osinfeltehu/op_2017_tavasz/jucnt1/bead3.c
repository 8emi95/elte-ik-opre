#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

/*struct QA{
    char kerdes[100];
    char valaszok[4][100];
} QA;*/

void handler(int signumber);

int countLines(char *filename);

int main(int argc, char *argv[])
{

    signal(SIGTERM,handler);

    /* create pipe */
    int pipefd[2];

    if (pipe(pipefd) == -1) 
	{
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    /* create child */
    pid_t child = fork();
    if (child < 0) 
    {
        perror ("fork() error!");
        exit(-1);
    }

    /* parent process */
    if (0 != child)    
    {         
        char place2[100];
            FILE *fp1 = fopen("fajl.txt", "r");
        int db=0;
        if(fp1 == NULL)
        {
        printf("Error!");
        exit(1);
        }
        char place[100];
        while(fgets(place2, sizeof(place2), fp1))
        {
            fgets(place2, sizeof(place2), fp1);
            int x = atoi(place2)+2;
            while(x>0 && fgets(place2, sizeof(place2), fp1))
            {
                --x;
            }
            db++;
       }

        /* Azonositos fajl hossza */
        int hossz = countLines("id.txt") - 1;
        printf("A fajl hossza %i\n", hossz);

        printf("Varok a gyerek bejelentkezesere!\n");
        kill(child, SIGTERM);
        pause();
        printf("A gyerek bejelentkezett, mehetünk!\n");

        /* Random kerdes generalasa */
        srand(time(NULL)); //the starting value of random number generation
        int j = 0;
        while(j<3)
        {
            int r=rand()%db; //number between 0-9
            printf("A random szam: %i\n", r);
        
            /* Megkeresem a random szam alapján a megfelelő sort */
            

            /* Csovezeteken keresztul atkuldom az azonositot */
            printf("Szulo indul!\n");
            //close(pipefd[0]); //Usually we close unused read end
            write(pipefd[1], place, sizeof(place));
            //close(pipefd[1]); // Closing write descriptor 
            //printf("Beirtuk a random generalt kerdest!%s\n", pipefd[1]);
            ++j;
            kill(child, SIGTERM);
            pause();
            
        }
    }

    /* child process */
    else
    {
        pause();
        printf("Sikerult bejelentkezni!\n");
        kill(getppid(), SIGTERM);
        
        char buffer[3][100];

        int j = 0;
        while(j<3)
        {
            pause();
            /* Kiolvasom a csovezetekbol az adatot */
            //close(pipefd[1]);  //Usually we close the unused write end
            printf("Gyerek elkezdi kiolvasni a dolgokat!\n");
            read(pipefd[0],buffer[j],sizeof(buffer[j])); // reading max 100 chars
            printf("Gyerek olvasta uzenet: %s\n\n", buffer[j]);
            ++j;
            kill(getppid(), SIGTERM);
        }
        int i;
        for(i=0; i<3; ++i)
        {
            printf("A kapott kerdesek: %s\n", buffer[i]);
        }
        
    }

    return 0;
}

void handler(int signumber)
{
    //printf("Signal with number %i has arrived\n", signumber);
}

int countLines(char *filename)
{
    FILE *fp = fopen(filename, "r");

    char c;
    int lines = 1;

    while ((c = fgetc(fp)))
    {
        if (c == EOF)
            break;
        if (c == '\n')
        {
            lines++;
        }
    }
    fclose(fp);

    return lines;
}