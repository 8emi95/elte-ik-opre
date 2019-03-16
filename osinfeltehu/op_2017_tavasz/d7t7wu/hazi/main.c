#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // for pipe()
struct question
{
    char question[512];
    char answer1[256];
    char answer2[256];
    char answer3[256];
    char answer4[256];
    time_t submit_time;
};

typedef struct question Question;

int main()
{
    Question survey1[3];
    Question survey[3];

    int i = 0;
    int pipefdl[2];
    int pipefdw[2];
    int pid = 0;
    char buffer[100];
    int nr;
    int valaszok[60];
    time_t t;

    srand((unsigned) time(&t));


    for(i=0; i < 3; ++i)
    {
        strcpy(survey1[i].question, "kerdes");
        strcpy(survey1[i].answer1, "valasz1");
        strcpy(survey1[i].answer2, "evalasz2");
        strcpy(survey1[i].answer3, "valasz3");
        strcpy(survey1[i].answer4, "-");
        survey1[i].submit_time = time(NULL);
    }
    /* for(i=0; i < 3; ++i)
     {
         printf("%d. %s\n",i,questions[i].question);
         printf("%d. %s\n",i,questions[i].answer1);
         printf("%d. %s\n",i,questions[i].answer2);
         printf("%d. %s\n",i,questions[i].answer3);
         printf("%d. %s\n",i,questions[i].answer4);
     }*/

    if (pipe(pipefdl)<0||pipe(pipefdw)<0)
        //if (pipe(pipefdl)<0)
    {
        perror("problem with the pipes");
        exit(1);
    }

    pid = fork();

    if (pid<0)
    {
        perror("problem with creating the child");
    }

    if (pid>0)
    {
        //parent
        close(pipefdl[1]);//bezarta a pipe iro veget
        printf("szulo olvassa az l pipeot\n");

        read(pipefdl[0], buffer, sizeof(buffer));
        printf("szulo olvasta uzenet: %s\n",buffer);
        // a szulo uzen
        close(pipefdw[0]);
        printf("szulo ir");

        write(pipefdw[1], survey1, 3*sizeof(Question));

        read(pipefdl[0], &nr, sizeof(nr));
        printf("\n%d ez a szuloben a kitoltok szama\n", nr);
        read(pipefdl[0], valaszok, nr*3*sizeof(int));

        printf("szulo\n");
        for(i = 0; i<nr; i++)
        {
            printf("\n%d. question \n 1.answer: %d, 2. answer %d,3. answer %d \n",i+1,valaszok[i*3],valaszok[i*3 + 1],valaszok[i*3 + 2]);
        }

        close(pipefdl[0]);
        wait();
        printf("szulo keszen van\n");
    }
    else
    {
        close(pipefdl[0]);//bezarom a pipe olvaso veget
        strcpy(buffer,"a vizsgalobiztos keszen all\n");
        write(pipefdl[1],buffer, sizeof(buffer));

        close(pipefdw[1]);//bezarom a pipe iro fejet
        read(pipefdw[0], survey, 3*sizeof(Question));

        int nr_kerdes[3];
        for(i=0; i < 3; ++i)
        {
            nr_kerdes[i]=4;
            if (strcmp(survey[i].answer3,"-")==0)
            {
                nr_kerdes[i]=2;
            }
            if (strcmp(survey[i].answer4,"-")==0)
            {
                nr_kerdes[i]=3;
            }
        }
        int emberek=rand()% 10 + 10;

        for(i = 0; i<emberek; i++)
        {
            valaszok[i*3]=rand()%nr_kerdes[0];
            valaszok[i*3 + 1]=rand()%nr_kerdes[1];
            valaszok[i*3 + 2]=rand()%nr_kerdes[2];
        }

        write(pipefdl[1],&emberek, sizeof(emberek));
        write(pipefdl[1],valaszok, emberek*3*sizeof(int));

        close(pipefdw[0]);
        close(pipefdl[1]);
        printf("gyerek kesz\n");
    }

    return 0;
}
