#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void handler(int signumber){
}
int countQuestions(char name[100])
{
     FILE *fp1 = fopen(name, "r");
     int db=0;
     if(fp1 == NULL)
    {
        printf("Error!");
        exit(1);
    }
    char place[100];
        while(fgets(place, sizeof(place), fp1))
        {
            fgets(place, sizeof(place), fp1);
            int x = atoi(place)+2;
            while(x>0 && fgets(place, sizeof(place), fp1))
            {
                --x;
            }
            db++;
       }
   return db;
}
int main()
{
    printf("%s","Itt vagyok");
    int pipefd[2];
    signal(SIGTERM,handler);
    if (pipe(pipefd) == -1) 
	{
       perror("Hiba a pipe nyitaskor!");
       exit(EXIT_FAILURE);
    }
    pid_t child=fork();
    if (child == -1) 
	{
               perror("Fork hiba");
               exit(EXIT_FAILURE);
    }
    char name[100]="fajl.txt";
    if(child>0)
    {
       int numbers[3];
        int ered=countQuestions(name);
        int i;
        kill(child,SIGTERM);
        printf("%s","Elkuldtem a signalt");
        pause();
        srand (time(NULL));
        for(i=0;i<3;++i)
        {
            int num=rand() % ered;
            numbers[i]=num;
        }
        for(i=0;i<3;++i)
        {
            int num=numbers[i];
            write(pipefd[1], &num,sizeof(num));
            printf("%i",num);
            pause();
            kill(child,SIGTERM);
            //pause();
        }
    }
    else
    {
        pause();
        printf("%s","Indulok a varosba");
        kill(getppid(),SIGTERM);
        printf("%s","Bejelentkeztem");
        int question_numbers[3][100];
        int i;
        for(i=0;i<3;++i)
        {
            pause();
            read(pipefd[0],question_numbers[i],sizeof(question_numbers[i]));
            printf("%i", question_numbers[i]);
            kill(getppid(),SIGTERM);  
        }
    }
    return 0;
}