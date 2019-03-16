#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void handler(int x) { x = x; }
void startEvent(const char* eventIdentifier);

int main(int argc, char **argv)
{
    printf("--beginning of program\n");

    int option;
    char eventIdentifier[100];

    do{
        printf("Choose event to start: ");
        scanf("%s", &eventIdentifier);

        //ha nem létezik az event legyen vége a programnak
        FILE *fp;
        if ((fp = fopen(eventIdentifier, "r")) == NULL)
        {
            printf("Error, event does not exsits!\n");
            exit(1);         
        }
        else
        {
            fclose(fp);
            startEvent(eventIdentifier);
        }

        printf("If you want to quit enter 0 or you can start new event: ");
        scanf("%d", &option);
    }
    while(option != 0);

    printf("--End of program--\n");

    return 0;
}

void startEvent(const char* eventIdentifier){
    int myPipe[2];
    char buffer[300];

    struct sigaction sigact;
    sigact.sa_handler=handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags=0;
    sigaction(SIGUSR1,&sigact,NULL);

    int MyPipeline = pipe(myPipe);
    if (MyPipeline < 0)
    {
        perror("pipe(myPipe) error!\n");
        exit(-1);
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Fork error!\n");
        exit(-1);
    }

    /* parent process */
    if (pid > 0)
    {
        printf("/****Parent process begin****/\n");

        sigset_t sigset;
        sigfillset(&sigset);
        sigdelset(&sigset,SIGUSR1);
        
        //gyereknek elküldjük melyik rendezvényről van szó
        //printf("PARENT|eventIdentifier: %s", eventIdentifier);
        close(myPipe[0]);
        write(myPipe[1], eventIdentifier, sizeof(eventIdentifier));

        //fogadjuk a jelzést a gyerektől
        sigsuspend(&sigset);

        //elküldjük a résztvevőket a gyereknek
        FILE *fp;
        if ((fp = fopen(eventIdentifier, "r")) == NULL)
        {
            printf("PARENT| Error opening file!\n");
            exit(1);         
        }

        int i=0;
        while (fgets(buffer, sizeof(buffer), fp)) {// stop when new line, or reached the size limit
            ++i;
            if(i % 3 == 1){ //első sortól kezdve minden 3. lesz egy név
                //int number = (i/3)+1;
                //printf("PARENT| %i. member: %s", number, buffer);// \n in buffer
                write(myPipe[1], buffer, sizeof(buffer));
                sigsuspend(&sigset);  
            }
        }

        fclose(fp);
        close(myPipe[1]);

        waitpid(pid, 0, 0);
        printf("/****Parent process end****/\n");
    }
    /* child process */
    else if (pid == 0)
    {
        printf("/****Child process begin****/\n");

        // beolvassuk a rendezvényt
        close(myPipe[1]);
        read(myPipe[0], buffer, sizeof(buffer));
        printf("CHILD| eventIdentifier: %s \n", buffer); 

        //jelzést küld vissza a parentnek, hogy megérkeztünk, küldheti a résztvevőket
        kill(getppid(), SIGUSR1);
        printf("CHILD| I have arrived\n");

        //fogadjuk a résztvevőket és kiírjuk a képernyőre
        printf("CHILD| List of guests:\n");

        int num = 1;
        while( read(myPipe[0], buffer, sizeof(buffer)) > 0)
        {
            kill(getppid(), SIGUSR1);
            printf("CHILD| %d. member: %s", num, buffer);
            num++;
        }
        printf("\n");

        //kiírjuk hogy kik nem vettek részt
        printf("Missed guests:\n");
        srand (time(NULL));
        int result = num-1;
        int sumMissed=0;
        int i;
        for(i=1; i < num; i++)
        {
            if(rand() % 10 == 0){
                printf("CHILD| %d. member missed!\n", i);
                sumMissed++;
                result--;
            }
        }

        //kiírjuk, hogy mennyire volt sikeres
        printf("CHILD| %d guests were here from %d!\n", result, num-1);
        
        close(myPipe[0]);

        printf("/****Child process end****/\n");
        exit(0);
    }
    /* fork failed */
    else
    { 
        printf("fork() failed!\n");
        exit(1);
    }
}