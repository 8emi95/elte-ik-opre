#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

int main()
{
    int parent_to_child_pipe[2];
    int child_to_parent_pipe[2];
    pipe(parent_to_child_pipe);
    pipe(child_to_parent_pipe);
    pid_t child;



    const int sleep_time = 1;


    child = fork();


        if(child < 0)
        {
            printf("Failed to fork!");
            exit(1);
        }
        else if(child != 0)
        {
            close(parent_to_child_pipe[0]);
            char location[256];
            printf("Location: \n");
            scanf ("%s",&location);
            sleep(sleep_time);
            //lokáció küldése
            write(parent_to_child_pipe[1], &location, sizeof(location));

            char message [64];
            //megkapja, hogy sikerült-e a küldés
            read(child_to_parent_pipe[0], message, sizeof(message));
            sleep(sleep_time);

            if(strcmp(message, "ok") == 0)
            {
                char participants[1024];
                printf("Please enumerate the participants separated by ';' : \n");
                scanf ("%s",&participants);
                //elküldi a részvevőket
                write(parent_to_child_pipe[1], &participants, sizeof(participants));
                const char *participant_names[512];
                char *pt;
                pt = strtok (participants,";");
                int i = 0;
                while (pt != NULL)
                {
                    participant_names[i] = pt;
                    ++i;
                    pt = strtok (NULL, ";");

                }
                int stats[i];
                read(child_to_parent_pipe[0], &stats, sizeof(stats));
                int j;
                for(j = 0; j < i; ++j)
                {
                    if(stats[j]==1)
                    {
                        printf("%s: ATTENDED\n",participant_names[j]);
                    }
                    else
                    {
                        printf("%s: NOT ATTENDED\n",participant_names[j]);
                    }

                }



            }

            close(child_to_parent_pipe[0]);


        }
        else
        {
            close(parent_to_child_pipe[1]);
            char location[256];
            // olvassa a lokációt
            read(parent_to_child_pipe[0], &location, sizeof(location));
            sleep(sleep_time);
            close(child_to_parent_pipe[0]);
            //visszajelez
            write(child_to_parent_pipe[1], "ok", sizeof("ok"));
            sleep(sleep_time);
            char participants[1024];
            //mekapja a résztvevőket
            read(parent_to_child_pipe[0], &participants, sizeof(participants));
            const char *participant_names[512];
            char *pt;
            pt = strtok (participants,";");
            int i = 0;
            while (pt != NULL)
            {
                participant_names[i] = pt;
                ++i;
                pt = strtok (NULL, ";");

            }

            int j;
            for (j=0; j < i; j++)
            {
                printf("%d: %s\n",j+1,participant_names[j]);
            }
            int stats[i];
            int k;

            for (k=0; k < i; k++)
            {
                int r = rand() % 10;
                if(r < 9)
                {
                    stats[k] = 1;
                }
                else
                {
                    stats[k] = 0;
                }
            }
            //statisztika elküldése
            write(child_to_parent_pipe[1], &stats,sizeof(stats));

            close(child_to_parent_pipe[1]);
        }
    
    return 0;
}
