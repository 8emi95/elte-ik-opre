#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void handler(int signumber);

int countLines(char *filename);

int main(int argc, char *argv[])
{

    signal(SIGTERM,handler);
    int status;
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
            char buf[100];
            snprintf(buf, 100, "%d", r);
            /* Csovezeteken keresztul atkuldom az azonositot */
            printf("Szulo indul!\n");
            //close(pipefd[0]); //Usually we close unused read end
            write(pipefd[1], buf, sizeof(buf));
            //close(pipefd[1]); // Closing write descriptor 
            //printf("Beirtuk a random generalt kerdest!%s\n", pipefd[1]);
            ++j;
            kill(child, SIGTERM);
            pause();
            
        }
        waitpid(child, &status, 0); 
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
        int kerdesek[3];
        
        for(i=0; i<3; ++i)
        {
            kerdesek[i]=atoi(buffer[i]);
            printf("A kapott kerdesek: %i\n", kerdesek[i]);
        }
        FILE *fp1 = fopen("fajl.txt", "r");
        FILE *fp2 = fopen("kerdesek.txt", "w");
        char place2[100];
        int db=0;
        if(fp1 == NULL)
        {
        printf("Error!");
        exit(1);
        }
        char place[100];
        int valaszlehetosegek[3];
        while(fgets(place2, sizeof(place2), fp1))
        {
           /*if(db==kerdesek[0])
           {
               fprintf(fp2, "%s", place2);
           }*/
           fgets(place2, sizeof(place2), fp1);
           if(db==kerdesek[0])
           {
               //fprintf(fp2, "%s", place2);
               valaszlehetosegek[0]=atoi(place2);
           }

            int x = atoi(place2)+2;
            while(x>0 && fgets(place2, sizeof(place2), fp1))
            {
                /*if(db==kerdesek[0])
                {
                    fprintf(fp2, "%s", place2);
                }*/
                --x;
            }
            db++;
       }
       fclose(fp1);
       srand(time(0));
       int citizans=rand()%11+10;
       printf("%i\n",citizans);
       printf("%i\n",valaszlehetosegek[0]);
       int l;
       int elsovalaszok[20];
       for(l=0;l<citizans;++l)
       {
           elsovalaszok[l]=0;
           //printf("%i\n",elsovalaszok[l]);
       }

       for(l=0;l<citizans;++l)
       {
           elsovalaszok[l]=(rand()%valaszlehetosegek[0]);
           ++elsovalaszok[l];
           //printf("%i\n",(rand()%valaszlehetosegek[0])+1);

           printf("%i\n",elsovalaszok[l]);
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