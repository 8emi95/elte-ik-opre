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
    printf("%s\n","Itt vagyok");
    int pipefd[2];
    signal(SIGTERM,handler);
    pid_t child=fork();
    if (pipe(pipefd) == -1) 
	{
       perror("Hiba a pipe nyitaskor!");
       exit(EXIT_FAILURE);
    }
    if (child == -1) 
	{
               perror("Fork hiba");
               exit(EXIT_FAILURE);
    }
    char name[100]="fajl.txt";
    srand (time(NULL));
    if(child>0)
    {
        int numbers[3];
        //int ered=countQuestions(name);
        int ered=3;
        int i;
        for(i=0;i<3;++i)
        {
            int num=rand() % ered;
            numbers[i]=num;
        }
        for(i=0;i<3;++i)
        {
            int num=numbers[i];
            close(pipefd[0]); //Usually we close unused read e
            write(pipefd[1], &num,sizeof(num));
            printf("%i",num);
            close(pipefd[1]); // finally we close the used read end
            //kill(child,SIGTERM);
            //pause();
        }
    }
    else
    {
        int question_numbers[3][100];
        int i;
        sleep(3);
        for(i=0;i<3;++i)
        {
            //pause();
            close(pipefd[1]);  //Usually we close the unused write end
            read(pipefd[0],&question_numbers[i],sizeof(question_numbers[i]));
            printf("%i\n", &question_numbers[i]);
            close(pipefd[0]); // finally we close the used read end

            //kill(getppid(),SIGTERM);
            
        }
    }
    return 0;
}