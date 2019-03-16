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
        char place[200];
        FILE *fp1 = fopen("fajl.txt", "r");
        FILE *fp2 = fopen("id.txt", "w");
        if(fp1 == NULL)
        {
            printf("Error!");
            exit(1);
        }
        while(fgets(place, sizeof(place), fp1)){
            fprintf(fp2, "%s", place);
            fgets(place, sizeof(place), fp1);
            int x = atoi(place)+1;
            if(x>0)
            {
                 while(x>0 && fgets(place, sizeof(place), fp1))
                {
                    memset(place, 0, sizeof(place));
                    --x;
                }
             }
             fgets(place, sizeof(place), fp1);
        }

        fclose(fp1);
        fclose(fp2);

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
            int r=rand()%(hossz); //number between 0-9
            printf("A random szam: %i\n", r);
        
            /* Megkeresem a random szam alapján a megfelelő sort */
            char place2[100];
            FILE *fp3 = fopen("id.txt", "r");
            while(fgets(place2, sizeof(place2), fp3) && r>0){
                r--;
            }
            printf("Az adott sorzamu sor: %s", place2);
            fclose(fp3);

            /* Csovezeteken keresztul atkuldom az azonositot */
            printf("Szulo indul!\n");
            //close(pipefd[0]); //Usually we close unused read end
            write(pipefd[1], place2, sizeof(place2));
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
       
		int valaszok[3];
        FILE *fp1 = fopen("asd.txt", "r"); 
        FILE *fp2 = fopen("id.txt", "w");
        char place[100];
        int igen = 0;
		int k=0;
        while(fgets(place, sizeof(place), fp1)){
            igen=0;
            if(atoi(buffer[0])==atoi(place))
            {
                fprintf(fp2, "%s", place);
                fgets(place, sizeof(place), fp1);
                fprintf(fp2, "%s", place);
                int x = atoi(place)+1;
				valaszok[k]=x-1;
				k++;
                if(x>0)
                {
                    while(x>0 && fgets(place, sizeof(place), fp1))
                    {
                        fprintf(fp2, "%s", place);
                        memset(place, 0, sizeof(place));
                        --x;
                    }
                }
                fgets(place, sizeof(place), fp1);
            }
            if(igen==0)
            { 
                fgets(place, sizeof(place), fp1);
                int x = atoi(place)+2;
                while(x>0 && fgets(place, sizeof(place), fp1))
                {
                    --x;
                }
            }
        }
        fclose(fp1);
        fclose(fp2);
		for(i=0;i<3;++i)
		{
			printf("%s","itt a vege");
			printf("%i",valaszok[i]);
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