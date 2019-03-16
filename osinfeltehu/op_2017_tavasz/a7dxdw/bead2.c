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

    /* Csővezeték létrehozása */
    int pipefd[2];

    if (pipe(pipefd) == -1) 
	{
        perror("PROGRAM: Nem sikerült létrehozni a pipe-ot!\n\n");
        exit(EXIT_FAILURE);
    }

    /* Gyerek folyamat létrehozása */
    pid_t child = fork();
    if (child < 0) 
    {
        perror ("PROGRAM: Nem sikerült a gyerekfolyamat létrehozása!\n\n");
        exit(-1);
    }

    /* Szülő folyamat */
    if (0 != child)    
    {         
        /* Kigyűjtöm egy fájlba a kérdések azonosítóit */
        char place[200];
        FILE *fp1 = fopen("out.txt", "r");
        FILE *fp2 = fopen("id.txt", "w");
        if(fp1 == NULL)
        {
            printf("PROGRAM: Nem sikerült megnyitni a fájlt!\n\n");
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
        /* Meghatározom az azonosítós fájl hosszát, majd várok a kérdezőbiztos bejelentkezésére */
        int hossz = countLines("id.txt") - 1;

        printf("SZÜLŐ: Várok a gyerek bejelentkezésére!\n\n");
        sleep(1);
        kill(child, SIGTERM);
        pause();
        printf("SZÜLŐ: Megkaptam a szignált, a gyerek bejelentkezett %s", argv[1]);
        printf(" városból, kezdődhet a konzultáció!\n\n");
 
        int k;
        int r[3];
        char hely[3][100];
        char kerdesvalasz[3][220];
        char place2[3][100];
        
        /*Létrehozok 3 random számot, majd a random "számadik" sor értékét beleteszem egy tömbbe */
        srand(time(NULL));
        for (k=0; k<3; ++k)
        {
            r[k]=rand()%(hossz); 
            
            FILE *fp3 = fopen("id.txt", "r");
            while(fgets(place2[k], sizeof(place2), fp3) && r[k]>0){
                r[k]--;
            }
            fclose(fp3);

            /* A megfelelő azonosítóhoz tartozó válaszlehetőségek számát, a kérdést, és a válaszait 
               beleteszem egy tömbbe (kerdesvalasz[3][300]) */
            FILE *fp4 = fopen("asd.txt", "r");
            if(fp4 == NULL)
            {
                printf("PROGRAM: Hiba, nem sikerült a fájl megnyitása!");
                exit(1);
            }
            int igen = 0;
            
            memset(kerdesvalasz[k], 0, sizeof(kerdesvalasz[k]));
            while(fgets(hely[k], sizeof(hely[k]), fp4)){
                igen=0;
                if(atoi(place2[k])==atoi(hely[k]))
                {
                    fgets(hely[k], sizeof(hely[k]), fp1);
                    strcat(kerdesvalasz[k], hely[k]);
                    int x = atoi(hely[k])+1;
                    if(x>0)
                    {
                        while(x>0 && fgets(hely[k], sizeof(hely[k]), fp1))
                        {
                            strcat(kerdesvalasz[k], hely[k]);
                            memset(hely[k], 0, sizeof(hely[k]));
                            --x;
                        }
                    }
                    fgets(hely[k], sizeof(hely[k]), fp1);
                    igen=1;
                }
                if(igen==0)
                {
                    fgets(hely[k], sizeof(hely[k]), fp1);
                    int x = atoi(hely[k])+2;
                    while(x>0 && fgets(hely[k], sizeof(hely[k]), fp1))
                    {
                        --x;
                    }
                }
            }

            fclose(fp4);
        }
        printf("SZÜLŐ: Készen vagyok, megvan a 3 db random kérdés, kezdődhet a kommunikáció!\n\n");
        /* Leküldöm a kérdéseket és a válaszaikat a csövön */
        int j;
        for(j=0;j<3;++j)
        {
            printf("SZÜLŐ: Leküldöm a(z) %i", j+1);
            printf(". kérdést és válaszait!\n\n");
            write(pipefd[1], kerdesvalasz[j], sizeof(kerdesvalasz[j]));
            sleep(1);
            kill(child, SIGTERM);
            pause();
        } 
        j=0;
        char kapottvalaszok[3][25];
        for(j=0; j<3; ++j)
        {
            memset(kapottvalaszok[j], 0, sizeof(kapottvalaszok[j]));
            read(pipefd[0], kapottvalaszok[j], sizeof(kapottvalaszok[j]));
            sleep(1);
            kill(child, SIGTERM);
        }
        int i=0;
        for(i=0; i<3;++i)
        {
            int j=0;
            printf("SZÜLŐ: Megkaptam a(z) %i", i+1);
            printf(". kérdés kitöltését, a polgárok válaszai alapján!\n");
            printf("SZÜLŐ: Az érkezett válaszok sorszámai a(z) %i", i+1);
            printf(". kérdésre rendre: ");
            while(kapottvalaszok[i][j]!='0'){
                printf("%c", kapottvalaszok[i][j]);
                printf(" ");
                ++j;
            }
            printf("\n\n");
        }

        printf("SZÜLŐ: Befejeztem a konzultálást, további szép napot! :)\n\n");
     }    

    /* child process */
    else
    {
        pause();
        printf("GYEREK: Sikerult bejelentkezni az MLSZ-hez %s", argv[1]);
        printf(" városból, folytatódhat a szülő!\n\n");
        sleep(1);
        kill(getppid(), SIGTERM);

        char buffer[3][220];
        int j = 0;
        for(j=0; j<3; ++j){
            pause();
            read(pipefd[0],buffer[j],sizeof(buffer[j]));
            sleep(1);
            kill(getppid(), SIGTERM);
        }
        printf("GYEREK: Megkaptam a 3 db random generált kérdést és válaszait, ezeket a buffer tömbbe helyeztem!\n\n");
        
        srand(time(NULL));
        int rx=rand()%11 + 10; 
        int k;
        printf("GYEREK: A kérdőív kitöltésére kiválasztottam %i", rx);
        printf(" darab konzultálni kívánó polgárt!\n\n");
        int valaszLehetoseg[3];
        for(k=0; k<3; ++k)
        {
            valaszLehetoseg[k] = (int)buffer[k][0]-(int)('0');
        }
        char valaszok[3][25];
        int i;
        for(i=0; i<3; ++i)
        {
            memset(valaszok[i], 0, sizeof(valaszok[i]));
            int j;
            for(j=0; j<rx;++j)
            {
                int temp=rand()%valaszLehetoseg[i]+1;
                char x = (char)(temp + (int)('0'));
                valaszok[i][j]=x;
            }
            valaszok[i][j+1]='0';
        }
        i=0;
        for(i=0; i<3;++i)
        {
            int j=0;
            printf("GYEREK: A polgárok válaszainak sorszámai a %i", i+1 );
            printf(". kérdésre rendre, ahol %i", valaszLehetoseg[i] );
            printf(" db válasz van: ");
            for(j=0; j<rx; ++j)
            {
                printf("%c", valaszok[i][j]);
                printf(" ");
            }
            printf("\n\n");
        }
        printf("GYEREK: Felküldöm a szülőnek a kérdőív kitöltési adatokat!\n\n");
        i=0;
        for(i=0; i<3; ++i)
        {
            write(pipefd[1], valaszok[i], sizeof(valaszok[i]));
            kill(getppid(), SIGTERM);
            pause();
        }
    }

    return 0;
}

void handler(int signumber)
{ 
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