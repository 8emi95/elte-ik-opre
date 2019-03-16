//
//  main.c
//  ministry-research
//
//  Created by Roberto Roque Silva on 04/05/15.
//  Copyright (c) 2015 Roberto Roque Silva. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

char strs[5][5] = {"lion", "bear", "wolf", "bird", "frog"};

void handler(int signumber){
   // printf("Signal with number %i has arrived\n",signumber);
}

int main(int argc, const char * argv[])
{

    signal(SIGTERM, handler); //handler = SIG_IGN - ignore the signal (not SIGKILL,SIGSTOP),
    //handler = SIG_DFL - back to default behavior
    int childs = atoi(argv[1]);
    int expTime = atoi(argv[2]);
    int i, nbytes;
    int fd[2];
    int random = 0;
    char readbuffer[80];

    pipe(fd);
    
    for (i = 0; i < childs; i++) {
        pid_t child = fork();
        if (child > 0) //parent
        {
            pause(); //waits till a signal arrive
            printf("Signal arrived\n", SIGTERM);
            /* Send "string" through the output side of pipe */
            random = rand() % 5;
            printf("Random generated: %d\n", random);
            // Send team name
            write(fd[1], strs[random], 5);
            pause();
            // Wait for team to be ready
            read(fd[0], readbuffer, sizeof(readbuffer));
            printf("Team %s is ready to work.\n", readbuffer);
            int status;
            wait(&status);
            printf("Parent process ended\n");
        }
        else //children
        {
            printf("Waits 1 second, then send a SIGTERM %i signal\n", SIGTERM);
            sleep(1);
            int pid = getppid();
            kill(getppid(), SIGTERM);
            /* Read in a string from the pipe */
            read(fd[0], readbuffer, sizeof(readbuffer));
            char team[5];
            strcpy(team, readbuffer);
            printf("Child #%d team name: %s\n", pid, readbuffer);
            // Child ready to work
            write(fd[1], team, strlen(team)+1);
            kill(getppid(), SIGTERM);
            printf("Child process ended\n");
        }
        
    }

    return 0;
}

