#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <signal.h>

void handler(int signumber)
{
    //printf("Signal with number %i has arrived\n",signumber);
}

int main(int argc, char **argv)
{

    signal(SIGTERM,handler);

    int parent_to_child_pipe[2];
    int child_to_parent_pipe[2];
    pipe(parent_to_child_pipe);
    pipe(child_to_parent_pipe);
    pid_t child;

    child = fork();

    if(child < 0)
    {
        printf("Failed to fork!");
        exit(1);
    }
    else if(child != 0)
    {
        int ossz = atoi(argv[1]);
        //printf(" szulo ossz: %d\n", ossz);
        close(parent_to_child_pipe[0]);
        int szerelveny_szam = (rand() % ossz) + 1;


        write(parent_to_child_pipe[1], &szerelveny_szam, sizeof(szerelveny_szam));
        write(parent_to_child_pipe[1], &ossz, sizeof(ossz));

        pause();
        //printf("Signal arrived\n",SIGTERM);

        char uzenet[250];
        //printf("haladok tovabb\n");
        read(child_to_parent_pipe[0], &uzenet, sizeof(uzenet));
        //printf("beolvastam\n");

        //printf(uzenet);
        printf("szulo: %s\n",uzenet);

    }
    else
    {
        close(parent_to_child_pipe[1]);
        int sz;
        int o;

        read(parent_to_child_pipe[0], &sz, sizeof(sz));
        read(parent_to_child_pipe[0], &o, sizeof(o));
        printf("gyerek: szerelveny_szam: %d\n", sz);
        printf("gyerek: maradt meg: %d\n", o-sz);

        int baleset = rand() % 10;
        //char baleset_szoveg[256];
        if(baleset == 9)
        {
            printf("gyerek: elcsusztam egy bananon\n");
            //baleset_szoveg = "elcsusztam egy bananon";
        }
        else if(baleset == 8)
        {
            printf("gyerek: lefagytam\n");
            //baleset_szoveg = "lefagytam";

        }
        else
        {
            printf("gyerek: sikeresen megerkeztem\n");
            //baleset_szoveg = "sikeresen megerkeztem";

        }
        kill(getppid(),SIGTERM);

        close(child_to_parent_pipe[0]);

        if(baleset == 9)
        {
            write(child_to_parent_pipe[1], "elcsusztam egy bananon\n", sizeof("elcsusztam egy bananon\n"));
            //baleset_szoveg = "elcsusztam egy bananon";
        }
        else if(baleset == 8)
        {
            //printf("gyerek: lefagytam");
            write(child_to_parent_pipe[1], "lefagytam\n", sizeof("lefagytam\n"));
            //baleset_szoveg = "lefagytam";

        }
        else
        {
            write(child_to_parent_pipe[1], "sikeresen megerkeztem\n", sizeof("sikeresen megerkeztem\n"));
            //baleset_szoveg = "sikeresen megerkeztem";

        }

        exit(0);

    }

    return 0;
}
